#pragma once
#include <string>

struct CrashConfig {
    std::wstring dumpPrefix            = L"GameEngineCrash";
    std::string  uploadURL             = "";
    bool         captureScreenshot     = true;
    bool         captureSystemInfo     = true;
    bool         captureAllThreads     = true;
    bool         zipBeforeUpload       = true;
    int          connectTimeoutSeconds = 5;
};

// Install the unhandled‐exception filter
void InstallCrashHandler(const CrashConfig& cfg);

// Called by Assert::Fail to trigger the crash pipeline
void TriggerCrashHandler(const char* assertMsg);
