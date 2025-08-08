#include "SceneManager.h"
#include "Game/GameObject.h"
#include "Game/CubeObject.h"
#include "Game/PlaneObject.h"
#include "Game/SphereObject.h"
#include "Game/PlaceholderMesh.h"
#include "Utils/Assert.h"

#include <fstream>
#include <sstream>
#include "PyramidObject.h"
#include "RampObject.h"
#include "WallObject.h"

using DirectX::XMFLOAT3;

SceneManager::SceneManager(GraphicsEngine* graphics, InputManager* input)
    : m_graphics(graphics), m_input(input)
{
    ASSERT_MSG(graphics != nullptr, "SceneManager: graphics is null");
    ASSERT_MSG(input != nullptr, "SceneManager: input is null");
}

const std::vector<std::unique_ptr<GameObject>>& SceneManager::GetObjects() const
{
    return m_objects;
}

bool SceneManager::LoadScene(const std::wstring& filepath)
{
    if (std::filesystem::path(filepath).extension() == L".scene")
        return LoadCustom(filepath);
    return false;
}

bool SceneManager::LoadCustom(const std::wstring& path)
{
    std::wifstream file(path);
    if (!file.is_open())
        return false;

    std::wstring line;
    while (std::getline(file, line))
    {
        // Skip empty lines and comments
        if (line.empty() || line[0] == L'#')
            continue;

        std::wstringstream ss(line);
        std::wstring type;
        ss >> type;

        float x = 0, y = 0, z = 0;
        ss >> x >> y >> z;

        std::unique_ptr<GameObject> obj;

        if (type == L"Cube")
        {
            float size;
            ss >> size;
            obj = std::make_unique<CubeObject>(size);
        }
        else if (type == L"Plane")
        {
            float width, depth;
            ss >> width >> depth;
            obj = std::make_unique<PlaneObject>(width, depth);
        }
        else if (type == L"Sphere")
        {
            float radius;
            int slices, stacks;
            ss >> radius >> slices >> stacks;
            obj = std::make_unique<SphereObject>(radius, slices, stacks);
        }
        else if (type == L"Pyramid")
        {
            float size;
            ss >> size;
            obj = std::make_unique<PyramidObject>(size);
        }
        else if (type == L"Ramp")
        {
            float length, height;
            ss >> length >> height;
            obj = std::make_unique<RampObject>(length, height);
        }
        else if (type == L"Wall")
        {
            float width, height;
            ss >> width >> height;
            obj = std::make_unique<WallObject>(width, height);
        }
        else
        {
            // Unknown type
            continue;
        }

        ASSERT(obj);
        // Initialize object (creates its mesh)
        HRESULT hr = obj->Initialize(
            m_graphics->GetDevice(),
            m_graphics->GetContext());
        ASSERT_MSG(SUCCEEDED(hr), "SceneManager: object Initialize failed");

        // Load mesh from .obj via PlaceholderMesh
        LoadOrPlaceholderMesh(
            *obj->GetMesh(),
            m_graphics->GetDevice(),
            m_graphics->GetContext(),
            L"Assets\\Models\\" + type + L".obj");

        obj->SetPosition({ x, y, z });
        m_objects.push_back(std::move(obj));
    }

    return !m_objects.empty();
}