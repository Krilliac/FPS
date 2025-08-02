#include "../Core/Engine.h"
#include "../Game/Game.h"
#include "../Utils/Logger.h"
#include <Windows.h>

using namespace SparkEngine;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    Logger::Initialize();

    Engine engine;
    if (!engine.Initialize()) return -1;

    // Initialize window etc.
    HWND hwnd = /* create window and retrieve HWND */;
    engine.GetGraphics().Initialize(hwnd, 1280, 720);
    engine.GetInput().Initialize(hwnd);

    Game game;
    game.Initialize(engine);

    engine.Run();

    Logger::Shutdown();
    return 0;
}
