// AudioEngine.h
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

struct AudioSource
{
    IXAudio2SourceVoice* Voice;
    XMFLOAT3 Position;
    XMFLOAT3 Velocity;
    float    Volume;
    float    Pitch;
    bool     Is3D;
    bool     IsLooping;
    bool     IsPlaying;
    SoundEffect* Sound;

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

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    HRESULT Initialize(size_t maxSources);
    void    Update(float deltaTime);
    void    Shutdown();

    HRESULT LoadSound(const std::string& name, const std::wstring& filename);
    void    UnloadSound(const std::string& name);
    SoundEffect* GetSound(const std::string& name);

    AudioSource* PlaySound(const std::string& name, float volume = 1.0f, float pitch = 1.0f, bool loop = false);
    AudioSource* PlaySound3D(const std::string& name, const XMFLOAT3& position, float volume = 1.0f, float pitch = 1.0f, bool loop = false);
    void StopSound(AudioSource* source);
    void StopAllSounds();
    void PauseAllSounds();
    void ResumeAllSounds();

    void SetMasterVolume(float volume);
    void SetSFXVolume(float volume);
    void SetMusicVolume(float volume);

    size_t GetActiveSourceCount() const;

private:
    HRESULT CreateSourceVoice(const WAVEFORMATEX& format, IXAudio2SourceVoice** voice);

    void UpdateSources();
    AudioSource* GetAvailableSource();
    void ReturnSource(AudioSource* source);

    IXAudio2* m_xAudio2;
    IXAudio2MasteringVoice* m_masterVoice;
    float                               m_masterVolume;
    float                               m_sfxVolume;
    float                               m_musicVolume;
    size_t                              m_maxSources;

    std::vector<std::unique_ptr<AudioSource>> m_audioSources;
    std::vector<AudioSource*>                m_availableSources;
    std::unordered_map<std::string, std::unique_ptr<SoundEffect>> m_soundEffects;
};