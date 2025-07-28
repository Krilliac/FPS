#include "SoundEffect.h"
#include <fstream>
#include <cmath>
#include <random>

SoundEffect::SoundEffect()
    : m_audioDataSize(0)
{
    ZeroMemory(&m_format, sizeof(m_format));
}

SoundEffect::~SoundEffect()
{
    Unload();
}

HRESULT SoundEffect::LoadFromFile(const std::wstring& filename)
{
    // Open file
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
    }
    
    // Get file size
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Read entire file
    std::vector<BYTE> fileData(fileSize);
    if (!file.read(reinterpret_cast<char*>(fileData.data()), fileSize))
    {
        return E_FAIL;
    }
    
    return ParseWAVFile(fileData.data(), static_cast<DWORD>(fileSize));
}

HRESULT SoundEffect::LoadFromMemory(const BYTE* data, DWORD dataSize)
{
    return ParseWAVFile(data, dataSize);
}

void SoundEffect::Unload()
{
    m_audioData.clear();
    m_audioDataSize = 0;
    ZeroMemory(&m_format, sizeof(m_format));
}

float SoundEffect::GetDuration() const
{
    if (m_format.nAvgBytesPerSec == 0) return 0.0f;
    return static_cast<float>(m_audioDataSize) / static_cast<float>(m_format.nAvgBytesPerSec);
}

HRESULT SoundEffect::ParseWAVFile(const BYTE* fileData, DWORD fileSize)
{
    // Find format chunk
    DWORD formatChunkSize;
    DWORD formatChunkPosition;
    HRESULT hr = FindChunk(fileData, fileSize, 0x20746d66, formatChunkSize, formatChunkPosition); // 'fmt '
    if (FAILED(hr)) return hr;
    
    // Read format
    hr = ReadChunkData(fileData, formatChunkPosition, &m_format, formatChunkSize);
    if (FAILED(hr)) return hr;
    
    // Find data chunk
    DWORD dataChunkSize;
    DWORD dataChunkPosition;
    hr = FindChunk(fileData, fileSize, 0x61746164, dataChunkSize, dataChunkPosition); // 'data'
    if (FAILED(hr)) return hr;
    
    // Read audio data
    m_audioData.resize(dataChunkSize);
    hr = ReadChunkData(fileData, dataChunkPosition, m_audioData.data(), dataChunkSize);
    if (FAILED(hr)) return hr;
    
    m_audioDataSize = dataChunkSize;
    return S_OK;
}

HRESULT SoundEffect::FindChunk(const BYTE* data, DWORD dataSize, DWORD fourcc, DWORD& chunkSize, DWORD& chunkDataPosition)
{
    DWORD offset = 12; // Skip RIFF header
    
    while (offset < dataSize)
    {
        DWORD chunkType = *reinterpret_cast<const DWORD*>(data + offset);
        DWORD chunkDataSize = *reinterpret_cast<const DWORD*>(data + offset + 4);
        
        if (chunkType == fourcc)
        {
            chunkSize = chunkDataSize;
            chunkDataPosition = offset + 8;
            return S_OK;
        }
        
        offset += 8 + chunkDataSize;
        if (chunkDataSize % 2 != 0) offset++; // Pad to even boundary
    }
    
    return E_FAIL;
}

HRESULT SoundEffect::ReadChunkData(const BYTE* data, DWORD chunkDataPosition, void* buffer, DWORD buffersize)
{
    memcpy(buffer, data + chunkDataPosition, buffersize);
    return S_OK;
}

// SoundEffectFactory implementation
std::unique_ptr<SoundEffect> SoundEffectFactory::CreateBeep(float frequency, float duration)
{
    std::vector<short> samples;
    GenerateWaveform(samples, frequency, duration, SineWave);
    return CreateFromSamples(samples);
}

std::unique_ptr<SoundEffect> SoundEffectFactory::CreateNoise(float duration)
{
    std::vector<short> samples;
    GenerateWaveform(samples, 0.0f, duration, NoiseWave);
    return CreateFromSamples(samples);
}

std::unique_ptr<SoundEffect> SoundEffectFactory::CreateSine(float frequency, float duration)
{
    std::vector<short> samples;
    GenerateWaveform(samples, frequency, duration, SineWave);
    return CreateFromSamples(samples);
}

std::unique_ptr<SoundEffect> SoundEffectFactory::CreateGunshot()
{
    // Create a quick burst of noise with exponential decay
    const DWORD sampleRate = 44100;
    const float duration = 0.1f;
    const DWORD numSamples = static_cast<DWORD>(duration * sampleRate);
    
    std::vector<short> samples(numSamples);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    
    for (DWORD i = 0; i < numSamples; ++i)
    {
        float t = static_cast<float>(i) / sampleRate;
        float decay = expf(-t * 50.0f); // Exponential decay
        float noise = dis(gen);
        samples[i] = static_cast<short>(noise * decay * 32767.0f);
    }
    
    return CreateFromSamples(samples, sampleRate);
}

std::unique_ptr<SoundEffect> SoundEffectFactory::CreateExplosion()
{
    // Create low-frequency rumble
    const DWORD sampleRate = 44100;
    const float duration = 1.0f;
    const DWORD numSamples = static_cast<DWORD>(duration * sampleRate);
    
    std::vector<short> samples(numSamples);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    
    for (DWORD i = 0; i < numSamples; ++i)
    {
        float t = static_cast<float>(i) / sampleRate;
        float decay = expf(-t * 3.0f);
        
        // Low frequency rumble
        float rumble = sinf(2.0f * 3.14159f * 60.0f * t) * 0.5f;
        // High frequency noise
        float noise = dis(gen) * 0.3f;
        
        float sample = (rumble + noise) * decay;
        samples[i] = static_cast<short>(sample * 32767.0f);
    }
    
    return CreateFromSamples(samples, sampleRate);
}

std::unique_ptr<SoundEffect> SoundEffectFactory::CreateFootstep()
{
    // Create a short thud sound
    const DWORD sampleRate = 44100;
    const float duration = 0.2f;
    const DWORD numSamples = static_cast<DWORD>(duration * sampleRate);
    
    std::vector<short> samples(numSamples);
    
    for (DWORD i = 0; i < numSamples; ++i)
    {
        float t = static_cast<float>(i) / sampleRate;
        float decay = expf(-t * 20.0f);
        
        // Low frequency thump
        float thump = sinf(2.0f * 3.14159f * 120.0f * t) * decay;
        samples[i] = static_cast<short>(thump * 16383.0f); // Half volume
    }
    
    return CreateFromSamples(samples, sampleRate);
}

std::unique_ptr<SoundEffect> SoundEffectFactory::CreateReload()
{
    // Create metallic click sounds
    const DWORD sampleRate = 44100;
    const float duration = 0.3f;
    const DWORD numSamples = static_cast<DWORD>(duration * sampleRate);
    
    std::vector<short> samples(numSamples);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-0.3f, 0.3f);
    
    for (DWORD i = 0; i < numSamples; ++i)
    {
        float t = static_cast<float>(i) / sampleRate;
        float sample = 0.0f;
        
        // Click at beginning
        if (t < 0.05f)
        {
            sample = sinf(2.0f * 3.14159f * 2000.0f * t) * expf(-t * 50.0f);
        }
        // Click at end
        else if (t > 0.25f)
        {
            float clickT = t - 0.25f;
            sample = sinf(2.0f * 3.14159f * 1500.0f * clickT) * expf(-clickT * 40.0f);
        }
        
        // Add some metallic noise
        sample += dis(gen) * 0.1f * expf(-t * 5.0f);
        
        samples[i] = static_cast<short>(sample * 16383.0f);
    }
    
    return CreateFromSamples(samples, sampleRate);
}

std::unique_ptr<SoundEffect> SoundEffectFactory::CreatePickup()
{
    // Create ascending tone
    const DWORD sampleRate = 44100;
    const float duration = 0.3f;
    const DWORD numSamples = static_cast<DWORD>(duration * sampleRate);
    
    std::vector<short> samples(numSamples);
    
    for (DWORD i = 0; i < numSamples; ++i)
    {
        float t = static_cast<float>(i) / sampleRate;
        float progress = t / duration;
        
        // Ascending frequency from 440Hz to 880Hz
        float frequency = 440.0f + (440.0f * progress);
        float amplitude = 1.0f - progress; // Fade out
        
        float sample = sinf(2.0f * 3.14159f * frequency * t) * amplitude;
        samples[i] = static_cast<short>(sample * 16383.0f);
    }
    
    return CreateFromSamples(samples, sampleRate);
}

void SoundEffectFactory::GenerateWaveform(std::vector<short>& samples, float frequency, float duration, float (*waveformFunc)(float))
{
    const DWORD sampleRate = 44100;
    const DWORD numSamples = static_cast<DWORD>(duration * sampleRate);
    
    samples.resize(numSamples);
    
    for (DWORD i = 0; i < numSamples; ++i)
    {
        float t = static_cast<float>(i) / sampleRate;
        float phase = 2.0f * 3.14159f * frequency * t;
        float sample = waveformFunc(phase);
        samples[i] = static_cast<short>(sample * 16383.0f); // Scale to 16-bit range
    }
}

std::unique_ptr<SoundEffect> SoundEffectFactory::CreateFromSamples(const std::vector<short>& samples, DWORD sampleRate)
{
    auto soundEffect = std::make_unique<SoundEffect>();
    
    // Create audio data
    DWORD dataSize = static_cast<DWORD>(samples.size() * sizeof(short));
    const BYTE* audioData = reinterpret_cast<const BYTE*>(samples.data());
    
    soundEffect->LoadFromMemory(audioData, dataSize);
    
    return soundEffect;
}

float SoundEffectFactory::SineWave(float t)
{
    return sinf(t);
}

float SoundEffectFactory::NoiseWave(float t)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    return dis(gen);
}

