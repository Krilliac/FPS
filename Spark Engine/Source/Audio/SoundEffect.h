#pragma once

//------------------------------------------------------------------------------
//  SoundEffect ‒ simple WAV loader + in-memory buffer
//  Depends only on <xaudio2.h> and the custom Assert system.
//------------------------------------------------------------------------------

#include "Utils/Assert.h"
#include <xaudio2.h>
#include <vector>
#include <string>
#include <memory>

//──────────────────────────────────────────────────────────────────────────────
//  SoundEffect
//──────────────────────────────────────────────────────────────────────────────
class SoundEffect
{
public:
    SoundEffect();
    ~SoundEffect();

    // ------------------------------------------------------------------
    // Loading / Unloading
    // ------------------------------------------------------------------
    HRESULT LoadFromFile(const std::wstring& filename);
    HRESULT LoadFromMemory(const BYTE* data, DWORD dataSize);
    void    Unload();

    // ------------------------------------------------------------------
    // Accessors
    // ------------------------------------------------------------------
    const WAVEFORMATEX& GetFormat()   const { return m_format; }
    const BYTE* GetData()     const { return m_audioData.data(); }
    DWORD              GetDataSize() const { return m_audioDataSize; }
    bool               IsLoaded()    const { return m_audioDataSize != 0; }

    float  GetDuration()     const;                 // seconds
    DWORD  GetSampleRate()   const { return m_format.nSamplesPerSec; }
    WORD   GetChannels()     const { return m_format.nChannels; }
    WORD   GetBitsPerSample()const { return m_format.wBitsPerSample; }

private:
    WAVEFORMATEX              m_format;
    std::vector<BYTE>         m_audioData;
    DWORD                     m_audioDataSize;

    // internal helpers
    HRESULT ParseWAVFile(const BYTE* data, DWORD size);
    HRESULT FindChunk(const BYTE* data, DWORD dataSize,
        DWORD fourCC, DWORD& outSize, DWORD& outPos);
    HRESULT ReadChunkData(const BYTE* data, DWORD pos, void* out, DWORD bytes);

    // grant factory access to internals
    friend class SoundEffectFactory;
};

//──────────────────────────────────────────────────────────────────────────────
//  SoundEffectFactory ‒ generate procedural SFX
//──────────────────────────────────────────────────────────────────────────────
class SoundEffectFactory
{
public:
    // Simple procedural tones
    static std::unique_ptr<SoundEffect> CreateBeep(float freq = 440.f, float dur = 0.5f);
    static std::unique_ptr<SoundEffect> CreateSine(float freq = 440.f, float dur = 1.0f);
    static std::unique_ptr<SoundEffect> CreateNoise(float dur = 1.0f);

    // Typical game sounds
    static std::unique_ptr<SoundEffect> CreateGunshot();
    static std::unique_ptr<SoundEffect> CreateExplosion();
    static std::unique_ptr<SoundEffect> CreateFootstep();
    static std::unique_ptr<SoundEffect> CreateReload();
    static std::unique_ptr<SoundEffect> CreatePickup();

private:
    static void  GenerateWaveform(std::vector<short>& dst,
        float freq, float dur,
        float (*wave)(float));
    static std::unique_ptr<SoundEffect>
        CreateFromSamples(const std::vector<short>& samples,
            DWORD sampleRate = 44100);

    // basic waves
    static float SineWave(float phase);
    static float NoiseWave(float);      // phase ignored
};