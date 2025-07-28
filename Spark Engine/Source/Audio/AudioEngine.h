#pragma once
#include "..\Core\framework.h"
#include <xaudio2.h>
#include <unordered_map>
#include <string>

#pragma comment(lib, "xaudio2.lib")

class SoundEffect;

struct AudioSource
{
    IXAudio2SourceVoice* Voice;
    XMFLOAT3 Position;
    XMFLOAT3 Velocity;
    float Volume;
    float Pitch;
    bool Is3D;
    bool IsLooping;
    bool IsPlaying;
    SoundEffect* Sound;
    
    AudioSource();
    ~AudioSource();
};

class AudioEngine
{
private:
    IXAudio2* m_xAudio2;
    IXAudio2MasteringVoice* m_masterVoice;
    
    std::unordered_map<std::string, std::unique_ptr<SoundEffect>> m_soundEffects;
    std::vector<std::unique_ptr<AudioSource>> m_audioSources;
    std::vector<AudioSource*> m_availableSources;
    
    // Settings
    float m_masterVolume;
    float m_sfxVolume;
    float m_musicVolume;
    
    size_t m_maxSources;

public:
    AudioEngine();
    ~AudioEngine();

    HRESULT Initialize(size_t maxSources = 64);
    void Update(float deltaTime);
    void Shutdown();

    // Sound loading and management
    HRESULT LoadSound(const std::string& name, const std::wstring& filename);
    void UnloadSound(const std::string& name);
    SoundEffect* GetSound(const std::string& name);

    // Sound playing
    AudioSource* PlaySound(const std::string& name, float volume = 1.0f, float pitch = 1.0f, bool loop = false);
    AudioSource* PlaySound3D(const std::string& name, const XMFLOAT3& position, 
                            float volume = 1.0f, float pitch = 1.0f, bool loop = false);
    
    void StopSound(AudioSource* source);
    void StopAllSounds();
    void PauseAllSounds();
    void ResumeAllSounds();

    // Volume controls
    void SetMasterVolume(float volume);
    void SetSFXVolume(float volume);
    void SetMusicVolume(float volume);
    
    float GetMasterVolume() const { return m_masterVolume; }
    float GetSFXVolume() const { return m_sfxVolume; }
    float GetMusicVolume() const { return m_musicVolume; }

    // Source management
    size_t GetActiveSourceCount() const;
    size_t GetAvailableSourceCount() const { return m_availableSources.size(); }

private:
    AudioSource* GetAvailableSource();
    void ReturnSource(AudioSource* source);
    void UpdateSources();
    
    HRESULT CreateSourceVoice(const WAVEFORMATEX& format, IXAudio2SourceVoice** voice);
};

