#include <imgui.h>
#include "Utils/FileWatcher.h"
#include "Utils/Assert.h"
#include "Utils/CrashHandler.h"
#include "Assets/AssetSystem.h"
#include "Editor/EditorSystem.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Scripting/ScriptingSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Graphics/Systems/RenderSystem.h"
#include "Game/SparkEngineGame.h"
#include "PhysicsSystem.h"
#include <iostream>

namespace SparkEngine {
    PhysicsSystem::~PhysicsSystem() { Shutdown(); }

    bool PhysicsSystem::Initialize(EntityRegistry* registry) {
        m_registry = registry;
        m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);
        m_physics   = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale(), true, m_pvd);
        #ifdef _DEBUG
        m_pvd = physx::PxCreatePvd(*m_foundation);
        physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        m_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
        #endif
        if (!m_physics) return false;
        physx::PxSceneDesc desc(m_physics->getTolerancesScale());
        desc.gravity       = physx::PxVec3(0.0f, -9.81f, 0.0f);
        m_dispatcher       = physx::PxDefaultCpuDispatcherCreate(2);
        desc.cpuDispatcher = m_dispatcher;
        desc.filterShader  = physx::PxDefaultSimulationFilterShader;
        m_scene            = m_physics->createScene(desc);
        m_defaultMaterial  = m_physics->createMaterial(0.5f,0.5f,0.1f);
        m_initialized = true;
        return true;
    }

    void PhysicsSystem::Update(float dt) {
        if (!m_initialized) return;
        m_timeAccumulator += dt;
        while (m_timeAccumulator >= m_stepSize) {
            m_scene->simulate(m_stepSize);
            m_scene->fetchResults(true);
            SyncTransforms();
            m_timeAccumulator -= m_stepSize;
        }
    }

    void PhysicsSystem::Shutdown() {
        if (m_scene)       { m_scene->release();       m_scene=null; }
        if (m_dispatcher) { m_dispatcher->release(); m_dispatcher=null; }
        if (m_physics)    { m_physics->release();    m_physics=null; }
        if (m_foundation) { m_foundation->release(); m_foundation=null; }
        m_initialized = false;
    }

    void PhysicsSystem::SyncTransforms() {
        auto view = m_registry->View<RigidBodyComponent, TransformComponent>();
        for (auto e : view) {
            auto& rb = view.get<RigidBodyComponent>(e);
            physx::PxTransform t = rb.actor->getGlobalPose();
            view.get<TransformComponent>(e).position = {t.p.x, t.p.y, t.p.z};
        }
    }
}