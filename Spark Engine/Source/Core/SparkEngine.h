#pragma once
#include "resource.h"
#include <memory>

// Forward declarations
class GraphicsEngine;
class Game;
class InputManager;
class Timer;

// Global variables
extern HINSTANCE hInst;
extern std::unique_ptr<GraphicsEngine> g_graphics;
extern std::unique_ptr<Game> g_game;
extern std::unique_ptr<InputManager> g_input;
extern std::unique_ptr<Timer> g_timer;

