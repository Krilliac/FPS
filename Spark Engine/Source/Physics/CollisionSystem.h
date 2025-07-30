// CollisionSystem.h
#pragma once

#include "Utils/Assert.h"
#include "..\Core\framework.h"
#include <DirectXMath.h>
#include <vector>
#include <cfloat>
#include <cmath>

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

// Bounding box
struct BoundingBox
{
    XMFLOAT3 Min;
    XMFLOAT3 Max;

    BoundingBox() : Min(-1, -1, -1), Max(1, 1, 1) {}
    BoundingBox(const XMFLOAT3& min, const XMFLOAT3& max) : Min(min), Max(max) {}

    XMFLOAT3 GetCenter()  const;
    XMFLOAT3 GetExtents() const;
    void      Transform(const XMMATRIX& transform);
};

// Bounding sphere
struct BoundingSphere
{
    XMFLOAT3 Center;
    float     Radius;

    BoundingSphere() : Center(0, 0, 0), Radius(1) {}
    BoundingSphere(const XMFLOAT3& c, float r) : Center(c), Radius(r) {}

    void Transform(const XMMATRIX& transform);
};

// Ray
struct Ray
{
    XMFLOAT3 Origin;
    XMFLOAT3 Direction;

    Ray() : Origin(0, 0, 0), Direction(0, 0, 1) {}
    Ray(const XMFLOAT3& o, const XMFLOAT3& d) : Origin(o), Direction(d) {}

    XMFLOAT3 GetPoint(float t) const;
};

// Contact manifold
struct ContactManifold
{
    XMFLOAT3 ContactPoints[4];
    XMFLOAT3 Normal;
    float     PenetrationDepth;
    int       ContactCount;

    ContactManifold()
        : Normal(0, 1, 0), PenetrationDepth(0), ContactCount(0)
    {
        for (int i = 0; i < 4; ++i)
            ContactPoints[i] = XMFLOAT3(0, 0, 0);
    }
};

// Raycast result
struct CollisionResult
{
    bool     Hit;
    XMFLOAT3 Point;
    XMFLOAT3 Normal;
    float    Distance;
    void* UserData;

    CollisionResult()
        : Hit(false), Point(0, 0, 0), Normal(0, 1, 0), Distance(0), UserData(nullptr) {
    }
};

class CollisionSystem
{
public:
    // Primitive tests
    static bool SphereVsSphere(const BoundingSphere& a, const BoundingSphere& b);
    static bool SphereVsSphere(const BoundingSphere& a, const BoundingSphere& b, ContactManifold& m);
    static bool SphereVsBox(const BoundingSphere& sphere, const BoundingBox& box);
    static bool BoxVsBox(const BoundingBox& a, const BoundingBox& b);

    // Ray tests
    static CollisionResult RayVsSphere(const Ray& ray, const BoundingSphere& sphere);
    static CollisionResult RayVsBox(const Ray& ray, const BoundingBox& box);
    static CollisionResult RayVsPlane(const Ray& ray, const XMFLOAT3& planePoint, const XMFLOAT3& planeNormal);
    static CollisionResult RayVsTriangle(const Ray& ray, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2);

    // Utility functions
    static XMFLOAT3 ClosestPointOnBox(const XMFLOAT3& point, const BoundingBox& box);
    static XMFLOAT3 ClosestPointOnSphere(const XMFLOAT3& point, const BoundingSphere& sphere);
    static float    DistancePointToPlane(const XMFLOAT3& point, const XMFLOAT3& planePoint, const XMFLOAT3& planeNormal);

    // Point tests
    static bool PointInSphere(const XMFLOAT3& point, const BoundingSphere& sphere);
    static bool PointInBox(const XMFLOAT3& point, const BoundingBox& box);

    // Vector helpers
    static float    Vector3Length(const XMFLOAT3& v);
    static float    Vector3LengthSquared(const XMFLOAT3& v);
    static XMFLOAT3 Vector3Normalize(const XMFLOAT3& v);
    static float    Vector3Dot(const XMFLOAT3& a, const XMFLOAT3& b);
    static XMFLOAT3 Vector3Cross(const XMFLOAT3& a, const XMFLOAT3& b);
    static XMFLOAT3 Vector3Reflect(const XMFLOAT3& i, const XMFLOAT3& n);
    static XMFLOAT3 Vector3Lerp(const XMFLOAT3& a, const XMFLOAT3& b, float t);
};