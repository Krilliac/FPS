#include "PhysicsSystem.h"
#include "../Utils/Logger.h"

#ifdef PHYSX_AVAILABLE
namespace SparkEngine {
    PhysicsSystem::PhysicsSystem() {}
    PhysicsSystem::~PhysicsSystem() { Shutdown(); }

    bool PhysicsSystem::Initialize() {
        Logger::Info("Initializing PhysX...");
        m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);
        if (!m_foundation) { Logger::Error("PxCreateFoundation failed"); return false; }
        m_pvd = PxCreatePvd(*m_foundation);
        PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

        m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), true, m_pvd);
        if (!m_physics) { Logger::Error("PxCreatePhysics failed"); return false; }

        PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        m_scene = m_physics->createScene(sceneDesc);
        if (!m_scene) { Logger::Error("createScene failed"); return false; }

        Logger::Info("PhysX initialized");
        return true;
    }

    void PhysicsSystem::Shutdown() {
        Logger::Info("Shutting down PhysX...");
        if (m_scene) { m_scene->release(); m_scene = nullptr; }
        if (m_physics) { m_physics->release(); m_physics = nullptr; }
        if (m_pvd) { m_pvd->release(); m_pvd = nullptr; }
        if (m_foundation) { m_foundation->release(); m_foundation = nullptr; }
        Logger::Info("PhysX shutdown complete");
    }

    void PhysicsSystem::Update(float deltaTime) {
        Simulate(deltaTime);
    }

    void PhysicsSystem::Simulate(float deltaTime) {
        if (!m_scene) return;
        m_scene->simulate(deltaTime);
        m_scene->fetchResults(true);
    }

    PxRigidDynamic* PhysicsSystem::CreateRigidDynamic(const PxTransform& transform) {
        if (!m_physics || !m_scene) return nullptr;
        PxRigidDynamic* actor = m_physics->createRigidDynamic(transform);
        m_scene->addActor(*actor);
        return actor;
    }
}
#endif
