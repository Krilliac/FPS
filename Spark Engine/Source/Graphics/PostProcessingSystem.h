/**
 * @file PostProcessingSystem.h
 * @brief Basic post-processing system stub for Spark Engine
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

#include "Utils/Assert.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <string>
#include <memory>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

/**
 * @brief Basic post-processing system
 */
class PostProcessingSystem
{
public:
    PostProcessingSystem();
    ~PostProcessingSystem();

    /**
     * @brief Initialize the post-processing system
     */
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

    /**
     * @brief Shutdown the post-processing system
     */
    void Shutdown();

    /**
     * @brief Update the system
     */
    void Update(float deltaTime);
    
    /**
     * @brief Console integration - Set exposure
     */
    void Console_SetExposure(float exposure);
    
    /**
     * @brief Console integration - List effects
     */
    std::string Console_ListEffects() const;

private:
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;
};