#pragma once
#include "../Core/framework.h"
#include "../ECS/System.h"
#include "../ECS/Components.h"
#include <PxPhysicsAPI.h>
#include <memory>

namespace SparkEngine {
    // Physics Components
    struct RigidBodyComponent {
        physx::PxRigidActor* actor = nullptr;
        bool isDynamic = true;
        float mass = 1.0f;
        DirectX::XMFLOAT3 velocity = {0.0f, 0.0f, 0.0f};
        DirectX::XMFLOAT3 angularVelocity = {0.0f, 0.0f, 0.0f};
        bool kinematic = false;
        float linearDamping = 0.1f;
        float angularDamping = 0.1f;

        RigidBodyComponent() = default;
        ~RigidBodyComponent() = default;
    };

    struct ColliderComponent {
        physx::PxShape* shape = nullptr;
        physx::PxGeometry* geometry = nullptr;
        physx::PxMaterial* material = nullptr;

        enum Type { Box, Sphere, Capsule, Mesh };
        Type type = Box;

        // Box collider data
        DirectX::XMFLOAT3 boxHalfExtents = {0.5f, 0.5f, 0.5f};

        // Sphere collider data
        float sphereRadius = 0.5f;

        // Capsule collider data
        float capsuleRadius = 0.5f;
        float capsuleHeight = 2.0f;

        // Material properties
        float staticFriction = 0.5f;
        float dynamicFriction = 0.5f;
        float restitution = 0.1f;

        bool isTrigger = false;

        ColliderComponent() = default;
        ~ColliderComponent() = default;
    };

    class PhysicsSystem : public System {
    private:
        physx::PxDefaultAllocator m_allocator;
        physx::PxDefaultErrorCallback m_errorCallback;
        physx::PxFoundation* m_foundation = nullptr;
        physx::PxPhysics* m_physics = nullptr;
        physx::PxScene* m_scene = nullptr;
        physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
        physx::PxPvd* m_pvd = nullptr;

        // Default materials
        physx::PxMaterial* m_defaultMaterial = nullptr;

        float m_timeAccumulator = 0.0f;
        float m_stepSize = 1.0f / 60.0f;  // 60 FPS physics

        bool m_initialized = false;

    public:
        PhysicsSystem() = default;
        ~PhysicsSystem() override;

        bool Initialize(EntityRegistry* registry) override;
        void Update(float deltaTime) override;
        void Shutdown() override;
        const char* GetName() const override { return "PhysicsSystem"; }

        // Rigidbody management
        void CreateRigidBody(Entity entity, bool isDynamic = true, float mass = 1.0f);
        void UpdateRigidBodyTransform(Entity entity);
        void SetRigidBodyVelocity(Entity entity, const DirectX::XMFLOAT3& velocity);
        void AddForce(Entity entity, const DirectX::XMFLOAT3& force, bool impulse = false);
        void AddTorque(Entity entity, const DirectX::XMFLOAT3& torque, bool impulse = false);

        // Collider management
        void CreateBoxCollider(Entity entity, const DirectX::XMFLOAT3& halfExtents);
        void CreateSphereCollider(Entity entity, float radius);
        void CreateCapsuleCollider(Entity entity, float radius, float height);

        // Material management
        physx::PxMaterial* CreateMaterial(float staticFriction, float dynamicFriction, float restitution);
        physx::PxMaterial* GetDefaultMaterial() { return m_defaultMaterial; }

        // Scene queries
        bool Raycast(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& direction, 
                    float maxDistance, physx::PxRaycastHit& hit);
        bool SphereCast(const DirectX::XMFLOAT3& origin, float radius, 
                       const DirectX::XMFLOAT3& direction, float maxDistance, 
                       physx::PxSweepHit& hit);

        // Settings
        void SetGravity(const DirectX::XMFLOAT3& gravity);
        DirectX::XMFLOAT3 GetGravity() const;

        physx::PxScene* GetScene() { return m_scene; }

    private:
        void SyncTransforms();
        void UpdatePhysicsTransforms();
        physx::PxTransform XMFLOATToPxTransform(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot);
        void PxTransformToXMFLOAT(const physx::PxTransform& pxTransform, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& rot);
    };
}