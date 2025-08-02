#pragma once
#include "../Core/framework.h"
namespace SparkEngine {
class AudioSystem {
public: AudioSystem()=default; ~AudioSystem()=default;
  bool Initialize(){return true;} void Shutdown(){} void Update(){}
};
}
