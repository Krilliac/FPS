// WeaponStats.h
#pragma once

#include "Utils/Assert.h"

// Supported weapon types
enum class WeaponType
{
    PISTOL,
    RIFLE,
    SHOTGUN,
    ROCKET_LAUNCHER,
    GRENADE_LAUNCHER
};

// Stats for each weapon
struct WeaponStats
{
    WeaponType Type;         // Weapon category
    float      Damage;       // Damage per shot
    float      FireRate;     // Rounds per minute
    int        MagazineSize; // Rounds per magazine
    float      ReloadTime;   // Seconds to reload
    float      MuzzleVelocity; // Projectile speed (units/sec)
    float      Accuracy;     // [0.0f .. 1.0f], higher is more accurate

    WeaponStats()
        : Type(WeaponType::PISTOL)
        , Damage(0.0f)
        , FireRate(0.0f)
        , MagazineSize(0)
        , ReloadTime(0.0f)
        , MuzzleVelocity(0.0f)
        , Accuracy(1.0f)
    {
    }
};

// Helper to retrieve default stats for each weapon type
inline WeaponStats GetDefaultWeaponStats(WeaponType type)
{
    WeaponStats stats;
    stats.Type = type;

    switch (type)
    {
    case WeaponType::PISTOL:
        stats.Damage = 25.0f;
        stats.FireRate = 300.0f;
        stats.MagazineSize = 12;
        stats.ReloadTime = 1.5f;
        stats.MuzzleVelocity = 300.0f;
        stats.Accuracy = 0.95f;
        break;

    case WeaponType::RIFLE:
        stats.Damage = 35.0f;
        stats.FireRate = 600.0f;
        stats.MagazineSize = 30;
        stats.ReloadTime = 2.5f;
        stats.MuzzleVelocity = 800.0f;
        stats.Accuracy = 0.85f;
        break;

    case WeaponType::SHOTGUN:
        stats.Damage = 60.0f;
        stats.FireRate = 120.0f;
        stats.MagazineSize = 8;
        stats.ReloadTime = 3.0f;
        stats.MuzzleVelocity = 400.0f;
        stats.Accuracy = 0.60f;
        break;

    case WeaponType::ROCKET_LAUNCHER:
        stats.Damage = 150.0f;
        stats.FireRate = 60.0f;
        stats.MagazineSize = 4;
        stats.ReloadTime = 4.0f;
        stats.MuzzleVelocity = 200.0f;
        stats.Accuracy = 1.00f;
        break;

    case WeaponType::GRENADE_LAUNCHER:
        stats.Damage = 100.0f;
        stats.FireRate = 90.0f;
        stats.MagazineSize = 6;
        stats.ReloadTime = 3.5f;
        stats.MuzzleVelocity = 150.0f;
        stats.Accuracy = 0.90f;
        break;

    default:
        ASSERT_ALWAYS_MSG(false, "Unknown WeaponType in GetDefaultWeaponStats");
        break;
    }

    return stats;
}