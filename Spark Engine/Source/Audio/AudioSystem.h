#pragma once
#include "../Core/framework.h"

namespace SparkEngine {
    class AudioSystem {
    public:
        AudioSystem();
        ~AudioSystem();

        bool Initialize();
        void Shutdown();
        void Update();

        bool PlaySound(const std::string& assetName, bool loop = false);
        void StopAll();

    private:
        ComPtr<IXAudio2> m_xaudio;
        ComPtr<IXAudio2MasteringVoice> m_masterVoice;
        std::vector<IXAudio2SourceVoice*> m_activeVoices;
    };
}
