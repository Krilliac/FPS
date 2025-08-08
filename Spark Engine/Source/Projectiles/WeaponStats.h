/**
 * @file WeaponStats.h
 * @brief Weapon configuration and statistics system
 * @author Spark Engine Team
 * @date 2025
 * 
 * This file defines weapon types, statistics structures, and default configurations
 * for the weapon system. It provides a data-driven approach to weapon balancing
 * and configuration management.
 */

#pragma once

#include "Utils/Assert.h"

/**
 * @brief Enumeration of supported weapon types
 * 
 * Defines the different categories of weapons available in the game.
 * Each type has associated default statistics and behaviors.
 */
enum class WeaponType
{
    PISTOL,           ///< Semi-automatic pistol with moderate damage and high accuracy
    RIFLE,            ///< Automatic rifle with high damage and moderate accuracy
    SHOTGUN,          ///< Close-range weapon with high damage but low accuracy
    ROCKET_LAUNCHER,  ///< Explosive weapon with very high damage but slow fire rate
    GRENADE_LAUNCHER  ///< Area-of-effect weapon with explosive projectiles
};

/**
 * @brief Weapon statistics and configuration structure
 * 
 * Contains all the parameters that define a weapon's behavior including
 * damage, fire rate, ammunition, and ballistic properties.
 */
struct WeaponStats
{
    WeaponType Type;         ///< Weapon category/type
    float      Damage;       ///< Base damage per shot/projectile
    float      FireRate;     ///< Rate of fire in rounds per minute
    int        MagazineSize; ///< Number of rounds per magazine/clip
    float      ReloadTime;   ///< Time required to reload in seconds
    float      MuzzleVelocity; ///< Initial projectile speed in units per second
    float      Accuracy;     ///< Accuracy factor (0.0 = completely inaccurate, 1.0 = perfect accuracy)

    /**
     * @brief Default constructor with safe initial values
     */
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

/**
 * @brief Get default weapon statistics for a specific weapon type
 * 
 * Returns pre-configured weapon statistics for each weapon type with
 * balanced values suitable for gameplay. These can be used as baseline
 * values and modified as needed for game balancing.
 * 
 * @param type The weapon type to get default stats for
 * @return WeaponStats structure with default values for the specified type
 * @note Values are balanced for typical FPS gameplay scenarios
 * @warning Will assert if an unknown weapon type is provided
 */
inline WeaponStats GetDefaultWeaponStats(WeaponType type)
{
    WeaponStats stats;
    stats.Type = type;

    switch (type)
    {
    case WeaponType::PISTOL:
        stats.Damage = 25.0f;
        stats.FireRate = 300.0f;      // 5 shots per second
        stats.MagazineSize = 12;
        stats.ReloadTime = 1.5f;
        stats.MuzzleVelocity = 300.0f;
        stats.Accuracy = 0.95f;       // Very accurate
        break;

    case WeaponType::RIFLE:
        stats.Damage = 35.0f;
        stats.FireRate = 600.0f;      // 10 shots per second
        stats.MagazineSize = 30;
        stats.ReloadTime = 2.5f;
        stats.MuzzleVelocity = 800.0f;
        stats.Accuracy = 0.85f;       // Good accuracy
        break;

    case WeaponType::SHOTGUN:
        stats.Damage = 60.0f;
        stats.FireRate = 120.0f;      // 2 shots per second
        stats.MagazineSize = 8;
        stats.ReloadTime = 3.0f;
        stats.MuzzleVelocity = 400.0f;
        stats.Accuracy = 0.60f;       // Lower accuracy, spread pattern
        break;

    case WeaponType::ROCKET_LAUNCHER:
        stats.Damage = 150.0f;
        stats.FireRate = 60.0f;       // 1 shot per second
        stats.MagazineSize = 4;
        stats.ReloadTime = 4.0f;
        stats.MuzzleVelocity = 200.0f;
        stats.Accuracy = 1.00f;       // Perfect accuracy
        break;

    case WeaponType::GRENADE_LAUNCHER:
        stats.Damage = 100.0f;
        stats.FireRate = 90.0f;       // 1.5 shots per second
        stats.MagazineSize = 6;
        stats.ReloadTime = 3.5f;
        stats.MuzzleVelocity = 150.0f;
        stats.Accuracy = 0.90f;       // High accuracy
        break;

    default:
        ASSERT_ALWAYS_MSG(false, "Unknown WeaponType in GetDefaultWeaponStats");
        break;
    }

    return stats;
}