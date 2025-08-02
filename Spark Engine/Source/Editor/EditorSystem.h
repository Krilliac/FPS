#pragma once
#include "../Core/framework.h"
namespace SparkEngine {
class EditorSystem {
public: EditorSystem()=default; ~EditorSystem()=default;
  bool Initialize(){return true;} void Shutdown(){} void Update(){} void Render(){}
};
}
