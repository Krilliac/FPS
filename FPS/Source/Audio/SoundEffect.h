#pragma once
#include "..\Core\framework.h"
#include <xaudio2.h>
#include <vector>

class SoundEffect
{
private:
    WAVEFORMATEX m_format;
    std::vector<BYTE> m_audioData;
    DWORD m_audioDataSize;

public:
    SoundEffect();
    ~SoundEffect();

    // Loading
    HRESULT LoadFromFile(const std::wstring& filename);
    HRESULT LoadFromMemory(const BYTE* data, DWORD dataSize);
    void Unload();

    // Accessors
    const WAVEFORMATEX& GetFormat() const { return m_format; }
    const BYTE* GetData() const { return m_audioData.data(); }
    DWORD GetDataSize() const { return m_audioDataSize; }
    
    // Properties
    float GetDuration() const;
    DWORD GetSampleRate() const { return m_format.nSamplesPerSec; }
    WORD GetChannels() const { return m_format.nChannels; }
    WORD GetBitsPerSample() const { return m_format.wBitsPerSample; }
    
    bool IsLoaded() const { return !m_audioData.empty(); }

private:
    HRESULT ParseWAVFile(const BYTE* fileData, DWORD fileSize);
    HRESULT FindChunk(const BYTE* data, DWORD dataSize, DWORD fourcc, DWORD& chunkSize, DWORD& chunkDataPosition);
    HRESULT ReadChunkData(const BYTE* data, DWORD chunkDataPosition, void* buffer, DWORD buffersize);
};

// Sound effect factory for common sounds
class SoundEffectFactory
{
public:
    // Generate simple procedural sounds
    static std::unique_ptr<SoundEffect> CreateBeep(float frequency = 440.0f, float duration = 0.5f);
    static std::unique_ptr<SoundEffect> CreateNoise(float duration = 1.0f);
    static std::unique_ptr<SoundEffect> CreateSine(float frequency = 440.0f, float duration = 1.0f);
    
    // Common game sounds
    static std::unique_ptr<SoundEffect> CreateGunshot();
    static std::unique_ptr<SoundEffect> CreateExplosion();
    static std::unique_ptr<SoundEffect> CreateFootstep();
    static std::unique_ptr<SoundEffect> CreateReload();
    static std::unique_ptr<SoundEffect> CreatePickup();

private:
    static void GenerateWaveform(std::vector<short>& samples, float frequency, float duration, float (*waveformFunc)(float));
    static std::unique_ptr<SoundEffect> CreateFromSamples(const std::vector<short>& samples, DWORD sampleRate = 44100);
    static float SineWave(float t);
    static float NoiseWave(float t);
};
