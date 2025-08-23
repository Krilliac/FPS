/**
 * @file GameSystemEnums.h
 * @brief Game system enumerations for Spark Engine
 * @author Spark Engine Team
 * @date 2025
 * 
 * This file contains enumerations related to game mechanics, weapon systems,
 * player states, AI behaviors, and other core game functionality.
 */

#pragma once

namespace SparkEditor {

/**
 * @brief Weapon types supported by the game system
 * 
 * Comprehensive list of weapon categories with distinct characteristics
 * and behaviors. Each weapon type has associated default statistics
 * defined in WeaponStats.h.
 */
enum class WeaponType {
    PISTOL = 0,              ///< Semi-automatic pistol with moderate damage and high accuracy
    RIFLE = 1,               ///< Battle rifle with high damage and moderate accuracy
    SHOTGUN = 2,             ///< Close-range weapon with high damage but low accuracy
    ROCKET_LAUNCHER = 3,     ///< Explosive weapon with very high damage but slow fire rate
    GRENADE_LAUNCHER = 4,    ///< Area-of-effect weapon with explosive projectiles
    SNIPER_RIFLE = 5,        ///< Long-range precision weapon with very high damage
    SUBMACHINE_GUN = 6,      ///< High rate of fire weapon with moderate damage
    ASSAULT_RIFLE = 7,       ///< Versatile automatic weapon balanced for combat
    MACHINE_GUN = 8,         ///< Heavy automatic weapon with sustained fire capability
    FLAMETHROWER = 9,        ///< Close-range continuous damage weapon
    PLASMA_RIFLE = 10,       ///< Energy weapon with unique projectile properties
    LASER_CANNON = 11,       ///< Instantaneous hit-scan energy weapon
    RAILGUN = 12,            ///< Electromagnetic projectile weapon with piercing capability
    MINIGUN = 13,            ///< Rotary cannon with extremely high rate of fire
    CROSSBOW = 14,           ///< Projectile weapon with special bolt types
    BOW = 15,                ///< Traditional ranged weapon with arrow projectiles
    THROWING_KNIFE = 16,     ///< Thrown melee weapon
    MELEE_WEAPON = 17,       ///< Close combat weapons (sword, axe, etc.)
    
    // Special/Utility weapons
    GRAPPLING_HOOK = 50,     ///< Utility weapon for traversal
    SCANNER = 51,            ///< Detection and analysis tool
    REPAIR_TOOL = 52,        ///< Engineering/repair equipment
    MEDICAL_TOOL = 53,       ///< Healing and medical equipment
    
    // Custom/Modded weapons
    CUSTOM_1 = 100,          ///< Custom weapon slot 1
    CUSTOM_2 = 101,          ///< Custom weapon slot 2
    CUSTOM_3 = 102,          ///< Custom weapon slot 3
    
    COUNT                    ///< Total number of weapon types (keep last)
};

/**
 * @brief Player movement states
 * 
 * Defines the various movement states a player character can be in,
 * affecting physics, animation, and gameplay mechanics.
 */
enum class MovementState {
    IDLE = 0,                ///< Player is not moving
    WALKING = 1,             ///< Normal walking speed
    RUNNING = 2,             ///< Increased movement speed
    SPRINTING = 3,           ///< Maximum movement speed
    CROUCHING = 4,           ///< Reduced profile, slower movement
    CRAWLING = 5,            ///< Prone position, minimal profile
    JUMPING = 6,             ///< In air from jump
    FALLING = 7,             ///< In air from gravity
    CLIMBING = 8,            ///< Climbing on surfaces
    SWIMMING = 9,            ///< Moving through water
    SLIDING = 10,            ///< Sliding down surfaces
    WALL_RUNNING = 11,       ///< Running along walls
    GRAPPLING = 12,          ///< Using grappling hook
    STUNNED = 13,            ///< Temporarily unable to move
    KNOCKED_DOWN = 14,       ///< Fallen and getting up
    DEAD = 15                ///< No movement possible
};

/**
 * @brief Player health states
 * 
 * Represents different health conditions that affect gameplay,
 * visual effects, and available actions.
 */
enum class HealthState {
    HEALTHY = 0,             ///< Full or high health
    INJURED = 1,             ///< Moderate damage taken
    CRITICAL = 2,            ///< Low health, near death
    BLEEDING = 3,            ///< Losing health over time
    POISONED = 4,            ///< Affected by poison/toxin
    BURNING = 5,             ///< Taking fire damage
    FROZEN = 6,              ///< Slowed by cold damage
    ELECTRIFIED = 7,         ///< Affected by electrical damage
    DEAD = 8,                ///< Health depleted
    INVULNERABLE = 9,        ///< Temporarily immune to damage
    REGENERATING = 10        ///< Actively healing
};

/**
 * @brief AI behavior states
 * 
 * Defines the primary behavior modes for AI-controlled entities,
 * determining their decision-making and actions.
 */
enum class AIBehaviorState {
    PASSIVE = 0,             ///< Non-aggressive, ignores player
    PATROL = 1,              ///< Following predetermined patrol route
    ALERT = 2,               ///< Investigating disturbance
    SEARCHING = 3,           ///< Actively looking for target
    PURSUING = 4,            ///< Chasing known target
    ATTACKING = 5,           ///< Engaging in combat
    FLEEING = 6,             ///< Retreating from threat
    GUARDING = 7,            ///< Protecting specific area/object
    FOLLOWING = 8,           ///< Following another entity
    STUNNED = 9,             ///< Temporarily incapacitated
    DEAD = 10,               ///< No longer active
    SCRIPTED = 11            ///< Following scripted sequence
};

/**
 * @brief Game difficulty levels
 * 
 * Affects enemy behavior, resource availability, damage scaling,
 * and other gameplay parameters.
 */
enum class DifficultyLevel {
    EASY = 0,                ///< Reduced challenge, more resources
    NORMAL = 1,              ///< Balanced gameplay experience
    HARD = 2,                ///< Increased challenge, fewer resources
    EXPERT = 3,              ///< Maximum difficulty, realistic damage
    CUSTOM = 4               ///< User-defined difficulty settings
};

/**
 * @brief Game session states
 * 
 * Represents the current state of the game session,
 * affecting UI, input handling, and system behavior.
 */
enum class GameSessionState {
    MENU = 0,                ///< In main menu
    LOADING = 1,             ///< Loading game content
    PLAYING = 2,             ///< Active gameplay
    PAUSED = 3,              ///< Game paused
    INVENTORY = 4,           ///< Inventory/equipment screen
    DIALOGUE = 5,            ///< In conversation/dialogue
    CUTSCENE = 6,            ///< Playing cinematic sequence
    GAME_OVER = 7,           ///< Game over screen
    VICTORY = 8,             ///< Victory/completion screen
    SETTINGS = 9,            ///< Settings/options menu
    SAVING = 10,             ///< Saving game data
    LOADING_SAVE = 11        ///< Loading saved game
};

/**
 * @brief Item rarity levels
 * 
 * Classification system for items, weapons, and equipment
 * affecting their properties, availability, and value.
 */
enum class ItemRarity {
    COMMON = 0,              ///< Basic items, widely available
    UNCOMMON = 1,            ///< Slightly better than common
    RARE = 2,                ///< Significantly improved properties
    EPIC = 3,                ///< High-quality items with special features
    LEGENDARY = 4,           ///< Extremely powerful, very rare
    ARTIFACT = 5,            ///< Unique items with special lore
    CUSTOM = 6               ///< Special event or custom items
};

/**
 * @brief Damage types for combat system
 * 
 * Different damage types that can affect entities differently
 * based on armor, resistances, and vulnerabilities.
 */
enum class DamageType {
    PHYSICAL = 0,            ///< Standard kinetic damage
    FIRE = 1,                ///< Heat-based damage
    ICE = 2,                 ///< Cold-based damage
    ELECTRIC = 3,            ///< Electrical damage
    POISON = 4,              ///< Chemical/toxic damage
    EXPLOSIVE = 5,           ///< Blast damage with area effect
    ENERGY = 6,              ///< Plasma/laser energy damage
    PSYCHIC = 7,             ///< Mental/psionic damage
    HOLY = 8,                ///< Divine/blessed damage
    DARK = 9,                ///< Shadow/curse damage
    SONIC = 10,              ///< Sound-based damage
    RADIATION = 11,          ///< Nuclear/radioactive damage
    PIERCING = 12,           ///< Armor-penetrating damage
    SLASHING = 13,           ///< Cutting damage
    BLUDGEONING = 14,        ///< Impact/crushing damage
    TRUE_DAMAGE = 15         ///< Unresistable damage
};

/**
 * @brief Interaction types for objects and NPCs
 * 
 * Defines how the player can interact with various
 * entities in the game world.
 */
enum class InteractionType {
    NONE = 0,                ///< No interaction available
    EXAMINE = 1,             ///< Look at object for information
    PICKUP = 2,              ///< Add item to inventory
    USE = 3,                 ///< Activate or use object
    TALK = 4,                ///< Initiate dialogue with NPC
    TRADE = 5,               ///< Open trading interface
    ATTACK = 6,              ///< Hostile interaction
    HEAL = 7,                ///< Provide medical assistance
    REPAIR = 8,              ///< Fix broken object
    HACK = 9,                ///< Computer/electronic interaction
    LOCKPICK = 10,           ///< Bypass lock mechanism
    CLIMB = 11,              ///< Climb on object
    PUSH = 12,               ///< Move object
    PULL = 13,               ///< Pull object toward player
    OPERATE = 14,            ///< Complex operation (machinery)
    READ = 15,               ///< Read text/documents
    SLEEP = 16,              ///< Rest/save point
    TELEPORT = 17,           ///< Fast travel point
    CRAFTING = 18,           ///< Access crafting interface
    STORAGE = 19             ///< Access storage container
};

/**
 * @brief Achievement/trophy categories
 * 
 * Classification system for game achievements
 * and progress tracking.
 */
enum class AchievementCategory {
    STORY = 0,               ///< Main story progress
    COMBAT = 1,              ///< Combat-related achievements
    EXPLORATION = 2,         ///< Discovery and exploration
    COLLECTION = 3,          ///< Item/collectible gathering
    SKILL = 4,               ///< Demonstrate specific skills
    SOCIAL = 5,              ///< Multiplayer/social achievements
    HIDDEN = 6,              ///< Secret achievements
    CHALLENGE = 7,           ///< Special challenge completion
    SPEEDRUN = 8,            ///< Time-based achievements
    PERFECTIONIST = 9,       ///< 100% completion achievements
    MISCELLANEOUS = 10       ///< Other achievements
};

/**
 * @brief Save file types
 * 
 * Different types of save data that can be
 * stored and loaded by the game system.
 */
enum class SaveFileType {
    QUICK_SAVE = 0,          ///< Temporary quick save
    AUTO_SAVE = 1,           ///< Automatic checkpoint save
    MANUAL_SAVE = 2,         ///< Player-initiated save
    CHAPTER_SAVE = 3,        ///< End of chapter/level save
    COMPLETION_SAVE = 4,     ///< Game completion save
    NEW_GAME_PLUS = 5,       ///< New Game+ save data
    BACKUP_SAVE = 6,         ///< Backup/recovery save
    CLOUD_SAVE = 7,          ///< Cloud synchronized save
    EXPORT_SAVE = 8          ///< Exported save for sharing
};

} // namespace SparkEditor