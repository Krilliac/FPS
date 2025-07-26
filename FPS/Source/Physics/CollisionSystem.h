#pragma once
#include "..\Core\framework.h"
#include <vector>

struct BoundingBox
{
    XMFLOAT3 Min;
    XMFLOAT3 Max;
    
    BoundingBox() : Min(-1.0f, -1.0f, -1.0f), Max(1.0f, 1.0f, 1.0f) {}
    BoundingBox(const XMFLOAT3& min, const XMFLOAT3& max) : Min(min), Max(max) {}
    
    XMFLOAT3 GetCenter() const;
    XMFLOAT3 GetExtents() const;
    void Transform(const XMMATRIX& transform);
};

struct BoundingSphere
{
    XMFLOAT3 Center;
    float Radius;
    
    BoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(1.0f) {}
    BoundingSphere(const XMFLOAT3& center, float radius) : Center(center), Radius(radius) {}
    
    void Transform(const XMMATRIX& transform);
};

struct Ray
{
    XMFLOAT3 Origin;
    XMFLOAT3 Direction;
    
    Ray() : Origin(0.0f, 0.0f, 0.0f), Direction(0.0f, 0.0f, 1.0f) {}
    Ray(const XMFLOAT3& origin, const XMFLOAT3& direction) : Origin(origin), Direction(direction) {}
    
    XMFLOAT3 GetPoint(float t) const;
};

struct CollisionResult
{
    bool Hit;
    XMFLOAT3 Point;
    XMFLOAT3 Normal;
    float Distance;
    void* UserData;
    
    CollisionResult() : Hit(false), Point(0.0f, 0.0f, 0.0f), Normal(0.0f, 1.0f, 0.0f), 
                       Distance(0.0f), UserData(nullptr) {}
};

struct ContactManifold
{
    XMFLOAT3 ContactPoints[4];
    XMFLOAT3 Normal;
    float PenetrationDepth;
    int ContactCount;

    ContactManifold() : Normal(0.0f, 1.0f, 0.0f), PenetrationDepth(0.0f), ContactCount(0)  
    {  
        for (int i = 0; i < 4; ++i)  
        {  
            ContactPoints[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);  
        }  
    }
};

class CollisionSystem
{
public:
    // Sphere collision tests
    static bool SphereVsSphere(const BoundingSphere& sphere1, const BoundingSphere& sphere2);
    static bool SphereVsSphere(const BoundingSphere& sphere1, const BoundingSphere& sphere2, ContactManifold& manifold);
    static bool SphereVsBox(const BoundingSphere& sphere, const BoundingBox& box);
    
    // Box collision tests
    static bool BoxVsBox(const BoundingBox& box1, const BoundingBox& box2);
    
    // Ray collision tests
    static CollisionResult RayVsSphere(const Ray& ray, const BoundingSphere& sphere);
    static CollisionResult RayVsBox(const Ray& ray, const BoundingBox& box);
    static CollisionResult RayVsPlane(const Ray& ray, const XMFLOAT3& planePoint, const XMFLOAT3& planeNormal);
    static CollisionResult RayVsTriangle(const Ray& ray, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2);
    
    // Swept collision tests (continuous collision detection)
    static CollisionResult SweptSphereVsSphere(const BoundingSphere& sphere1, const XMFLOAT3& velocity1,
                                              const BoundingSphere& sphere2, const XMFLOAT3& velocity2, float deltaTime);
    static CollisionResult SweptSphereVsBox(const BoundingSphere& sphere, const XMFLOAT3& velocity,
                                           const BoundingBox& box, float deltaTime);
    
    // Utility functions
    static XMFLOAT3 ClosestPointOnBox(const XMFLOAT3& point, const BoundingBox& box);
    static XMFLOAT3 ClosestPointOnSphere(const XMFLOAT3& point, const BoundingSphere& sphere);
    static XMFLOAT3 ClosestPointOnPlane(const XMFLOAT3& point, const XMFLOAT3& planePoint, const XMFLOAT3& planeNormal);
    static float DistancePointToPlane(const XMFLOAT3& point, const XMFLOAT3& planePoint, const XMFLOAT3& planeNormal);
    
    // Intersection tests
    static bool PointInSphere(const XMFLOAT3& point, const BoundingSphere& sphere);
    static bool PointInBox(const XMFLOAT3& point, const BoundingBox& box);
    static bool SphereInFrustum(const BoundingSphere& sphere, const XMMATRIX& viewProjection);
    static bool BoxInFrustum(const BoundingBox& box, const XMMATRIX& viewProjection);
    
    // Helper functions for vector math
    static float Vector3Length(const XMFLOAT3& v);
    static float Vector3LengthSquared(const XMFLOAT3& v);
    static XMFLOAT3 Vector3Normalize(const XMFLOAT3& v);
    static float Vector3Dot(const XMFLOAT3& v1, const XMFLOAT3& v2);
    static XMFLOAT3 Vector3Cross(const XMFLOAT3& v1, const XMFLOAT3& v2);
    static XMFLOAT3 Vector3Reflect(const XMFLOAT3& incident, const XMFLOAT3& normal);
    static XMFLOAT3 Vector3Lerp(const XMFLOAT3& a, const XMFLOAT3& b, float t);
};
