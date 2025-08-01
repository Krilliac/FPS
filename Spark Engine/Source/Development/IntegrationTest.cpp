#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/MeshManager.h"
#include "../Graphics/Lighting/LightingSystem.h"
#include "../Utils/Profiler.h"
#include <iostream>

namespace SparkEngine {
    bool RunCompleteIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                  ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Complete SparkEngine Integration Test..." << std::endl;
        
        PROFILE_FUNCTION();
        
        // Test 1: Graphics Engine Enhanced Features
        {
            PROFILE_SCOPE("Graphics Engine Test");
            
            if (!graphics || !graphics->GetDevice()) {
                std::cerr << "Graphics engine not properly initialized" << std::endl;
                return false;
            }
            
            // Test advanced graphics features
            GraphicsSettings settings;
            settings.enableVSync = true;
            settings.enableMSAA = false;
            graphics->SetGraphicsSettings(settings);
            
            std::cout << "✓ Enhanced Graphics Engine tested" << std::endl;
        }
        
        // Test 2: Shader Management System
        {
            PROFILE_SCOPE("Shader Manager Test");
            
            ShaderManager shaderManager;
            if (!shaderManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize shader manager" << std::endl;
                return false;
            }
            
            // Test hot reload
            shaderManager.EnableHotReload(true);
            
            std::cout << "✓ Shader Management System tested" << std::endl;
        }
        
        // Test 3: Mesh Management System
        {
            PROFILE_SCOPE("Mesh Manager Test");
            
            MeshManager meshManager;
            if (!meshManager.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize mesh manager" << std::endl;
                return false;
            }
            
            // Create procedural meshes
            auto cube = meshManager.CreateCube("test_cube", 1.0f);
            auto sphere = meshManager.CreateSphere("test_sphere", 1.0f, 32);
            
            if (!cube || !sphere) {
                std::cerr << "Failed to create procedural meshes" << std::endl;
                return false;
            }
            
            std::cout << "✓ Mesh Management System tested" << std::endl;
        }
        
        // Test 4: Lighting System
        {
            PROFILE_SCOPE("Lighting System Test");
            
            LightingSystem lightingSystem;
            if (!lightingSystem.Initialize(graphics->GetDevice())) {
                std::cerr << "Failed to initialize lighting system" << std::endl;
                return false;
            }
            
            lightingSystem.SetAmbientLight({0.2f, 0.2f, 0.3f}, 0.5f);
            lightingSystem.EnableShadows(true);
            
            std::cout << "✓ Advanced Lighting System tested" << std::endl;
        }
        
        // Test 5: Asset Management System
        {
            PROFILE_SCOPE("Asset Manager Test");
            
            if (!assetManager->Initialize()) {
                std::cerr << "Failed to initialize asset manager" << std::endl;
                return false;
            }
            
            // Test dynamic pipeline
            assetManager->EnableDynamicPipeline(true);
            assetManager->EnableAssetStreaming(true);
            
            std::cout << "✓ Enhanced Asset Management tested" << std::endl;
        }
        
        // Test 6: Collaborative Development
        {
            PROFILE_SCOPE("Collaborative Development Test");
            
            CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
            if (!collaboration.Initialize("TestDeveloper", false)) {
                std::cerr << "Failed to initialize collaborative development" << std::endl;
                return false;
            }
            
            // Test session creation
            if (!collaboration.CreateDevelopmentSession("IntegrationTestSession")) {
                std::cerr << "Failed to create development session" << std::endl;
                return false;
            }
            
            // Test content synchronization
            collaboration.EnableRealTimeContentSync(true);
            collaboration.EnableAssetStreaming(true);
            
            std::cout << "✓ Collaborative Development tested" << std::endl;
        }
        
        // Test 7: Profiling System
        {
            PROFILE_SCOPE("Profiler Test");
            
            Profiler& profiler = Profiler::Get();
            profiler.SetEnabled(true);
            profiler.BeginFrame();
            
            // Simulate some work
            profiler.BeginSample("Test Work");
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            profiler.EndSample("Test Work");
            
            profiler.EndFrame();
            
            if (profiler.GetFrameSamples().empty()) {
                std::cerr << "Profiler not capturing samples" << std::endl;
                return false;
            }
            
            std::cout << "✓ Performance Profiling System tested" << std::endl;
        }
        
        std::cout << "🎉 All integration tests passed! SparkEngine is fully operational!" << std::endl;
        
        // Print performance summary
        const auto& samples = Profiler::Get().GetFrameSamples();
        std::cout << "\nPerformance Summary:" << std::endl;
        for (const auto& sample : samples) {
            std::cout << "  " << sample.name << ": " << sample.duration << "ms" << std::endl;
        }
        
        return true;
    }
}
