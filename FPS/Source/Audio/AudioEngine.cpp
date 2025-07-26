#include "AudioEngine.h"
#include "SoundEffect.h"

// AudioSource implementation
AudioSource::AudioSource()
    : Voice(nullptr)
    , Position(0.0f, 0.0f, 0.0f)
    , Velocity(0.0f, 0.0f, 0.0f)
    , Volume(1.0f)
    , Pitch(1.0f)
    , Is3D(false)
    , IsLooping(false)
    , IsPlaying(false)
    , Sound(nullptr)
{
}

AudioSource::~AudioSource()
{
    if (Voice)
    {
        Voice->DestroyVoice();
        Voice = nullptr;
    }
}

// AudioEngine implementation
AudioEngine::AudioEngine()
    : m_xAudio2(nullptr)
    , m_masterVoice(nullptr)
    , m_masterVolume(1.0f)
    , m_sfxVolume(0.8f)
    , m_musicVolume(0.6f)
    , m_maxSources(64)
{
}

AudioEngine::~AudioEngine()
{
    Shutdown();
}

HRESULT AudioEngine::Initialize(size_t maxSources)
{
    m_maxSources = maxSources;
    
    // Initialize XAudio2
    HRESULT hr = XAudio2Create(&m_xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) return hr;

    // Create mastering voice
    hr = m_xAudio2->CreateMasteringVoice(&m_masterVoice);
    if (FAILED(hr)) return hr;

    // Create audio source pool
    m_audioSources.reserve(maxSources);
    m_availableSources.reserve(maxSources);
    
    for (size_t i = 0; i < maxSources; ++i)
    {
        auto source = std::make_unique<AudioSource>();
        m_availableSources.push_back(source.get());
        m_audioSources.push_back(std::move(source));
    }

    return S_OK;
}

void AudioEngine::Update(float deltaTime)
{
    UpdateSources();
}

void AudioEngine::Shutdown()
{
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
}

HRESULT AudioEngine::LoadSound(const std::string& name, const std::wstring& filename)
{
    auto sound = std::make_unique<SoundEffect>();
    HRESULT hr = sound->LoadFromFile(filename);
    if (FAILED(hr)) return hr;
    
    m_soundEffects[name] = std::move(sound);
    return S_OK;
}

void AudioEngine::UnloadSound(const std::string& name)
{
    auto it = m_soundEffects.find(name);
    if (it != m_soundEffects.end())
    {
        // Stop all sources using this sound
        for (auto& source : m_audioSources)
        {
            if (source->Sound == it->second.get() && source->IsPlaying)
            {
                StopSound(source.get());
            }
        }
        
        m_soundEffects.erase(it);
    }
}

SoundEffect* AudioEngine::GetSound(const std::string& name)
{
    auto it = m_soundEffects.find(name);
    return (it != m_soundEffects.end()) ? it->second.get() : nullptr;
}

AudioSource* AudioEngine::PlaySound(const std::string& name, float volume, float pitch, bool loop)
{
    SoundEffect* sound = GetSound(name);
    if (!sound) return nullptr;
    
    AudioSource* source = GetAvailableSource();
    if (!source) return nullptr;
    
    // Create source voice if needed
    if (!source->Voice)
    {
        HRESULT hr = CreateSourceVoice(sound->GetFormat(), &source->Voice);
        if (FAILED(hr)) return nullptr;
    }
    
    // Configure source
    source->Sound = sound;
    source->Volume = volume * m_sfxVolume * m_masterVolume;
    source->Pitch = pitch;
    source->IsLooping = loop;
    source->Is3D = false;
    source->IsPlaying = true;
    
    // Set volume and pitch
    source->Voice->SetVolume(source->Volume);
    source->Voice->SetFrequencyRatio(pitch);
    
    // Submit audio buffer
    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = sound->GetDataSize();
    buffer.pAudioData = sound->GetData();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    if (loop)
    {
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }
    
    source->Voice->SubmitSourceBuffer(&buffer);
    source->Voice->Start();
    
    return source;
}

AudioSource* AudioEngine::PlaySound3D(const std::string& name, const XMFLOAT3& position, 
                                     float volume, float pitch, bool loop)
{
    AudioSource* source = PlaySound(name, volume, pitch, loop);
    if (source)
    {
        source->Is3D = true;
        source->Position = position;
    }
    return source;
}

void AudioEngine::StopSound(AudioSource* source)
{
    if (source && source->IsPlaying)
    {
        if (source->Voice)
        {
            source->Voice->Stop();
            source->Voice->FlushSourceBuffers();
        }
        
        source->IsPlaying = false;
        ReturnSource(source);
    }
}

void AudioEngine::StopAllSounds()
{
    for (auto& source : m_audioSources)
    {
        if (source->IsPlaying)
        {
            StopSound(source.get());
        }
    }
}

void AudioEngine::PauseAllSounds()
{
    for (auto& source : m_audioSources)
    {
        if (source->IsPlaying && source->Voice)
        {
            source->Voice->Stop();
        }
    }
}

void AudioEngine::ResumeAllSounds()
{
    for (auto& source : m_audioSources)
    {
        if (source->IsPlaying && source->Voice)
        {
            source->Voice->Start();
        }
    }
}

void AudioEngine::SetMasterVolume(float volume)
{
    m_masterVolume = std::max(0.0f, std::min(1.0f, volume));
    if (m_masterVoice)
    {
        m_masterVoice->SetVolume(m_masterVolume);
    }
}

void AudioEngine::SetSFXVolume(float volume)
{
    m_sfxVolume = std::max(0.0f, std::min(1.0f, volume));
}

void AudioEngine::SetMusicVolume(float volume)
{
    m_musicVolume = std::max(0.0f, std::min(1.0f, volume));
}

size_t AudioEngine::GetActiveSourceCount() const
{
    size_t count = 0;
    for (const auto& source : m_audioSources)
    {
        if (source->IsPlaying)
        {
            count++;
        }
    }
    return count;
}

AudioSource* AudioEngine::GetAvailableSource()
{
    if (m_availableSources.empty())
    {
        return nullptr;
    }
    
    AudioSource* source = m_availableSources.back();
    m_availableSources.pop_back();
    return source;
}

void AudioEngine::ReturnSource(AudioSource* source)
{
    if (source)
    {
        source->IsPlaying = false;
        source->Sound = nullptr;
        m_availableSources.push_back(source);
    }
}

void AudioEngine::UpdateSources()
{
    for (auto& source : m_audioSources)
    {
        if (source->IsPlaying && source->Voice)
        {
            // Check if source is still playing
            XAUDIO2_VOICE_STATE state;
            source->Voice->GetState(&state);
            
            if (state.BuffersQueued == 0)
            {
                // Sound finished playing
                StopSound(source.get());
            }
        }
    }
}

HRESULT AudioEngine::CreateSourceVoice(const WAVEFORMATEX& format, IXAudio2SourceVoice** voice)
{
    return m_xAudio2->CreateSourceVoice(voice, &format);
}
