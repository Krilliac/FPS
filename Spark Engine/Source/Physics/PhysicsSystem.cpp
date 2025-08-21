/**
 * @file PhysicsSystem.cpp
 * @brief Implementation of complete physics integration system
 * @author Spark Engine Team
 * @date 2025
 */

#include "PhysicsSystem.h"
#include "Utils/Assert.h"
#include "../Utils/SparkConsole.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>

using namespace DirectX;

// ============================================================================
// PHYSICS BODY IMPLEMENTATION (Stub for now)
// ============================================================================

PhysicsBody::PhysicsBody(const PhysicsBodyDesc& desc, btRigidBody* bulletBody)
    : m_desc(desc), m_bulletBody(bulletBody)
{
}

PhysicsBody::~PhysicsBody()
{
    // In a real implementation, this would clean up Bullet Physics resources
}

XMFLOAT3 PhysicsBody::GetPosition() const
{
    // Stub implementation - return stored position
    return m_desc.position;
}

void PhysicsBody::SetPosition(const XMFLOAT3& position)
{
    m_desc.position = position;
    // In real implementation, would update Bullet Physics body
}

XMFLOAT3 PhysicsBody::GetRotation() const
{
    return m_desc.rotation;
}

void PhysicsBody::SetRotation(const XMFLOAT3& rotation)
{
    m_desc.rotation = rotation;
    // In real implementation, would update Bullet Physics body
}

XMMATRIX PhysicsBody::GetTransform() const
{
    XMMATRIX translation = XMMatrixTranslation(m_desc.position.x, m_desc.position.y, m_desc.position.z);
    XMMATRIX rotation = XMMatrixRotationRollPitchYaw(m_desc.rotation.x, m_desc.rotation.y, m_desc.rotation.z);
    return rotation * translation;
}

void PhysicsBody::SetTransform(const XMMATRIX& transform)
{
    XMVECTOR scale, rotation, translation;
    XMMatrixDecompose(&scale, &rotation, &translation, transform);
    
    XMStoreFloat3(&m_desc.position, translation);
    
    // Convert quaternion to Euler angles (simplified)
    XMFLOAT4 rotQuat;
    XMStoreFloat4(&rotQuat, rotation);
    m_desc.rotation = {rotQuat.x, rotQuat.y, rotQuat.z}; // Simplified conversion
}

XMFLOAT3 PhysicsBody::GetLinearVelocity() const
{
    return m_desc.linearVelocity;
}

void PhysicsBody::SetLinearVelocity(const XMFLOAT3& velocity)
{
    m_desc.linearVelocity = velocity;
}

XMFLOAT3 PhysicsBody::GetAngularVelocity() const
{
    return m_desc.angularVelocity;
}

void PhysicsBody::SetAngularVelocity(const XMFLOAT3& velocity)
{
    m_desc.angularVelocity = velocity;
}

void PhysicsBody::ApplyForce(const XMFLOAT3& force, const XMFLOAT3& relativePos)
{
    // Stub implementation - in real version would apply to Bullet Physics body
}

void PhysicsBody::ApplyImpulse(const XMFLOAT3& impulse, const XMFLOAT3& relativePos)
{
    // Stub implementation - in real version would apply to Bullet Physics body
}

void PhysicsBody::ApplyTorque(const XMFLOAT3& torque)
{
    // Stub implementation
}

void PhysicsBody::ApplyTorqueImpulse(const XMFLOAT3& torque)
{
    // Stub implementation
}

float PhysicsBody::GetMass() const
{
    return m_desc.mass;
}

void PhysicsBody::SetMass(float mass)
{
    m_desc.mass = mass;
}

void PhysicsBody::SetMaterial(const PhysicsMaterial& material)
{
    m_desc.material = material;
}

void PhysicsBody::SetActive(bool active)
{
    // Stub implementation
}

bool PhysicsBody::IsActive() const
{
    return true; // Stub implementation
}

void PhysicsBody::SetKinematic(bool kinematic)
{
    m_desc.isKinematic = kinematic;
}

bool PhysicsBody::IsKinematic() const
{
    return m_desc.isKinematic;
}

void PhysicsBody::SetTrigger(bool trigger)
{
    m_desc.isTrigger = trigger;
}

bool PhysicsBody::IsTrigger() const
{
    return m_desc.isTrigger;
}

void PhysicsBody::SetCollisionGroup(uint16_t group)
{
    m_collisionGroup = group;
}

void PhysicsBody::SetCollisionMask(uint16_t mask)
{
    m_collisionMask = mask;
}

std::string PhysicsBody::GetInfo() const
{
    std::stringstream ss;
    ss << "Physics Body: " << m_desc.name << "\n";
    ss << "Type: " << PhysicsBodyTypeToString(m_desc.type) << "\n";
    ss << "Position: (" << m_desc.position.x << ", " << m_desc.position.y << ", " << m_desc.position.z << ")\n";
    ss << "Mass: " << m_desc.mass << "\n";
    ss << "Active: " << (IsActive() ? "Yes" : "No") << "\n";
    ss << "Kinematic: " << (m_desc.isKinematic ? "Yes" : "No") << "\n";
    ss << "Trigger: " << (m_desc.isTrigger ? "Yes" : "No") << "\n";
    return ss.str();
}

void PhysicsBody::Console_SetProperty(const std::string& property, float value)
{
    if (property == "mass") {
        SetMass(value);
    } else if (property == "friction") {
        m_desc.material.friction = value;
    } else if (property == "restitution") {
        m_desc.material.restitution = value;
    }
}

void PhysicsBody::Console_ApplyForce(float x, float y, float z)
{
    ApplyForce({x, y, z});
}

// ============================================================================
// PHYSICS CONSTRAINT IMPLEMENTATION (Stub)
// ============================================================================

PhysicsConstraint::PhysicsConstraint(ConstraintType type, btTypedConstraint* bulletConstraint)
    : m_type(type), m_bulletConstraint(bulletConstraint)
{
}

PhysicsConstraint::~PhysicsConstraint()
{
    // In real implementation, would clean up Bullet Physics constraint
}

void PhysicsConstraint::SetEnabled(bool enabled)
{
    // Stub implementation
}

bool PhysicsConstraint::IsEnabled() const
{
    return true; // Stub
}

void PhysicsConstraint::SetBreakingThreshold(float threshold)
{
    // Stub implementation
}

float PhysicsConstraint::GetBreakingThreshold() const
{
    return 0.0f; // Stub
}

// ============================================================================
// PHYSICS SYSTEM IMPLEMENTATION
// ============================================================================

PhysicsSystem::PhysicsSystem()
    : m_dynamicsWorld(nullptr)
    , m_collisionConfig(nullptr)
    , m_dispatcher(nullptr)
    , m_broadphase(nullptr)
    , m_solver(nullptr)
    , m_debugDrawer(nullptr)
{
}

PhysicsSystem::~PhysicsSystem()
{
    Shutdown();
}

HRESULT PhysicsSystem::Initialize()
{
    // Initialize default material
    m_defaultMaterial.friction = 0.5f;
    m_defaultMaterial.restitution = 0.1f;
    m_defaultMaterial.linearDamping = 0.1f;
    m_defaultMaterial.angularDamping = 0.1f;
    m_defaultMaterial.density = 1.0f;
    m_defaultMaterial.name = "Default";
    
    // Initialize metrics
    memset(&m_metrics, 0, sizeof(m_metrics));
    
    // Note: In a real implementation, this would initialize Bullet Physics:
    // m_collisionConfig = new btDefaultCollisionConfiguration();
    // m_dispatcher = new btCollisionDispatcher(m_collisionConfig);
    // m_broadphase = new btDbvtBroadphase();
    // m_solver = new btSequentialImpulseConstraintSolver();
    // m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfig);
    // m_dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
    
    Spark::SimpleConsole::GetInstance().LogSuccess("PhysicsSystem initialized successfully (stub implementation)");
    return S_OK;
}

void PhysicsSystem::Shutdown()
{
    // Clear all bodies and constraints
    m_bodies.clear();
    m_constraints.clear();
    m_namedBodies.clear();
    
    // Note: In real implementation, would clean up Bullet Physics:
    // delete m_dynamicsWorld;
    // delete m_solver;
    // delete m_broadphase;
    // delete m_dispatcher;
    // delete m_collisionConfig;
    
    m_dynamicsWorld = nullptr;
    m_solver = nullptr;
    m_broadphase = nullptr;
    m_dispatcher = nullptr;
    m_collisionConfig = nullptr;
    
    Spark::SimpleConsole::GetInstance().LogInfo("PhysicsSystem shutdown complete");
}

void PhysicsSystem::Update(float deltaTime)
{
    if (m_paused || deltaTime <= 0.0f) {
        return;
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Note: In real implementation, would step Bullet Physics simulation:
    // if (m_dynamicsWorld) {
    //     m_dynamicsWorld->stepSimulation(deltaTime, m_maxSubsteps, m_timeStep);
    // }
    
    // Update metrics
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    {
        std::lock_guard<std::mutex> lock(m_metricsMutex);
        m_metrics.simulationTime = duration.count() / 1000.0f; // Convert to milliseconds
        m_metrics.timeStep = deltaTime;
        m_metrics.totalRigidBodies = static_cast<uint32_t>(m_bodies.size());
        m_metrics.activeRigidBodies = static_cast<uint32_t>(m_bodies.size()); // Simplified
        m_metrics.activeConstraints = static_cast<uint32_t>(m_constraints.size());
    }
}

void PhysicsSystem::SetGravity(const XMFLOAT3& gravity)
{
    // Note: In real implementation:
    // if (m_dynamicsWorld) {
    //     m_dynamicsWorld->setGravity(ToBullet(gravity));
    // }
}

XMFLOAT3 PhysicsSystem::GetGravity() const
{
    // Note: In real implementation would get from Bullet Physics world
    return {0.0f, -9.8f, 0.0f}; // Default gravity
}

std::shared_ptr<PhysicsBody> PhysicsSystem::CreateBody(const PhysicsBodyDesc& desc)
{
    // Note: In real implementation, would create Bullet Physics rigid body
    btRigidBody* bulletBody = nullptr; // Placeholder
    
    auto body = std::make_shared<PhysicsBody>(desc, bulletBody);
    m_bodies.push_back(body);
    
    if (!desc.name.empty()) {
        m_namedBodies[desc.name] = body;
    }
    
    Spark::SimpleConsole::GetInstance().LogInfo("Created physics body: " + desc.name);
    return body;
}

void PhysicsSystem::RemoveBody(std::shared_ptr<PhysicsBody> body)
{
    if (!body) return;
    
    // Remove from named bodies
    const std::string& name = body->GetName();
    if (!name.empty()) {
        m_namedBodies.erase(name);
    }
    
    // Remove from bodies list
    auto it = std::find(m_bodies.begin(), m_bodies.end(), body);
    if (it != m_bodies.end()) {
        m_bodies.erase(it);
    }
    
    // Note: In real implementation, would remove from Bullet Physics world
}

void PhysicsSystem::RemoveAllBodies()
{
    m_bodies.clear();
    m_namedBodies.clear();
    
    // Note: In real implementation, would remove all from Bullet Physics world
}

std::shared_ptr<PhysicsConstraint> PhysicsSystem::CreateHingeConstraint(
    std::shared_ptr<PhysicsBody> bodyA, std::shared_ptr<PhysicsBody> bodyB,
    const XMFLOAT3& pivotA, const XMFLOAT3& pivotB,
    const XMFLOAT3& axisA, const XMFLOAT3& axisB)
{
    // Note: In real implementation, would create Bullet Physics hinge constraint
    btTypedConstraint* bulletConstraint = nullptr; // Placeholder
    
    auto constraint = std::make_shared<PhysicsConstraint>(ConstraintType::Hinge, bulletConstraint);
    m_constraints.push_back(constraint);
    
    return constraint;
}

std::shared_ptr<PhysicsConstraint> PhysicsSystem::CreateSliderConstraint(
    std::shared_ptr<PhysicsBody> bodyA, std::shared_ptr<PhysicsBody> bodyB,
    const XMMATRIX& frameA, const XMMATRIX& frameB)
{
    // Note: In real implementation, would create Bullet Physics slider constraint
    btTypedConstraint* bulletConstraint = nullptr; // Placeholder
    
    auto constraint = std::make_shared<PhysicsConstraint>(ConstraintType::Slider, bulletConstraint);
    m_constraints.push_back(constraint);
    
    return constraint;
}

std::shared_ptr<PhysicsConstraint> PhysicsSystem::CreateFixedConstraint(
    std::shared_ptr<PhysicsBody> bodyA, std::shared_ptr<PhysicsBody> bodyB,
    const XMMATRIX& frameA, const XMMATRIX& frameB)
{
    // Note: In real implementation, would create Bullet Physics fixed constraint
    btTypedConstraint* bulletConstraint = nullptr; // Placeholder
    
    auto constraint = std::make_shared<PhysicsConstraint>(ConstraintType::Fixed, bulletConstraint);
    m_constraints.push_back(constraint);
    
    return constraint;
}

void PhysicsSystem::RemoveConstraint(std::shared_ptr<PhysicsConstraint> constraint)
{
    if (!constraint) return;
    
    auto it = std::find(m_constraints.begin(), m_constraints.end(), constraint);
    if (it != m_constraints.end()) {
        m_constraints.erase(it);
    }
    
    // Note: In real implementation, would remove from Bullet Physics world
}

RaycastHit PhysicsSystem::Raycast(const XMFLOAT3& origin, const XMFLOAT3& direction, float maxDistance)
{
    RaycastHit hit;
    hit.hasHit = false;
    
    // Note: In real implementation, would perform Bullet Physics raycast
    // For now, return no hit
    
    std::lock_guard<std::mutex> lock(m_metricsMutex);
    m_metrics.raycastCount++;
    
    return hit;
}

std::vector<RaycastHit> PhysicsSystem::RaycastAll(const XMFLOAT3& origin, const XMFLOAT3& direction, float maxDistance)
{
    std::vector<RaycastHit> hits;
    
    // Note: In real implementation, would perform Bullet Physics raycast all
    
    std::lock_guard<std::mutex> lock(m_metricsMutex);
    m_metrics.raycastCount++;
    
    return hits;
}

bool PhysicsSystem::SphereOverlap(const XMFLOAT3& center, float radius, std::vector<PhysicsBody*>& results)
{
    results.clear();
    
    // Note: In real implementation, would perform Bullet Physics sphere overlap test
    
    return !results.empty();
}

bool PhysicsSystem::BoxOverlap(const XMFLOAT3& center, const XMFLOAT3& halfExtents, std::vector<PhysicsBody*>& results)
{
    results.clear();
    
    // Note: In real implementation, would perform Bullet Physics box overlap test
    
    return !results.empty();
}

void PhysicsSystem::Console_EnableDebugDraw(bool enabled)
{
    EnableDebugDraw(enabled);
    Spark::SimpleConsole::GetInstance().LogSuccess("Physics debug draw " + std::string(enabled ? "enabled" : "disabled"));
}

void PhysicsSystem::Console_PausePhysics(bool paused)
{
    m_paused = paused;
    Spark::SimpleConsole::GetInstance().LogSuccess("Physics simulation " + std::string(paused ? "paused" : "resumed"));
}

void PhysicsSystem::Console_SetTimeStep(float timeStep)
{
    SetTimeStep(timeStep);
    Spark::SimpleConsole::GetInstance().LogSuccess("Physics time step set to: " + std::to_string(timeStep));
}

std::string PhysicsSystem::Console_Raycast(float originX, float originY, float originZ, 
                                          float dirX, float dirY, float dirZ, float maxDistance)
{
    XMFLOAT3 origin = {originX, originY, originZ};
    XMFLOAT3 direction = {dirX, dirY, dirZ};
    
    // Normalize direction
    XMVECTOR dirVector = XMLoadFloat3(&direction);
    dirVector = XMVector3Normalize(dirVector);
    XMStoreFloat3(&direction, dirVector);
    
    RaycastHit hit = Raycast(origin, direction, maxDistance);
    
    std::stringstream ss;
    if (hit.hasHit) {
        ss << "Raycast HIT:\n";
        ss << "Hit Point: (" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ")\n";
        ss << "Hit Normal: (" << hit.normal.x << ", " << hit.normal.y << ", " << hit.normal.z << ")\n";
        ss << "Distance: " << hit.distance << "\n";
        if (hit.body) {
            ss << "Hit Body: " << hit.body->GetName() << "\n";
        }
    } else {
        ss << "Raycast MISS - No objects hit";
    }
    
    return ss.str();
}

void PhysicsSystem::Console_Reset()
{
    RemoveAllBodies();
    m_constraints.clear();
    SetGravity({0.0f, -9.8f, 0.0f});
    m_paused = false;
    
    Spark::SimpleConsole::GetInstance().LogSuccess("Physics system reset complete");
}

// Console integration methods
PhysicsSystem::PhysicsMetrics PhysicsSystem::Console_GetMetrics() const
{
    std::lock_guard<std::mutex> lock(m_metricsMutex);
    return m_metrics;
}

std::string PhysicsSystem::Console_ListBodies() const
{
    std::stringstream ss;
    ss << "=== Physics Bodies (" << m_bodies.size() << ") ===\n";
    
    for (const auto& body : m_bodies) {
        if (body) {
            ss << body->GetName() << " - " << PhysicsBodyTypeToString(body->GetType());
            auto pos = body->GetPosition();
            ss << " at (" << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
        }
    }
    
    return ss.str();
}

std::string PhysicsSystem::Console_GetBodyInfo(const std::string& name) const
{
    auto it = m_namedBodies.find(name);
    if (it != m_namedBodies.end() && it->second) {
        return it->second->GetInfo();
    }
    
    return "Physics body not found: " + name;
}

bool PhysicsSystem::Console_CreateBody(const std::string& name, const std::string& type, float x, float y, float z)
{
    PhysicsBodyDesc desc;
    desc.name = name;
    desc.position = {x, y, z};
    desc.type = StringToPhysicsBodyType(type);
    desc.shape.type = CollisionShapeType::Box; // Default to box
    desc.mass = (desc.type == PhysicsBodyType::Static) ? 0.0f : 1.0f;
    
    auto body = CreateBody(desc);
    return body != nullptr;
}

bool PhysicsSystem::Console_RemoveBody(const std::string& name)
{
    auto it = m_namedBodies.find(name);
    if (it != m_namedBodies.end()) {
        RemoveBody(it->second);
        return true;
    }
    
    return false;
}

void PhysicsSystem::Console_SetGravity(float x, float y, float z)
{
    SetGravity({x, y, z});
    Spark::SimpleConsole::GetInstance().LogSuccess("Gravity set to (" + 
        std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
}

void PhysicsSystem::Console_SetBodyProperty(const std::string& name, const std::string& property, float value)
{
    auto it = m_namedBodies.find(name);
    if (it != m_namedBodies.end() && it->second) {
        it->second->Console_SetProperty(property, value);
        Spark::SimpleConsole::GetInstance().LogSuccess("Set " + property + " = " + std::to_string(value) + " for " + name);
    } else {
        Spark::SimpleConsole::GetInstance().LogError("Physics body not found: " + name);
    }
}

void PhysicsSystem::Console_ApplyForce(const std::string& name, float x, float y, float z)
{
    auto it = m_namedBodies.find(name);
    if (it != m_namedBodies.end() && it->second) {
        it->second->ApplyForce({x, y, z});
        Spark::SimpleConsole::GetInstance().LogSuccess("Applied force (" + 
            std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ") to " + name);
    } else {
        Spark::SimpleConsole::GetInstance().LogError("Physics body not found: " + name);
    }
}

void PhysicsSystem::Console_ApplyImpulse(const std::string& name, float x, float y, float z)
{
    auto it = m_namedBodies.find(name);
    if (it != m_namedBodies.end() && it->second) {
        it->second->ApplyImpulse({x, y, z});
        Spark::SimpleConsole::GetInstance().LogSuccess("Applied impulse (" + 
            std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ") to " + name);
    } else {
        Spark::SimpleConsole::GetInstance().LogError("Physics body not found: " + name);
    }
}

void PhysicsSystem::RegisterMaterial(const std::string& name, const PhysicsMaterial& material)
{
    m_materials[name] = material;
}

const PhysicsMaterial* PhysicsSystem::GetMaterial(const std::string& name) const
{
    auto it = m_materials.find(name);
    return (it != m_materials.end()) ? &it->second : nullptr;
}

void PhysicsSystem::RenderDebug()
{
    // Note: In real implementation would render Bullet debug visualization
}

// ============================================================================
// UTILITY FUNCTIONS IMPLEMENTATION - MISSING GLOBAL FUNCTIONS
// ============================================================================

std::string PhysicsBodyTypeToString(PhysicsBodyType type)
{
    switch (type) {
        case PhysicsBodyType::Static: return "Static";
        case PhysicsBodyType::Kinematic: return "Kinematic";
        case PhysicsBodyType::Dynamic: return "Dynamic";
        default: return "Unknown";
    }
}

PhysicsBodyType StringToPhysicsBodyType(const std::string& str)
{
    if (str == "Static" || str == "static") return PhysicsBodyType::Static;
    if (str == "Kinematic" || str == "kinematic") return PhysicsBodyType::Kinematic;
    if (str == "Dynamic" || str == "dynamic") return PhysicsBodyType::Dynamic;
    return PhysicsBodyType::Dynamic; // Default
}

std::string CollisionShapeTypeToString(CollisionShapeType type)
{
    switch (type) {
        case CollisionShapeType::Box: return "Box";
        case CollisionShapeType::Sphere: return "Sphere";
        case CollisionShapeType::Capsule: return "Capsule";
        case CollisionShapeType::Cylinder: return "Cylinder";
        case CollisionShapeType::Cone: return "Cone";
        case CollisionShapeType::Mesh: return "Mesh";
        case CollisionShapeType::ConvexHull: return "ConvexHull";
        case CollisionShapeType::Heightfield: return "Heightfield";
        case CollisionShapeType::Compound: return "Compound";
        default: return "Unknown";
    }
}

CollisionShapeType StringToCollisionShapeType(const std::string& str)
{
    if (str == "Box" || str == "box") return CollisionShapeType::Box;
    if (str == "Sphere" || str == "sphere") return CollisionShapeType::Sphere;
    if (str == "Capsule" || str == "capsule") return CollisionShapeType::Capsule;
    if (str == "Cylinder" || str == "cylinder") return CollisionShapeType::Cylinder;
    if (str == "Cone" || str == "cone") return CollisionShapeType::Cone;
    if (str == "Mesh" || str == "mesh") return CollisionShapeType::Mesh;
    if (str == "ConvexHull" || str == "convexhull") return CollisionShapeType::ConvexHull;
    if (str == "Heightfield" || str == "heightfield") return CollisionShapeType::Heightfield;
    if (str == "Compound" || str == "compound") return CollisionShapeType::Compound;
    return CollisionShapeType::Box; // Default
}

std::string ConstraintTypeToString(ConstraintType type)
{
    switch (type) {
        case ConstraintType::Point2Point: return "Point2Point";
        case ConstraintType::Hinge: return "Hinge";
        case ConstraintType::Slider: return "Slider";
        case ConstraintType::ConeTwist: return "ConeTwist";
        case ConstraintType::Generic6DOF: return "Generic6DOF";
        case ConstraintType::Fixed: return "Fixed";
        default: return "Unknown";
    }
}

ConstraintType StringToConstraintType(const std::string& str)
{
    if (str == "Point2Point" || str == "point2point") return ConstraintType::Point2Point;
    if (str == "Hinge" || str == "hinge") return ConstraintType::Hinge;
    if (str == "Slider" || str == "slider") return ConstraintType::Slider;
    if (str == "ConeTwist" || str == "conetwist") return ConstraintType::ConeTwist;
    if (str == "Generic6DOF" || str == "generic6dof") return ConstraintType::Generic6DOF;
    if (str == "Fixed" || str == "fixed") return ConstraintType::Fixed;
    return ConstraintType::Fixed; // Default
}