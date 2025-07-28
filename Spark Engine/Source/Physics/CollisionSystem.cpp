#include "CollisionSystem.h"
#include <algorithm>

// BoundingBox methods
XMFLOAT3 BoundingBox::GetCenter() const
{
    return XMFLOAT3(
        (Min.x + Max.x) * 0.5f,
        (Min.y + Max.y) * 0.5f,
        (Min.z + Max.z) * 0.5f
    );
}

XMFLOAT3 BoundingBox::GetExtents() const
{
    return XMFLOAT3(
        (Max.x - Min.x) * 0.5f,
        (Max.y - Min.y) * 0.5f,
        (Max.z - Min.z) * 0.5f
    );
}

void BoundingBox::Transform(const XMMATRIX& transform)
{
    XMFLOAT3 corners[8] = {
        XMFLOAT3(Min.x, Min.y, Min.z),
        XMFLOAT3(Max.x, Min.y, Min.z),
        XMFLOAT3(Min.x, Max.y, Min.z),
        XMFLOAT3(Max.x, Max.y, Min.z),
        XMFLOAT3(Min.x, Min.y, Max.z),
        XMFLOAT3(Max.x, Min.y, Max.z),
        XMFLOAT3(Min.x, Max.y, Max.z),
        XMFLOAT3(Max.x, Max.y, Max.z)
    };
    
    Min = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
    Max = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    
    for (int i = 0; i < 8; ++i)
    {
        XMVECTOR corner = XMLoadFloat3(&corners[i]);
        corner = XMVector3Transform(corner, transform);
        XMFLOAT3 transformedCorner;
        XMStoreFloat3(&transformedCorner, corner);        
        Min.x = (std::min)(Min.x, transformedCorner.x);
        Min.y = (std::min)(Min.y, transformedCorner.y);
        Min.z = (std::min)(Min.z, transformedCorner.z);        
        Max.x = (std::max)(Max.x, transformedCorner.x);
        Max.y = (std::max)(Max.y, transformedCorner.y);
        Max.z = (std::max)(Max.z, transformedCorner.z);
    }
}

// BoundingSphere methods
void BoundingSphere::Transform(const XMMATRIX& transform)
{
    XMVECTOR center = XMLoadFloat3(&Center);
    center = XMVector3Transform(center, transform);
    XMStoreFloat3(&Center, center);
    
    // Scale radius by maximum scale factor
    XMVECTOR scale = XMVector3Length(transform.r[0]);
    float scaleValue;
    XMStoreFloat(&scaleValue, scale);
    Radius *= scaleValue;
}

// Ray methods
XMFLOAT3 Ray::GetPoint(float t) const
{
    return XMFLOAT3(
        Origin.x + Direction.x * t,
        Origin.y + Direction.y * t,
        Origin.z + Direction.z * t
    );
}

// CollisionSystem implementation
bool CollisionSystem::SphereVsSphere(const BoundingSphere& sphere1, const BoundingSphere& sphere2)
{
    XMVECTOR center1 = XMLoadFloat3(&sphere1.Center);
    XMVECTOR center2 = XMLoadFloat3(&sphere2.Center);
    XMVECTOR distance = XMVector3Length(center1 - center2);
    
    float dist;
    XMStoreFloat(&dist, distance);
    
    return dist <= (sphere1.Radius + sphere2.Radius);
}

bool CollisionSystem::SphereVsSphere(const BoundingSphere& sphere1, const BoundingSphere& sphere2, ContactManifold& manifold)
{
    XMVECTOR center1 = XMLoadFloat3(&sphere1.Center);
    XMVECTOR center2 = XMLoadFloat3(&sphere2.Center);
    XMVECTOR delta = center2 - center1;
    XMVECTOR distance = XMVector3Length(delta);
    
    float dist;
    XMStoreFloat(&dist, distance);
    float totalRadius = sphere1.Radius + sphere2.Radius;
    
    if (dist <= totalRadius)
    {
        manifold.ContactCount = 1;
        manifold.PenetrationDepth = totalRadius - dist;
        
        if (dist > 0.0001f)
        {
            XMVECTOR normal = delta / distance;
            XMStoreFloat3(&manifold.Normal, normal);
            
            XMVECTOR contactPoint = center1 + normal * sphere1.Radius;
            XMStoreFloat3(&manifold.ContactPoints[0], contactPoint);
        }
        else
        {
            // Spheres are at same position
            manifold.Normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
            manifold.ContactPoints[0] = sphere1.Center;
        }
        
        return true;
    }
    
    return false;
}

bool CollisionSystem::SphereVsBox(const BoundingSphere& sphere, const BoundingBox& box)
{
    XMFLOAT3 closestPoint = ClosestPointOnBox(sphere.Center, box);
    
    XMVECTOR sphereCenter = XMLoadFloat3(&sphere.Center);
    XMVECTOR closest = XMLoadFloat3(&closestPoint);
    XMVECTOR distance = XMVector3Length(sphereCenter - closest);
    
    float dist;
    XMStoreFloat(&dist, distance);
    
    return dist <= sphere.Radius;
}

bool CollisionSystem::BoxVsBox(const BoundingBox& box1, const BoundingBox& box2)
{
    return (box1.Min.x <= box2.Max.x && box1.Max.x >= box2.Min.x) &&
           (box1.Min.y <= box2.Max.y && box1.Max.y >= box2.Min.y) &&
           (box1.Min.z <= box2.Max.z && box1.Max.z >= box2.Min.z);
}

CollisionResult CollisionSystem::RayVsSphere(const Ray& ray, const BoundingSphere& sphere)
{
    CollisionResult result;

    // Load ray origin and direction
    XMVECTOR rayOrigin = XMLoadFloat3(&ray.Origin);
    XMVECTOR rayDirection = XMVector3Normalize(XMLoadFloat3(&ray.Direction));
    XMVECTOR sphereCenter = XMLoadFloat3(&sphere.Center);

    // Compute vector from sphere center to ray origin
    XMVECTOR ocVec = rayOrigin - sphereCenter;

    // Quadratic coefficients
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;

    // a = dot(D, D)
    XMStoreFloat(&a, XMVector3Dot(rayDirection, rayDirection));

    // b = 2 * dot(oc, D)
    float ocDotDir = 0.0f;
    XMStoreFloat(&ocDotDir, XMVector3Dot(ocVec, rayDirection));
    b = 2.0f * ocDotDir;

    // c = dot(oc, oc) - radius^2
    float ocDotOc = 0.0f;
    XMStoreFloat(&ocDotOc, XMVector3Dot(ocVec, ocVec));
    c = ocDotOc - (sphere.Radius * sphere.Radius);

    // Compute discriminant
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
    {
        result.Hit = false;
        return result;
    }

    // Compute nearest intersection t
    float sqrtDisc = sqrtf(discriminant);
    float t0 = (-b - sqrtDisc) / (2.0f * a);
    float t1 = (-b + sqrtDisc) / (2.0f * a);
    float t = t0 > 0.0f ? t0 : t1;
    if (t < 0.0f)
    {
        result.Hit = false;
        return result;
    }

    // Fill result
    result.Hit = true;
    result.Distance = t;

    // Compute hit point
    XMVECTOR hitPointVec = rayOrigin + rayDirection * t;
    XMStoreFloat3(&result.Point, hitPointVec);

    // Compute normal at hit point
    XMVECTOR normalVec = XMVector3Normalize(hitPointVec - sphereCenter);
    XMStoreFloat3(&result.Normal, normalVec);

    return result;
}

CollisionResult CollisionSystem::RayVsBox(const Ray& ray, const BoundingBox& box)
{
    CollisionResult result;
    
    XMVECTOR rayOrigin = XMLoadFloat3(&ray.Origin);
    XMVECTOR rayDirection = XMLoadFloat3(&ray.Direction);
    XMVECTOR boxMin = XMLoadFloat3(&box.Min);
    XMVECTOR boxMax = XMLoadFloat3(&box.Max);
    
    XMVECTOR invDirection = XMVectorReciprocal(rayDirection);
    
    XMVECTOR t1 = (boxMin - rayOrigin) * invDirection;
    XMVECTOR t2 = (boxMax - rayOrigin) * invDirection;
    
    XMVECTOR tmin = XMVectorMin(t1, t2);
    XMVECTOR tmax = XMVectorMax(t1, t2);
    
    float tminVal = XMVectorGetX(XMVectorMax(XMVectorMax(tmin, XMVectorSplatY(tmin)), XMVectorSplatZ(tmin)));
    float tmaxVal = XMVectorGetX(XMVectorMin(XMVectorMin(tmax, XMVectorSplatY(tmax)), XMVectorSplatZ(tmax)));
    
    if (tminVal > tmaxVal || tmaxVal < 0)
    {
        result.Hit = false;
        return result;
    }
    
    float t = (tminVal < 0) ? tmaxVal : tminVal;
    
    result.Hit = true;
    result.Distance = t;
    
    XMVECTOR hitPoint = rayOrigin + rayDirection * t;
    XMStoreFloat3(&result.Point, hitPoint);
    
    // Calculate normal (simplified)
    XMFLOAT3 center = box.GetCenter();
    XMVECTOR normal = XMVector3Normalize(hitPoint - XMLoadFloat3(&center));
    XMStoreFloat3(&result.Normal, normal);
    
    return result;
}

CollisionResult CollisionSystem::RayVsPlane(const Ray& ray, const XMFLOAT3& planePoint, const XMFLOAT3& planeNormal)
{
    CollisionResult result;
    
    XMVECTOR rayOrigin = XMLoadFloat3(&ray.Origin);
    XMVECTOR rayDirection = XMVector3Normalize(XMLoadFloat3(&ray.Direction));
    XMVECTOR planePos = XMLoadFloat3(&planePoint);
    XMVECTOR planeNorm = XMVector3Normalize(XMLoadFloat3(&planeNormal));
    
    float denom;
    XMStoreFloat(&denom, XMVector3Dot(rayDirection, planeNorm));
    
    if (abs(denom) < 0.0001f)
    {
        result.Hit = false;
        return result;
    }
    
    float num;
    XMStoreFloat(&num, XMVector3Dot(planePos - rayOrigin, planeNorm));
    
    float t = num / denom;
    
    if (t < 0)
    {
        result.Hit = false;
        return result;
    }
    
    result.Hit = true;
    result.Distance = t;
    XMStoreFloat3(&result.Normal, planeNorm);
    
    XMVECTOR hitPoint = rayOrigin + rayDirection * t;
    XMStoreFloat3(&result.Point, hitPoint);
    
    return result;
}

CollisionResult CollisionSystem::RayVsTriangle(const Ray& ray, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2)  
{  
    CollisionResult result;

    // Load triangle vertices into XMVECTOR  
    XMVECTOR p0 = XMLoadFloat3(&v0);  
    XMVECTOR p1 = XMLoadFloat3(&v1);  
    XMVECTOR p2 = XMLoadFloat3(&v2);  

    // Compute triangle edges  
    XMVECTOR edge1 = XMVectorSubtract(p1, p0);  
    XMVECTOR edge2 = XMVectorSubtract(p2, p0);  

    // Compute the determinant  
    XMVECTOR h = XMVector3Cross(XMLoadFloat3(&ray.Direction), edge2);  
    float a = XMVectorGetX(XMVector3Dot(edge1, h));  

    // If the determinant is near zero, the ray lies in the plane of the triangle  
    if (fabs(a) < 1e-6f)  
    {  
        result.Hit = false;  
        return result;  
    }  

    float f = 1.0f / a;  
    XMVECTOR s = XMVectorSubtract(XMLoadFloat3(&ray.Origin), p0);  
    float u = f * XMVectorGetX(XMVector3Dot(s, h));  

    // Check if the intersection lies outside the triangle  
    if (u < 0.0f || u > 1.0f)  
    {  
        result.Hit = false;  
        return result;  
    }  

    XMVECTOR q = XMVector3Cross(s, edge1);  
    float v = f * XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ray.Direction), q));  

    // Check if the intersection lies outside the triangle  
    if (v < 0.0f || u + v > 1.0f)  
    {  
        result.Hit = false;  
        return result;  
    }  

    // Compute the distance along the ray to the intersection  
    float t = f * XMVectorGetX(XMVector3Dot(edge2, q));  

    if (t > 1e-6f) // Ray intersection  
    {  
        result.Hit = true;  
        result.Distance = t;  
        XMVECTOR intersectionPoint = XMVectorAdd(XMLoadFloat3(&ray.Origin), XMVectorScale(XMLoadFloat3(&ray.Direction), t));  
        XMStoreFloat3(&result.Point, intersectionPoint);  

        // Compute the normal of the triangle  
        XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge1, edge2));  
        XMStoreFloat3(&result.Normal, normal);  
    }  
    else // This means that there is a line intersection but not a ray intersection  
    {  
        result.Hit = false;  
    }  

    return result;  
}

XMFLOAT3 CollisionSystem::ClosestPointOnBox(const XMFLOAT3& point, const BoundingBox& box)
{
    XMFLOAT3 result{};
    
    result.x = (std::max)(box.Min.x, (std::min)(point.x, box.Max.x));
    result.y = (std::max)(box.Min.y, (std::min)(point.y, box.Max.y));
    result.z = (std::max)(box.Min.z, (std::min)(point.z, box.Max.z));
    
    return result;
}

XMFLOAT3 CollisionSystem::ClosestPointOnSphere(const XMFLOAT3& point, const BoundingSphere& sphere)
{
    XMVECTOR sphereCenter = XMLoadFloat3(&sphere.Center);
    XMVECTOR pointVec = XMLoadFloat3(&point);
    XMVECTOR direction = XMVector3Normalize(pointVec - sphereCenter);
    XMVECTOR closestPoint = sphereCenter + direction * sphere.Radius;
    
    XMFLOAT3 result;
    XMStoreFloat3(&result, closestPoint);
    return result;
}

float CollisionSystem::DistancePointToPlane(const XMFLOAT3& point, const XMFLOAT3& planePoint, const XMFLOAT3& planeNormal)
{
    XMVECTOR p = XMLoadFloat3(&point);
    XMVECTOR planeP = XMLoadFloat3(&planePoint);
    XMVECTOR planeN = XMVector3Normalize(XMLoadFloat3(&planeNormal));
    
    float distance;
    XMStoreFloat(&distance, XMVector3Dot(p - planeP, planeN));
    
    return distance;
}

bool CollisionSystem::PointInSphere(const XMFLOAT3& point, const BoundingSphere& sphere)
{
    return Vector3LengthSquared(XMFLOAT3(
        point.x - sphere.Center.x,
        point.y - sphere.Center.y,
        point.z - sphere.Center.z
    )) <= (sphere.Radius * sphere.Radius);
}

bool CollisionSystem::PointInBox(const XMFLOAT3& point, const BoundingBox& box)
{
    return (point.x >= box.Min.x && point.x <= box.Max.x) &&
           (point.y >= box.Min.y && point.y <= box.Max.y) &&
           (point.z >= box.Min.z && point.z <= box.Max.z);
}

float CollisionSystem::Vector3Length(const XMFLOAT3& v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float CollisionSystem::Vector3LengthSquared(const XMFLOAT3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

XMFLOAT3 CollisionSystem::Vector3Normalize(const XMFLOAT3& v)
{
    float length = Vector3Length(v);
    if (length > 0.0f)
    {
        return XMFLOAT3(v.x / length, v.y / length, v.z / length);
    }
    return XMFLOAT3(0.0f, 0.0f, 0.0f);
}

float CollisionSystem::Vector3Dot(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

XMFLOAT3 CollisionSystem::Vector3Cross(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
    return XMFLOAT3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

XMFLOAT3 CollisionSystem::Vector3Reflect(const XMFLOAT3& incident, const XMFLOAT3& normal)
{
    float dot = Vector3Dot(incident, normal);
    return XMFLOAT3(
        incident.x - 2.0f * dot * normal.x,
        incident.y - 2.0f * dot * normal.y,
        incident.z - 2.0f * dot * normal.z
    );
}

XMFLOAT3 CollisionSystem::Vector3Lerp(const XMFLOAT3& a, const XMFLOAT3& b, float t)
{
    return XMFLOAT3(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    );
}

