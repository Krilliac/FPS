#pragma once
#include "../../Core/framework.h"
#include "../../ECS/EntityRegistry.h"
namespace SparkEngine {
class GraphicsEngine;
class RenderSystem {
public:
  RenderSystem(EntityRegistry&,GraphicsEngine&);
  ~RenderSystem();
  void Update(float); void Render();
private:
  EntityRegistry& m_registry;
  GraphicsEngine& m_graphics;
};
}
