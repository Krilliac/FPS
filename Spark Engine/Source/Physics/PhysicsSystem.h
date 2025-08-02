#pragma once
#include "../Core/framework.h"

#ifdef PHYSX_AVAILABLE
#include <PxPhysicsAPI.h>
#endif

namespace SparkEngine {
#ifdef PHYSX_AVAILABLE
    using namespace physx;
    class PhysicsSystem {
    public:
        PhysicsSystem();
        ~PhysicsSystem();

        bool Initialize();
        void Shutdown();
        void Update(float deltaTime);

        PxRigidDynamic* CreateRigidDynamic(const PxTransform& transform);
        void Simulate(float deltaTime);

    private:
        PxDefaultAllocator      m_allocator;
        PxDefaultErrorCallback  m_errorCallback;
        PxFoundation*           m_foundation = nullptr;
        PxPhysics*              m_physics = nullptr;
        PxScene*                m_scene = nullptr;
        PxControllerManager*    m_controllerManager = nullptr;
        PxPvd*                  m_pvd = nullptr;
    };
#endif
}
