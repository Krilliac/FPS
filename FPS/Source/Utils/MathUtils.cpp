#include "MathUtils.h"
#include <random>

// Static constant definitions
const float MathUtils::PI = 3.14159265359f;
const float MathUtils::TWO_PI = 6.28318530718f;
const float MathUtils::HALF_PI = 1.57079632679f;
const float MathUtils::DEG_TO_RAD = 0.01745329252f;
const float MathUtils::RAD_TO_DEG = 57.2957795131f;

// Static random number generator
static std::random_device s_randomDevice;
static std::mt19937 s_randomGenerator(s_randomDevice());
static bool s_randomInitialized = false;

// =============================================================================
// ANGLE UTILITIES
// =============================================================================

float MathUtils::DegreesToRadians(float degrees)
{
    return degrees * DEG_TO_RAD;
}

float MathUtils::RadiansToDegrees(float radians)
{
    return radians * RAD_TO_DEG;
}

float MathUtils::WrapAngle(float angle)
{
    // Wrap angle to [-PI, PI] range
    while (angle > PI)
        angle -= TWO_PI;
    while (angle < -PI)
        angle += TWO_PI;
    return angle;
}

float MathUtils::NormalizeAngle(float angle)
{
    // Normalize angle to [0, 2*PI] range
    while (angle < 0.0f)
        angle += TWO_PI;
    while (angle >= TWO_PI)
        angle -= TWO_PI;
    return angle;
}

// =============================================================================
// INTERPOLATION FUNCTIONS
// =============================================================================

float MathUtils::Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

XMFLOAT3 MathUtils::Lerp(const XMFLOAT3& a, const XMFLOAT3& b, float t)
{
    return XMFLOAT3(
        Lerp(a.x, b.x, t),
        Lerp(a.y, b.y, t),
        Lerp(a.z, b.z, t)
    );
}

float MathUtils::SmoothStep(float a, float b, float t)
{
    // Clamp t to [0, 1]
    t = Clamp(t, 0.0f, 1.0f);
    
    // Apply smooth step curve
    t = t * t * (3.0f - 2.0f * t);
    
    return Lerp(a, b, t);
}

// =============================================================================
// DISTANCE CALCULATIONS
// =============================================================================

float MathUtils::Distance(const XMFLOAT3& a, const XMFLOAT3& b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

float MathUtils::DistanceSquared(const XMFLOAT3& a, const XMFLOAT3& b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;
    return dx * dx + dy * dy + dz * dz;
}

XMFLOAT3 MathUtils::Direction(const XMFLOAT3& from, const XMFLOAT3& to)
{
    XMFLOAT3 direction = Subtract(to, from);
    return Normalize(direction);
}

// =============================================================================
// RANDOM NUMBER GENERATION
// =============================================================================

void MathUtils::InitializeRandom()
{
    if (!s_randomInitialized)
    {
        s_randomGenerator.seed(s_randomDevice());
        s_randomInitialized = true;
    }
}

float MathUtils::RandomFloat(float min, float max)
{
    if (!s_randomInitialized)
        InitializeRandom();
        
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(s_randomGenerator);
}

int MathUtils::RandomInt(int min, int max)
{
    if (!s_randomInitialized)
        InitializeRandom();
        
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(s_randomGenerator);
}

XMFLOAT3 MathUtils::RandomDirection()
{
    // Generate random point on unit sphere using spherical coordinates
    float theta = RandomFloat(0.0f, TWO_PI);  // Azimuth angle
    float phi = RandomFloat(-HALF_PI, HALF_PI); // Elevation angle
    
    float cosTheta = cosf(theta);
    float sinTheta = sinf(theta);
    float cosPhi = cosf(phi);
    float sinPhi = sinf(phi);
    
    return XMFLOAT3(
        cosPhi * cosTheta,
        sinPhi,
        cosPhi * sinTheta
    );
}

XMFLOAT3 MathUtils::RandomPointInSphere(float radius)
{
    // Generate random point inside sphere using rejection sampling
    XMFLOAT3 point;
    float lengthSquared;
    
    do
    {
        point.x = RandomFloat(-1.0f, 1.0f);
        point.y = RandomFloat(-1.0f, 1.0f);
        point.z = RandomFloat(-1.0f, 1.0f);
        lengthSquared = LengthSquared(point);
    }
    while (lengthSquared > 1.0f);
    
    return Multiply(point, radius);
}

// =============================================================================
// CLAMPING FUNCTIONS
// =============================================================================

float MathUtils::Clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int MathUtils::Clamp(int value, int min, int max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

XMFLOAT3 MathUtils::Clamp(const XMFLOAT3& value, const XMFLOAT3& min, const XMFLOAT3& max)
{
    return XMFLOAT3(
        Clamp(value.x, min.x, max.x),
        Clamp(value.y, min.y, max.y),
        Clamp(value.z, min.z, max.z)
    );
}

// =============================================================================
// MATRIX UTILITIES
// =============================================================================

XMMATRIX MathUtils::CreateLookAt(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up)
{
    XMVECTOR eyeVec = XMLoadFloat3(&eye);
    XMVECTOR targetVec = XMLoadFloat3(&target);
    XMVECTOR upVec = XMLoadFloat3(&up);
    
    return XMMatrixLookAtLH(eyeVec, targetVec, upVec);
}

XMMATRIX MathUtils::CreatePerspective(float fovY, float aspectRatio, float nearPlane, float farPlane)
{
    return XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearPlane, farPlane);
}

XMMATRIX MathUtils::CreateOrthographic(float width, float height, float nearPlane, float farPlane)
{
    return XMMatrixOrthographicLH(width, height, nearPlane, farPlane);
}

// =============================================================================
// COLLISION UTILITIES
// =============================================================================

bool MathUtils::PointInSphere(const XMFLOAT3& point, const XMFLOAT3& sphereCenter, float sphereRadius)
{
    float distanceSquared = DistanceSquared(point, sphereCenter);
    return distanceSquared <= (sphereRadius * sphereRadius);
}

bool MathUtils::PointInBox(const XMFLOAT3& point, const XMFLOAT3& boxMin, const XMFLOAT3& boxMax)
{
    return (point.x >= boxMin.x && point.x <= boxMax.x) &&
           (point.y >= boxMin.y && point.y <= boxMax.y) &&
           (point.z >= boxMin.z && point.z <= boxMax.z);
}

// =============================================================================
// VECTOR OPERATIONS
// =============================================================================

XMFLOAT3 MathUtils::Add(const XMFLOAT3& a, const XMFLOAT3& b)
{
    return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

XMFLOAT3 MathUtils::Subtract(const XMFLOAT3& a, const XMFLOAT3& b)
{
    return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}

XMFLOAT3 MathUtils::Multiply(const XMFLOAT3& v, float scalar)
{
    return XMFLOAT3(v.x * scalar, v.y * scalar, v.z * scalar);
}

XMFLOAT3 MathUtils::Divide(const XMFLOAT3& v, float scalar)
{
    if (scalar != 0.0f)
    {
        float invScalar = 1.0f / scalar;
        return XMFLOAT3(v.x * invScalar, v.y * invScalar, v.z * invScalar);
    }
    return XMFLOAT3(0.0f, 0.0f, 0.0f);
}

float MathUtils::Dot(const XMFLOAT3& a, const XMFLOAT3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

XMFLOAT3 MathUtils::Cross(const XMFLOAT3& a, const XMFLOAT3& b)
{
    return XMFLOAT3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

XMFLOAT3 MathUtils::Normalize(const XMFLOAT3& v)
{
    float length = Length(v);
    if (length > 0.0f)
    {
        return Divide(v, length);
    }
    return XMFLOAT3(0.0f, 0.0f, 0.0f);
}

float MathUtils::Length(const XMFLOAT3& v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float MathUtils::LengthSquared(const XMFLOAT3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

// =============================================================================
// EASING FUNCTIONS
// =============================================================================

float MathUtils::EaseInQuad(float t)
{
    return t * t;
}

float MathUtils::EaseOutQuad(float t)
{
    return 1.0f - (1.0f - t) * (1.0f - t);
}

float MathUtils::EaseInOutQuad(float t)
{
    if (t < 0.5f)
    {
        return 2.0f * t * t;
    }
    else
    {
        return 1.0f - 2.0f * (1.0f - t) * (1.0f - t);
    }
}

float MathUtils::EaseInCubic(float t)
{
    return t * t * t;
}

float MathUtils::EaseOutCubic(float t)
{
    float f = 1.0f - t;
    return 1.0f - f * f * f;
}

float MathUtils::EaseInOutCubic(float t)
{
    if (t < 0.5f)
    {
        return 4.0f * t * t * t;
    }
    else
    {
        float f = 2.0f * t - 2.0f;
        return 1.0f + f * f * f * 0.5f;
    }
}
