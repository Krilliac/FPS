#include "AudioSystem.h"
#include "../Utils/Logger.h"
#include "../Assets/AssetManager.h"

namespace SparkEngine {
    AudioSystem::AudioSystem() {}
    AudioSystem::~AudioSystem() { Shutdown(); }

    bool AudioSystem::Initialize() {
        Logger::Info("Initializing AudioSystem...");
        if (FAILED(XAudio2Create(m_xaudio.GetAddressOf()))) {
            Logger::Error("Failed to create XAudio2 engine");
            return false;
        }
        if (FAILED(m_xaudio->CreateMasteringVoice(m_masterVoice.GetAddressOf()))) {
            Logger::Error("Failed to create mastering voice");
            return false;
        }
        Logger::Info("AudioSystem initialized");
        return true;
    }

    void AudioSystem::Shutdown() {
        Logger::Info("Shutting down AudioSystem...");
        StopAll();
        if (m_masterVoice) m_masterVoice->DestroyVoice();
        if (m_xaudio) m_xaudio.Reset();
        Logger::Info("AudioSystem shutdown complete");
    }

    void AudioSystem::Update() {
        m_activeVoices.erase(
            std::remove_if(m_activeVoices.begin(), m_activeVoices.end(), [](IXAudio2SourceVoice* v) {
                XAUDIO2_VOICE_STATE state; v->GetState(&state); return state.BuffersQueued==0;
            }),
            m_activeVoices.end());
    }

    bool AudioSystem::PlaySound(const std::string& assetName, bool loop) {
        auto* asset = Engine::GetInstance().GetAssets().GetAsset<SoundAsset>(assetName);
        if (!asset) { Logger::Warning("Sound asset not found: "+assetName); return false; }
        IXAudio2SourceVoice* voice=nullptr;
        if (FAILED(m_xaudio->CreateSourceVoice(&voice, asset->GetFormat()))) {
            Logger::Error("Failed to create source voice"); return false;
        }
        XAUDIO2_BUFFER buffer={};
        buffer.AudioBytes=asset->GetSize(); buffer.pAudioData=asset->GetData();
        buffer.Flags=XAUDIO2_END_OF_STREAM;
        if(loop) buffer.LoopCount=XAUDIO2_LOOP_INFINITE;
        if (FAILED(voice->SubmitSourceBuffer(&buffer))) { voice->DestroyVoice(); return false; }
        if (FAILED(voice->Start())) { voice->DestroyVoice(); return false; }
        m_activeVoices.push_back(voice);
        return true;
    }

    void AudioSystem::StopAll() {
        for (auto v: m_activeVoices) { v->Stop(); v->DestroyVoice(); }
        m_activeVoices.clear();
    }
}
