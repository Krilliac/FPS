#pragma once
#include "../Core/framework.h"
namespace SparkEngine {
class InputSystem {
public: InputSystem()=default; ~InputSystem()=default;
  void Update(){} bool IsKeyPressed(int){return false;} bool IsMouseButtonPressed(int){return false;}
};
}
