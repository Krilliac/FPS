#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>   // std::max
#include <sstream>     // std::wistringstream

// -----------------------------------------------------------------------------
// VERY-SIMPLE TEXT OUTPUT
// For now we just dump to the Visual Studio Output window.  Replace this with
// SpriteFont/SpriteBatch or DirectWrite later.
// -----------------------------------------------------------------------------
inline void DrawText(const std::wstring& text,
    float /*x*/, float /*y*/, float /*scale*/,
    ID3D11DeviceContext* /*ctx*/)
{
    OutputDebugStringW(text.c_str());
    OutputDebugStringW(L"\n");
}

// -----------------------------------------------------------------------------
// Console
// -----------------------------------------------------------------------------
struct ConsoleCommand
{
    std::wstring name;
    std::function<void(const std::vector<std::wstring>&)> callback;
};

class Console
{
public:
    void Initialize(int screenW, int screenH);
    void Toggle() { visible = !visible; ShowCursor(visible); }
    bool IsVisible()    const { return visible; }

    bool HandleChar(wchar_t c);       // WM_CHAR
    bool HandleKeyDown(WPARAM key);   // WM_KEYDOWN

    void Log(const std::wstring& msg);
    void Render(ID3D11DeviceContext* ctx);

private:
    bool                      visible{ false };
    int                       width{ 0 }, height{ 0 };

    std::vector<std::wstring> buffer;       // rolling log
    std::wstring              inputLine;    // current user input
    std::vector<ConsoleCommand> commands;   // registered commands

    void ExecuteCommand(const std::wstring& line);
};