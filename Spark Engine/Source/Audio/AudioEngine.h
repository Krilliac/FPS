/**
 * @file AudioEngine.h
 * @brief XAudio2-based audio engine with 3D spatial audio support
 * @author Spark Engine Team
 * @date 2025
 * 
 * This class provides a comprehensive audio system built on XAudio2, supporting
 * both 2D and 3D audio playback, sound effect management, volume controls, and
 * an object pool system for efficient audio source management. The engine handles
 * loading, playing, and managing multiple simultaneous audio sources.
 */

#pragma once

#include "Utils/Assert.h"
#include "SoundEffect.h"
#include <d3d11.h>
#include <xaudio2.h>
#include <DirectXMath.h>
#include <unordered_map>
#include <vector>
#include <memory>

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;

/**
 * @brief Audio source structure for managing individual sound instances
 * 
 * Represents a single audio source that can play a sound effect with specific
 * properties like position, velocity, volume, and pitch. Used for both 2D
 * and 3D audio playback within the audio engine's object pool system.
 */
struct AudioSource
{
    IXAudio2SourceVoice* Voice;    ///< XAudio2 source voice for audio playback
    XMFLOAT3 Position;             ///< 3D world position for spatial audio
    XMFLOAT3 Velocity;             ///< 3D velocity for Doppler effects
    float    Volume;               ///< Volume level (0.0 to 1.0+)
    float    Pitch;                ///< Pitch multiplier (1.0 = normal pitch)
    bool     Is3D;                 ///< Whether this source uses 3D positioning
    bool     IsLooping;            ///< Whether the sound should loop continuously
    bool     IsPlaying;            ///< Whether the source is currently playing
    SoundEffect* Sound;            ///< Pointer to the associated sound effect

    /**
     * @brief Default constructor with safe initial values
     * 
     * Initializes all members to safe defaults suitable for audio playback.
     */
    AudioSource()
        : Voice(nullptr)
        , Position(0, 0, 0)
        , Velocity(0, 0, 0)
        , Volume(1.0f)
        , Pitch(1.0f)
        , Is3D(false)
        , IsLooping(false)
        , IsPlaying(false)
        , Sound(nullptr)
    {
    }
};

/**
 * @brief Main audio engine class providing comprehensive audio functionality
 * 
 * The AudioEngine class manages all audio operations for the Spark Engine using
 * XAudio2 as the underlying audio API. It provides sound loading, 3D spatial audio,
 * volume controls, and efficient audio source management through object pooling.
 * 
 * Features include:
 * - XAudio2-based audio playback with hardware acceleration
 * - 2D and 3D spatial audio positioning
 * - Sound effect loading and management
 * - Volume controls (master, SFX, music)
 * - Audio source pooling for performance
 * - Looping and one-shot audio playback
 * - Pitch and volume controls per source
 * 
 * @note The engine uses an object pool to efficiently reuse audio sources
 * @warning Initialize() must be called before any audio operations
 */
class AudioEngine
{
public:
    /**
     * @brief Default constructor
     * 
     * Initializes member variables to safe default values. Call Initialize()
     * to set up the XAudio2 system.
     */
    AudioEngine();

    /**
     * @brief Destructor
     * 
     * Automatically calls Shutdown() to ensure proper cleanup of all
     * XAudio2 resources and audio sources.
     */
    ~AudioEngine();

    /**
     * @brief Initialize the audio engine with XAudio2
     * 
     * Sets up the XAudio2 engine, creates the mastering voice, and initializes
     * the audio source object pool with the specified number of sources.
     * 
     * @param maxSources Maximum number of simultaneous audio sources
     * @return HRESULT indicating success or failure of audio initialization
     * @note A typical value for maxSources is 32-64 for most games
     */
    HRESULT Initialize(size_t maxSources);

    /**
     * @brief Update the audio engine for the current frame
     * 
     * Updates all active audio sources, processes 3D audio calculations,
     * and manages the audio source pool. Should be called once per frame.
     * 
     * @param deltaTime Time elapsed since last frame in seconds
     */
    void    Update(float deltaTime);

    /**
     * @brief Shutdown the audio engine and clean up resources
     * 
     * Stops all playing sounds, releases all XAudio2 resources, and cleans
     * up the audio source pool. Safe to call multiple times.
     */
    void    Shutdown();

    /**
     * @brief Load a sound effect from file
     * 
     * Loads an audio file and associates it with a name for later playback.
     * Supports common audio formats like WAV files.
     * 
     * @param name Unique name to identify the sound effect
     * @param filename Path to the audio file to load
     * @return HRESULT indicating success or failure of sound loading
     */
    HRESULT LoadSound(const std::string& name, const std::wstring& filename);

    /**
     * @brief Unload a previously loaded sound effect
     * 
     * Removes a sound effect from memory and stops any instances currently
     * playing that sound.
     * 
     * @param name Name of the sound effect to unload
     */
    void    UnloadSound(const std::string& name);

    /**
     * @brief Get a pointer to a loaded sound effect
     * 
     * Retrieves a sound effect by name for direct manipulation or inspection.
     * 
     * @param name Name of the sound effect to retrieve
     * @return Pointer to the SoundEffect, or nullptr if not found
     */
    SoundEffect* GetSound(const std::string& name);

    /**
     * @brief Play a 2D sound effect
     * 
     * Plays a sound effect without 3D positioning. The sound will be heard
     * at the same volume regardless of listener position.
     * 
     * @param name Name of the loaded sound effect to play
     * @param volume Volume level (0.0 to 1.0+, default: 1.0)
     * @param pitch Pitch multiplier (1.0 = normal pitch, default: 1.0)
     * @param loop Whether to loop the sound continuously (default: false)
     * @return Pointer to the AudioSource playing the sound, or nullptr if failed
     */
    AudioSource* PlaySound(const std::string& name, float volume = 1.0f, float pitch = 1.0f, bool loop = false);

    /**
     * @brief Play a 3D positioned sound effect
     * 
     * Plays a sound effect with 3D spatial positioning. The volume and stereo
     * panning will be calculated based on the distance and direction from the listener.
     * 
     * @param name Name of the loaded sound effect to play
     * @param position 3D world position where the sound originates
     * @param volume Base volume level before 3D attenuation (default: 1.0)
     * @param pitch Pitch multiplier (1.0 = normal pitch, default: 1.0)
     * @param loop Whether to loop the sound continuously (default: false)
     * @return Pointer to the AudioSource playing the sound, or nullptr if failed
     */
    AudioSource* PlaySound3D(const std::string& name, const XMFLOAT3& position, float volume = 1.0f, float pitch = 1.0f, bool loop = false);

    /**
     * @brief Stop a specific audio source
     * 
     * Stops playback of the specified audio source and returns it to the
     * available source pool for reuse.
     * 
     * @param source Pointer to the AudioSource to stop
     */
    void StopSound(AudioSource* source);

    /**
     * @brief Stop all currently playing sounds
     * 
     * Immediately stops all active audio sources and returns them to the
     * available pool. Useful for scene transitions or pause functionality.
     */
    void StopAllSounds();

    /**
     * @brief Pause all currently playing sounds
     * 
     * Pauses all active audio sources without stopping them. Sounds can
     * be resumed later with ResumeAllSounds().
     */
    void PauseAllSounds();

    /**
     * @brief Resume all paused sounds
     * 
     * Resumes playback of all previously paused audio sources.
     */
    void ResumeAllSounds();

    /**
     * @brief Set the master volume level
     * 
     * Adjusts the overall volume for all audio output. This affects all
     * sound categories (SFX, music, etc.).
     * 
     * @param volume Master volume level (0.0 = silent, 1.0 = full volume)
     */
    void SetMasterVolume(float volume);

    /**
     * @brief Set the sound effects volume level
     * 
     * Adjusts the volume level specifically for sound effects, allowing
     * separate control from music and other audio categories.
     * 
     * @param volume SFX volume level (0.0 = silent, 1.0 = full volume)
     */
    void SetSFXVolume(float volume);

    /**
     * @brief Set the music volume level
     * 
     * Adjusts the volume level specifically for music tracks, allowing
     * separate control from sound effects and other audio categories.
     * 
     * @param volume Music volume level (0.0 = silent, 1.0 = full volume)
     */
    void SetMusicVolume(float volume);

    /**
     * @brief Get the number of currently active audio sources
     * 
     * Returns the count of audio sources that are currently playing sounds.
     * Useful for debugging and performance monitoring.
     * 
     * @return Number of active audio sources
     */
    size_t GetActiveSourceCount() const;

private:
    /**
     * @brief Create an XAudio2 source voice for audio playback
     * 
     * Internal method for creating source voices with the specified audio format.
     * 
     * @param format Audio format description for the source voice
     * @param voice Output parameter for the created source voice
     * @return HRESULT indicating success or failure of voice creation
     */
    HRESULT CreateSourceVoice(const WAVEFORMATEX& format, IXAudio2SourceVoice** voice);

    /**
     * @brief Update all active audio sources
     * 
     * Processes 3D audio calculations, updates source voice parameters,
     * and handles audio source lifecycle management.
     */
    void UpdateSources();

    /**
     * @brief Get an available audio source from the pool
     * 
     * Retrieves an unused audio source for playing a new sound. If no
     * sources are available, may stop the oldest playing source.
     * 
     * @return Pointer to an available AudioSource, or nullptr if none available
     */
    AudioSource* GetAvailableSource();

    /**
     * @brief Return an audio source to the available pool
     * 
     * Marks an audio source as available for reuse and cleans up its state.
     * 
     * @param source Pointer to the AudioSource to return to the pool
     */
    void ReturnSource(AudioSource* source);

    IXAudio2* m_xAudio2;                        ///< Main XAudio2 engine interface
    IXAudio2MasteringVoice* m_masterVoice;      ///< XAudio2 mastering voice for final output
    float                               m_masterVolume;  ///< Current master volume level
    float                               m_sfxVolume;     ///< Current sound effects volume level
    float                               m_musicVolume;   ///< Current music volume level
    size_t                              m_maxSources;    ///< Maximum number of simultaneous sources

    std::vector<std::unique_ptr<AudioSource>> m_audioSources;     ///< Pool of all audio sources
    std::vector<AudioSource*>                m_availableSources;  ///< Pool of available sources
    std::unordered_map<std::string, std::unique_ptr<SoundEffect>> m_soundEffects; ///< Loaded sound effects by name
};