#pragma once
#include "../Core/framework.h"
namespace SparkEngine {
class RenderThreadManager {
public: RenderThreadManager()=default; ~RenderThreadManager()=default;
  bool Initialize(){return true;} void Shutdown(){}
};
}
