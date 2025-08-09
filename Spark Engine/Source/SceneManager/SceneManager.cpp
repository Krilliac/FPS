#include "SceneManager.h"
#include "Game/GameObject.h"
#include "Game/CubeObject.h"
#include "Game/PlaneObject.h"
#include "Game/SphereObject.h"
#include "Game/PlaceholderMesh.h"
#include "Graphics/GraphicsEngine.h"
#include "Utils/Assert.h"
#include "../Utils/ConsoleProcessManager.h"

#include <fstream>
#include <sstream>
#include "Game/PyramidObject.h"
#include "Game/RampObject.h"
#include "Game/WallObject.h"
#include <iomanip>

using DirectX::XMFLOAT3;

// **FIXED: Rate-limited logging for SceneManager to prevent console spam**
#define LOG_TO_CONSOLE_RATE_LIMITED(msg, type) \
    do { \
        static auto lastLogTime = std::chrono::steady_clock::now(); \
        static int logCounter = 0; \
        auto now = std::chrono::steady_clock::now(); \
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastLogTime).count(); \
        if (elapsed >= 10 || logCounter < 1) { \
            Spark::ConsoleProcessManager::GetInstance().Log(msg, type); \
            if (elapsed >= 10) { \
                lastLogTime = now; \
                logCounter = 0; \
            } else { \
                logCounter++; \
            } \
        } \
    } while(0)

// Use rate-limited logging for most messages, immediate for critical ones
#define LOG_TO_CONSOLE(msg, type) LOG_TO_CONSOLE_RATE_LIMITED(msg, type)
#define LOG_TO_CONSOLE_IMMEDIATE(msg, type) Spark::ConsoleProcessManager::GetInstance().Log(msg, type)

SceneManager::SceneManager(GraphicsEngine* graphics, InputManager* input)
    : m_graphics(graphics), m_input(input)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager constructed.", L"INFO");
    ASSERT_MSG(graphics != nullptr, "SceneManager: graphics is null");
    ASSERT_MSG(input != nullptr, "SceneManager: input is null");
}

const std::vector<std::unique_ptr<GameObject>>& SceneManager::GetObjects() const
{
    // **FIXED: Remove per-frame logging completely - this is called every frame during rendering**
    return m_objects;
}

bool SceneManager::LoadScene(const std::wstring& filepath)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager::LoadScene called. filepath=" + filepath, L"OPERATION");
    if (std::filesystem::path(filepath).extension() == L".scene") {
        bool loaded = LoadCustom(filepath);
        LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager::LoadCustom returned: " + std::wstring(loaded ? L"SUCCESS" : L"FAILURE"), L"INFO");
        LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager objects loaded: " + std::to_wstring(m_objects.size()), L"INFO");
        return loaded;
    }
    LOG_TO_CONSOLE_IMMEDIATE(L"Scene file extension not recognized: " + filepath, L"WARNING");
    return false;
}

bool SceneManager::LoadCustom(const std::wstring& path)
{
    LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager::LoadCustom called. path=" + path, L"OPERATION");
    if (!m_graphics || !m_graphics->GetDevice() || !m_graphics->GetContext()) {
        ASSERT_MSG(false, "SceneManager: Graphics device/context is null");
        LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Graphics device/context is null! Aborting scene load.", L"ERROR");
        return false;
    }
    std::wifstream file(path);
    if (!file.is_open()) {
        LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Could not open scene file: " + path, L"ERROR");
        return false;
    }
    std::wstring line;
    int lineNum = 0;
    while (std::getline(file, line))
    {
        ++lineNum;
        if (line.empty() || line[0] == L'#') {
            LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Skipping comment/empty line " + std::to_wstring(lineNum), L"INFO");
            continue;
        }
        std::wstringstream ss(line);
        std::wstring type;
        ss >> type;
        float x = 0, y = 0, z = 0;
        ss >> x >> y >> z;
        std::unique_ptr<GameObject> obj;
        LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Line " + std::to_wstring(lineNum) + L": type=" + type + L" pos=(" + std::to_wstring(x) + L"," + std::to_wstring(y) + L"," + std::to_wstring(z) + L")", L"INFO");

        if (type == L"Cube")
        {
            float size;
            ss >> size;
            obj = std::make_unique<CubeObject>(size);
            LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Creating CubeObject size=" + std::to_wstring(size), L"INFO");
        }
        else if (type == L"Plane")
        {
            float width, depth;
            ss >> width >> depth;
            obj = std::make_unique<PlaneObject>(width, depth);
            LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Creating PlaneObject width=" + std::to_wstring(width) + L" depth=" + std::to_wstring(depth), L"INFO");
        }
        else if (type == L"Sphere")
        {
            float radius;
            int slices, stacks;
            ss >> radius >> slices >> stacks;
            obj = std::make_unique<SphereObject>(radius, slices, stacks);
            LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Creating SphereObject radius=" + std::to_wstring(radius) + L" slices=" + std::to_wstring(slices) + L" stacks=" + std::to_wstring(stacks), L"INFO");
        }
        else if (type == L"Pyramid")
        {
            float size;
            ss >> size;
            obj = std::make_unique<PyramidObject>(size);
            LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Creating PyramidObject size=" + std::to_wstring(size), L"INFO");
        }
        else if (type == L"Ramp")
        {
            float length, height;
            ss >> length >> height;
            obj = std::make_unique<RampObject>(length, height);
            LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Creating RampObject length=" + std::to_wstring(length) + L" height=" + std::to_wstring(height), L"INFO");
        }
        else if (type == L"Wall")
        {
            float width, height;
            ss >> width >> height;
            obj = std::make_unique<WallObject>(width, height);
            LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Creating WallObject width=" + std::to_wstring(width) + L" height=" + std::to_wstring(height), L"INFO");
        }
        else
        {
            LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Unknown type on line " + std::to_wstring(lineNum) + L": " + type, L"ERROR");
            continue;
        }

        ASSERT(obj);
        HRESULT hr = obj->Initialize(m_graphics->GetDevice(), m_graphics->GetContext());
        LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Object Initialize HR=0x" + std::to_wstring(hr), L"INFO");
        ASSERT_MSG(SUCCEEDED(hr), "SceneManager: object Initialize failed");
        LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Loading mesh for type=" + type + L" from OBJ...", L"INFO");
        LoadOrPlaceholderMesh(*obj->GetMesh(), m_graphics->GetDevice(), m_graphics->GetContext(), L"Assets\\Models\\" + type + L".obj");
        LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Mesh loaded for type=" + type, L"INFO");
        obj->SetPosition({ x, y, z });
        LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: SetPosition to (" + std::to_wstring(x) + L"," + std::to_wstring(y) + L"," + std::to_wstring(z) + L")", L"INFO");
        m_objects.push_back(std::move(obj));
    }
    LOG_TO_CONSOLE_IMMEDIATE(L"SceneManager: Finished loading scene. Objects count: " + std::to_wstring(m_objects.size()), L"INFO");
    return !m_objects.empty();
}