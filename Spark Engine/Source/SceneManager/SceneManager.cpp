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
#include <iomanip>

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
    std::wcerr << L"[SceneManager] Attempting to load scene: " << filepath << std::endl;
    if (std::filesystem::path(filepath).extension() == L".scene") {
        bool loaded = LoadCustom(filepath);
        std::wcerr << L"[SceneManager] LoadCustom returned: " << (loaded ? L"SUCCESS" : L"FAILURE") << std::endl;
        std::wcerr << L"[SceneManager] Objects loaded: " << m_objects.size() << std::endl;
        return loaded;
    }
    std::wcerr << L"[SceneManager] Scene file extension not recognized: " << filepath << std::endl;
    return false;
}

bool SceneManager::LoadCustom(const std::wstring& path)
{
    // Ensure device/context are valid before loading scene
    if (!m_graphics || !m_graphics->GetDevice() || !m_graphics->GetContext()) {
        ASSERT_MSG(false, "SceneManager: Graphics device/context is null");
        std::wcerr << L"[ERROR] SceneManager: Graphics device/context is null! Aborting scene load." << std::endl;
        return false;
    }

    std::wifstream file(path);
    if (!file.is_open()) {
        std::wcerr << L"[ERROR] SceneManager: Could not open scene file: " << path << std::endl;
        return false;
    }

    std::wstring line;
    int lineNum = 0;
    while (std::getline(file, line))
    {
        ++lineNum;
        // Skip empty lines and comments
        if (line.empty() || line[0] == L'#') {
            std::wcerr << L"[SceneManager] Skipping comment/empty line " << lineNum << std::endl;
            continue;
        }

        std::wstringstream ss(line);
        std::wstring type;
        ss >> type;

        float x = 0, y = 0, z = 0;
        ss >> x >> y >> z;

        std::unique_ptr<GameObject> obj;
        std::wcerr << L"[SceneManager] Line " << lineNum << L": type=" << type << L" pos=(" << x << L"," << y << L"," << z << L")" << std::endl;

        if (type == L"Cube")
        {
            float size;
            ss >> size;
            obj = std::make_unique<CubeObject>(size);
            std::wcerr << L"[SceneManager] Creating CubeObject size=" << size << std::endl;
        }
        else if (type == L"Plane")
        {
            float width, depth;
            ss >> width >> depth;
            obj = std::make_unique<PlaneObject>(width, depth);
            std::wcerr << L"[SceneManager] Creating PlaneObject width=" << width << " depth=" << depth << std::endl;
        }
        else if (type == L"Sphere")
        {
            float radius;
            int slices, stacks;
            ss >> radius >> slices >> stacks;
            obj = std::make_unique<SphereObject>(radius, slices, stacks);
            std::wcerr << L"[SceneManager] Creating SphereObject radius=" << radius << " slices=" << slices << " stacks=" << stacks << std::endl;
        }
        else if (type == L"Pyramid")
        {
            float size;
            ss >> size;
            obj = std::make_unique<PyramidObject>(size);
            std::wcerr << L"[SceneManager] Creating PyramidObject size=" << size << std::endl;
        }
        else if (type == L"Ramp")
        {
            float length, height;
            ss >> length >> height;
            obj = std::make_unique<RampObject>(length, height);
            std::wcerr << L"[SceneManager] Creating RampObject length=" << length << " height=" << height << std::endl;
        }
        else if (type == L"Wall")
        {
            float width, height;
            ss >> width >> height;
            obj = std::make_unique<WallObject>(width, height);
            std::wcerr << L"[SceneManager] Creating WallObject width=" << width << " height=" << height << std::endl;
        }
        else
        {
            std::wcerr << L"[SceneManager] Unknown type on line " << lineNum << L": " << type << std::endl;
            continue;
        }

        ASSERT(obj);
        // Initialize object (creates its mesh)
        HRESULT hr = obj->Initialize(
            m_graphics->GetDevice(),
            m_graphics->GetContext());
        std::wcerr << L"[SceneManager] Object Initialize HR=0x" << std::hex << hr << std::dec << std::endl;
        ASSERT_MSG(SUCCEEDED(hr), "SceneManager: object Initialize failed");

        // Load mesh from .obj via PlaceholderMesh
        std::wcerr << L"[SceneManager] Loading mesh for type=" << type << L" from OBJ..." << std::endl;
        LoadOrPlaceholderMesh(
            *obj->GetMesh(),
            m_graphics->GetDevice(),
            m_graphics->GetContext(),
            L"Assets\\Models\\" + type + L".obj");
        std::wcerr << L"[SceneManager] Mesh loaded for type=" << type << std::endl;

        obj->SetPosition({ x, y, z });
        std::wcerr << L"[SceneManager] SetPosition to (" << x << L"," << y << L"," << z << L")" << std::endl;
        m_objects.push_back(std::move(obj));
    }

    std::wcerr << L"[SceneManager] Finished loading scene. Objects count: " << m_objects.size() << std::endl;
    return !m_objects.empty();
}