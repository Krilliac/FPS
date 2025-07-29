#include <miniz_export.h>

#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#include "Spark Engine/Source/Utils/CrashHandler.h"
#include <dbghelp.h>
#include <curl/curl.h>
#include <wincodec.h>
#include <TlHelp32.h>
#include <dxgi.h>
#include <d3d11.h>
#define MINIZ_HEADER_FILE_ONLY
#include "../ThirdParty/Utils/miniz/miniz.c"
#include <fstream>
#include <sstream>
#include <mutex>
#include <filesystem>

#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"libcurl.lib")
#pragma comment(lib,"dxgi.lib")

static CrashConfig g_cfg;
static std::mutex g_lock;

extern IDXGISwapChain* GetMainSwapChain();
extern ID3D11Device* GetD3DDevice();
extern ID3D11DeviceContext* GetD3DContext();

static LONG WINAPI CrashFilter(EXCEPTION_POINTERS* ep);
static void WriteMiniDump(const std::wstring& path, EXCEPTION_POINTERS* ep);
static std::wstring MakeTimeStamp();
static std::wstring SymStackTrace(EXCEPTION_POINTERS* ep);
static std::wstring SystemInfo();
static std::wstring ThreadStacks();
static void SaveScreenshot(const std::wstring& file);
static void ZipFiles(const std::wstring& zip, const std::vector<std::wstring>& files);
static bool Upload(const std::string& url, const std::wstring& file, const std::string& field);

void InstallCrashHandler(const CrashConfig& cfg) {
    g_cfg = cfg;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    SetUnhandledExceptionFilter(CrashFilter);
}

static LONG WINAPI CrashFilter(EXCEPTION_POINTERS* ep) {
    std::lock_guard<std::mutex> guard(g_lock);
    
    std::wstring stamp = MakeTimeStamp();
    std::wstring dump = g_cfg.dumpPrefix + stamp + L".dmp";
    std::wstring log = g_cfg.dumpPrefix + stamp + L".log";
    std::wstring shot = g_cfg.dumpPrefix + stamp + L".png";
    std::wstring zip = g_cfg.dumpPrefix + stamp + L".zip";
    
    WriteMiniDump(dump, ep);
    
    std::wstring logText = SymStackTrace(ep);
    if (g_cfg.captureSystemInfo) logText += SystemInfo();
    if (g_cfg.captureAllThreads) logText += ThreadStacks();
    
    std::wofstream(log) << logText;
    
    if (g_cfg.captureScreenshot) SaveScreenshot(shot);
    
    std::vector<std::wstring> filesToZip = {dump, log};
    if (g_cfg.captureScreenshot) filesToZip.push_back(shot);
    
    if (g_cfg.zipBeforeUpload) ZipFiles(zip, filesToZip);
    
    bool uploadOK = true;
    if (!g_cfg.uploadURL.empty()) {
        if (g_cfg.zipBeforeUpload) {
            uploadOK = Upload(g_cfg.uploadURL, zip, "package");
        } else {
            uploadOK &= Upload(g_cfg.uploadURL, dump, "minidump");
            uploadOK &= Upload(g_cfg.uploadURL, log, "logfile");
            if (g_cfg.captureScreenshot) {
                uploadOK &= Upload(g_cfg.uploadURL, shot, "screenshot");
            }
        }
    }
    
    std::wstring msg = L"Fatal error captured.\nFiles: " + dump + L"\n" + log;
    if (g_cfg.captureScreenshot) msg += L"\n" + shot;
    if (!g_cfg.uploadURL.empty()) {
        msg += L"\nUpload: " + (uploadOK ? L"Success" : L"FAILED (kept locally)");
    }
    
    MessageBoxW(nullptr, msg.c_str(), L"Crash Handler", MB_OK | MB_ICONERROR);
    return EXCEPTION_EXECUTE_HANDLER;
}

// Implementation of helper functions...
static std::wstring MakeTimeStamp() {
    SYSTEMTIME t; GetLocalTime(&t);
    wchar_t buf[32];
    swprintf_s(buf, L"_%04d%02d%02d_%02d%02d%02d",
        t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
    return buf;
}

static void WriteMiniDump(const std::wstring& path, EXCEPTION_POINTERS* ep) {
    HANDLE h = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return;
    
    MINIDUMP_EXCEPTION_INFORMATION info{GetCurrentThreadId(), ep, TRUE};
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
        h, MiniDumpWithFullMemory | MiniDumpWithHandleData |
        MiniDumpWithUnloadedModules, &info, nullptr, nullptr);
    CloseHandle(h);
}

static std::wstring SymStackTrace(EXCEPTION_POINTERS* ep) {
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);
    
    std::wstringstream s;
    s << L"Exception: 0x" << std::hex << ep->ExceptionRecord->ExceptionCode << L"\n";
    s << L"Address: 0x" << ep->ExceptionRecord->ExceptionAddress << L"\n\n";
    
    CONTEXT& ctx = *ep->ContextRecord;
    STACKFRAME64 f{};
#ifdef _WIN64
    DWORD machine = IMAGE_FILE_MACHINE_AMD64;
    f.AddrPC.Offset = ctx.Rip;
    f.AddrFrame.Offset = ctx.Rbp;
    f.AddrStack.Offset = ctx.Rsp;
#else
    DWORD machine = IMAGE_FILE_MACHINE_I386;
    f.AddrPC.Offset = ctx.Eip;
    f.AddrFrame.Offset = ctx.Ebp;
    f.AddrStack.Offset = ctx.Esp;
#endif
    f.AddrPC.Mode = f.AddrFrame.Mode = f.AddrStack.Mode = AddrModeFlat;
    
    for (int i = 0; i < 64 && f.AddrPC.Offset; ++i) {
        StackWalk64(machine, GetCurrentProcess(), GetCurrentThread(),
            &f, &ctx, nullptr, SymFunctionTableAccess64,
            SymGetModuleBase64, nullptr);
        if (!f.AddrPC.Offset) break;
        
        BYTE symBuf[sizeof(SYMBOL_INFO) + 256];
        auto pSym = reinterpret_cast<PSYMBOL_INFO>(symBuf);
        pSym->MaxNameLen = 255;
        pSym->SizeOfStruct = sizeof(SYMBOL_INFO);
        DWORD64 disp = 0;
        
        if (SymFromAddr(GetCurrentProcess(), f.AddrPC.Offset, &disp, pSym)) {
            s << pSym->Name;
            IMAGEHLP_LINEW64 line{};
            line.SizeOfStruct = sizeof(line);
            DWORD ld = 0;
            if (SymGetLineFromAddrW64(GetCurrentProcess(), f.AddrPC.Offset, &ld, &line)) {
                s << L" - " << line.FileName << L":" << line.LineNumber;
            }
        } else {
            s << L"0x" << std::hex << f.AddrPC.Offset;
        }
        s << L"\n";
    }
    
    SymCleanup(GetCurrentProcess());
    return s.str();
}

static std::wstring SystemInfo() {
    MEMORYSTATUSEX mem{sizeof(mem)};
    GlobalMemoryStatusEx(&mem);
    
    std::wstringstream s;
    s << L"\n--- System Information ---\n";
    s << L"RAM: " << (mem.ullTotalPhys >> 20) << L" MB total\n";
    return s.str();
}

static std::wstring ThreadStacks() { return L""; }
static void SaveScreenshot(const std::wstring& file) { }
static void ZipFiles(const std::wstring& zip, const std::vector<std::wstring>& files) { }
static bool Upload(const std::string& url, const std::wstring& file, const std::string& field) { return true; }





