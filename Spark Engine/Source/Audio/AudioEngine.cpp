// AudioEngine.cpp
#include "AudioEngine.h"
#include "Utils/Assert.h"
#include <algorithm>
#include <iostream>

AudioEngine::AudioEngine()
{
    std::wcout << L"[INFO] AudioEngine constructed." << std::endl;
}

AudioEngine::~AudioEngine()
{
    std::wcout << L"[INFO] AudioEngine destructor called." << std::endl;
    Shutdown();
}

HRESULT AudioEngine::Initialize(size_t maxSources)
{
    std::wcout << L"[OPERATION] AudioEngine::Initialize called." << std::endl;
    ASSERT_MSG(maxSources > 0, "AudioEngine maxSources must be positive");
    m_maxSources = maxSources;

    HRESULT hr = XAudio2Create(&m_xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    ASSERT_MSG(SUCCEEDED(hr), "XAudio2Create failed");
    if (FAILED(hr)) return hr;

    hr = m_xAudio2->CreateMasteringVoice(&m_masterVoice);
    ASSERT_MSG(SUCCEEDED(hr), "CreateMasteringVoice failed");
    if (FAILED(hr)) return hr;

    m_audioSources.clear();
    m_availableSources.clear();
    m_audioSources.reserve(m_maxSources);
    m_availableSources.reserve(m_maxSources);

    for (size_t i = 0; i < m_maxSources; ++i)
    {
        auto source = std::make_unique<AudioSource>();
        ASSERT_NOT_NULL(source.get());
        m_availableSources.push_back(source.get());
        m_audioSources.push_back(std::move(source));
    }

    std::wcout << L"[INFO] AudioEngine initialization successful." << std::endl;
    return S_OK;
}

void AudioEngine::Update(float /*deltaTime*/)
{
    std::wcout << L"[OPERATION] AudioEngine::Update called." << std::endl;
    UpdateSources();
    std::wcout << L"[INFO] AudioEngine update complete." << std::endl;
}

void AudioEngine::Shutdown()
{
    std::wcout << L"[OPERATION] AudioEngine::Shutdown called." << std::endl;
    StopAllSounds();
    m_soundEffects.clear();
    m_audioSources.clear();
    m_availableSources.clear();

    if (m_masterVoice)
    {
        m_masterVoice->DestroyVoice();
        m_masterVoice = nullptr;
    }
    if (m_xAudio2)
    {
        m_xAudio2->Release();
        m_xAudio2 = nullptr;
    }
    std::wcout << L"[INFO] AudioEngine shutdown complete." << std::endl;
}

HRESULT AudioEngine::LoadSound(const std::string& name, const std::wstring& filename)
{
    ASSERT_MSG(!name.empty(), "Sound name must be non-empty");
    auto sound = std::make_unique<SoundEffect>();
    ASSERT_NOT_NULL(sound.get());

    HRESULT hr = sound->LoadFromFile(filename);
    ASSERT_MSG(SUCCEEDED(hr), "SoundEffect::LoadFromFile failed");
    if (FAILED(hr)) return hr;

    m_soundEffects[name] = std::move(sound);
    return S_OK;
}

void AudioEngine::UnloadSound(const std::string& name)
{
    auto it = m_soundEffects.find(name);
    if (it != m_soundEffects.end())
    {
        for (auto& s : m_audioSources)
        {
            if (s->Sound == it->second.get() && s->IsPlaying)
                StopSound(s.get());
        }
        m_soundEffects.erase(it);
    }
}

SoundEffect* AudioEngine::GetSound(const std::string& name)
{
    auto it = m_soundEffects.find(name);
    return it != m_soundEffects.end() ? it->second.get() : nullptr;
}

AudioSource* AudioEngine::PlaySound(const std::string& name,
    float volume, float pitch, bool loop)
{
    ASSERT_MSG(!name.empty(), "Sound name must be non-empty");
    SoundEffect* sound = GetSound(name);
    ASSERT_NOT_NULL(sound);

    AudioSource* src = GetAvailableSource();
    ASSERT_NOT_NULL(src);
    if (!src) return nullptr;

    if (!src->Voice)
    {
        HRESULT hr = CreateSourceVoice(sound->GetFormat(), &src->Voice);
        ASSERT_MSG(SUCCEEDED(hr), "CreateSourceVoice failed");
        if (FAILED(hr)) return nullptr;
    }

    ASSERT_MSG(volume >= 0.0f && volume <= 1.0f, "Volume out of range");
    ASSERT_MSG(pitch > 0.0f, "Pitch must be positive");

    src->Sound = sound;
    src->Volume = volume * m_sfxVolume * m_masterVolume;
    src->Pitch = pitch;
    src->IsLooping = loop;
    src->Is3D = false;
    src->IsPlaying = true;

    src->Voice->SetVolume(src->Volume);
    src->Voice->SetFrequencyRatio(src->Pitch);

    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = sound->GetDataSize();
    ASSERT_MSG(buffer.AudioBytes > 0, "Empty audio data");
    buffer.pAudioData = sound->GetData();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    if (loop) buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

    HRESULT hr = src->Voice->SubmitSourceBuffer(&buffer);
    ASSERT_MSG(SUCCEEDED(hr), "SubmitSourceBuffer failed");
    if (FAILED(hr)) return nullptr;

    src->Voice->Start();
    return src;
}

AudioSource* AudioEngine::PlaySound3D(const std::string& name,
    const XMFLOAT3& position, float volume, float pitch, bool loop)
{
    AudioSource* src = PlaySound(name, volume, pitch, loop);
    if (src)
    {
        src->Is3D = true;
        src->Position = position;
    }
    return src;
}

void AudioEngine::StopSound(AudioSource* source)
{
    ASSERT_NOT_NULL(source);
    if (source->IsPlaying && source->Voice)
    {
        source->Voice->Stop();
        source->Voice->FlushSourceBuffers();
        source->IsPlaying = false;
        ReturnSource(source);
    }
}

void AudioEngine::StopAllSounds()
{
    for (auto& s : m_audioSources)
        if (s->IsPlaying) StopSound(s.get());
}

void AudioEngine::PauseAllSounds()
{
    for (auto& s : m_audioSources)
        if (s->IsPlaying && s->Voice) s->Voice->Stop();
}

void AudioEngine::ResumeAllSounds()
{
    for (auto& s : m_audioSources)
        if (s->IsPlaying && s->Voice) s->Voice->Start();
}

void AudioEngine::SetMasterVolume(float volume)
{
    m_masterVolume = std::clamp(volume, 0.0f, 1.0f);
    if (m_masterVoice) m_masterVoice->SetVolume(m_masterVolume);
}

void AudioEngine::SetSFXVolume(float volume)
{
    m_sfxVolume = std::clamp(volume, 0.0f, 1.0f);
}

void AudioEngine::SetMusicVolume(float volume)
{
    m_musicVolume = std::clamp(volume, 0.0f, 1.0f);
}

size_t AudioEngine::GetActiveSourceCount() const
{
    size_t count = 0;
    for (auto& s : m_audioSources)
        if (s->IsPlaying) ++count;
    return count;
}

AudioSource* AudioEngine::GetAvailableSource()
{
    if (m_availableSources.empty())
        return nullptr;
    AudioSource* src = m_availableSources.back();
    m_availableSources.pop_back();
    return src;
}

void AudioEngine::ReturnSource(AudioSource* source)
{
    ASSERT_NOT_NULL(source);
    source->IsPlaying = false;
    source->Sound = nullptr;
    m_availableSources.push_back(source);
}

void AudioEngine::UpdateSources()
{
    for (auto& s : m_audioSources)
    {
        if (s->IsPlaying && s->Voice)
        {
            XAUDIO2_VOICE_STATE state;
            s->Voice->GetState(&state);
            if (state.BuffersQueued == 0)
                StopSound(s.get());
        }
    }
}

HRESULT AudioEngine::CreateSourceVoice(const WAVEFORMATEX& format, IXAudio2SourceVoice** voice)
{
    ASSERT_MSG(m_xAudio2 != nullptr, "XAudio2 not initialized");
    return m_xAudio2->CreateSourceVoice(voice, &format);
}