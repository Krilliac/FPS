/**
 * @file Shader.h
 * @brief HLSL shader management and constant buffer handling
 * @author Spark Engine Team
 * @date 2024
 * 
 * This class provides a comprehensive shader management system for DirectX 11,
 * handling vertex and pixel shader compilation, loading, and constant buffer
 * updates for rendering operations in the Spark Engine.
 */

#pragma once

#include "Utils/Assert.h"
#include "..\Core\framework.h"      // ConstantBuffer definition
#include <d3d11.h>
#include <string>

/**
 * @brief Constant buffer structure for shader matrix transformations
 * 
 * Contains the transformation matrices required for vertex shader operations.
 * This structure is mapped directly to the constant buffer in HLSL shaders.
 */
struct ConstantBuffer
{
    DirectX::XMMATRIX World;      ///< World transformation matrix
    DirectX::XMMATRIX View;       ///< View transformation matrix (camera)
    DirectX::XMMATRIX Projection; ///< Projection transformation matrix
};

/**
 * @brief HLSL shader management class
 * 
 * Handles loading, compilation, and management of vertex and pixel shaders.
 * Also manages constant buffer creation and updates for shader parameters.
 * Provides a clean interface for shader operations within the graphics pipeline.
 * 
 * @note Shaders are compiled at runtime from HLSL source files
 * @warning Ensure Initialize() is called before loading any shaders
 */
class Shader
{
public:
    /**
     * @brief Default constructor
     * 
     * Initializes member variables to safe default values.
     */
    Shader();

    /**
     * @brief Destructor
     * 
     * Automatically calls Shutdown() to ensure proper resource cleanup.
     */
    ~Shader();

    /**
     * @brief Initialize the shader system
     * 
     * Sets up the shader manager with DirectX device and context references.
     * Creates the constant buffer for matrix transformations.
     * 
     * @param device DirectX 11 device for resource creation
     * @param context DirectX 11 device context for rendering operations
     * @return HRESULT indicating success or failure of initialization
     */
    HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

    /**
     * @brief Clean up all shader resources
     * 
     * Releases all shader interfaces and buffers. Safe to call multiple times.
     */
    void    Shutdown();

    /**
     * @brief Load and compile a vertex shader from file
     * 
     * Compiles an HLSL vertex shader from the specified file and creates
     * the corresponding input layout for vertex data.
     * 
     * @param filename Path to the HLSL vertex shader file
     * @return HRESULT indicating success or failure of shader loading
     * @note The shader must have a "main" entry point
     */
    HRESULT LoadVertexShader(const std::wstring& filename);

    /**
     * @brief Load and compile a pixel shader from file
     * 
     * Compiles an HLSL pixel shader from the specified file.
     * 
     * @param filename Path to the HLSL pixel shader file
     * @return HRESULT indicating success or failure of shader loading
     * @note The shader must have a "main" entry point
     */
    HRESULT LoadPixelShader(const std::wstring& filename);

    /**
     * @brief Bind shaders to the graphics pipeline
     * 
     * Sets the loaded vertex and pixel shaders as active in the rendering pipeline.
     * Also sets the input layout for vertex data interpretation.
     */
    void SetShaders();

    /**
     * @brief Update the constant buffer with new transformation matrices
     * 
     * Updates the shader constant buffer with the provided world, view, and
     * projection matrices for the current frame.
     * 
     * @param cb ConstantBuffer structure containing transformation matrices
     */
    void UpdateConstantBuffer(const ConstantBuffer& cb);

    /**
     * @brief Compile a shader from an HLSL source file
     * 
     * Static utility function for compiling HLSL shaders from file.
     * Used internally but exposed for external shader compilation needs.
     * 
     * @param filename Path to the HLSL shader file
     * @param entryPoint Entry point function name (typically "main")
     * @param shaderModel Shader model version (e.g., "vs_5_0", "ps_5_0")
     * @param blobOut Output parameter for compiled shader bytecode
     * @return HRESULT indicating success or failure of compilation
     */
    static HRESULT CompileShaderFromFile(const std::wstring& filename,
        const std::string& entryPoint,
        const std::string& shaderModel,
        ID3DBlob** blobOut);

private:
    /**
     * @brief Create the constant buffer for transformation matrices
     * @return HRESULT indicating success or failure of buffer creation
     */
    HRESULT CreateConstantBuffer();

    ID3D11Device* m_device{ nullptr };           ///< DirectX 11 device reference
    ID3D11DeviceContext* m_context{ nullptr };   ///< DirectX 11 context reference
    ID3D11VertexShader* m_vertexShader{ nullptr }; ///< Loaded vertex shader
    ID3D11PixelShader* m_pixelShader{ nullptr };  ///< Loaded pixel shader
    ID3D11InputLayout* m_inputLayout{ nullptr };  ///< Vertex input layout
    ID3D11Buffer* m_constantBuffer{ nullptr };    ///< Constant buffer for transformations
};