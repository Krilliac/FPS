#pragma once

#include "Utils/Assert.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

//------------------------------------------------------------------------------
//  Model ‒ loads a simple OBJ file with tinyobjloader and creates GPU buffers
//------------------------------------------------------------------------------
class Model
{
public:
    /**
     * @brief Default constructor
     */
    Model() = default;

    /**
     * @brief Destructor - cleans up DirectX resources
     */
    ~Model();

    // Load OBJ from wide-string path and build vertex / index buffers.
    //   - filename : full or relative path to .obj (UTF-16 wide string)
    //   - device   : valid ID3D11Device*
    HRESULT LoadObj(const std::wstring& filename, ID3D11Device* device);

    // Issue draw call for the model (assumes shaders / CBs already bound)
    void Render(ID3D11DeviceContext* ctx);

private:
    ID3D11Buffer* m_vb{ nullptr };
    ID3D11Buffer* m_ib{ nullptr };
    UINT          m_indexCount{ 0 };
};