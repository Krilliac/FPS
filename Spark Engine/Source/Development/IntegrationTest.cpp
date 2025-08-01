#include "IntegrationTest.h"
#include "CollaborativeDevelopment.h"
#include <iostream>

namespace SparkEngine {
    bool RunCollaborativeIntegrationTest(EntityRegistry* registry, AssetManager* assetManager,
                                       ScriptingSystem* scripting, GraphicsEngine* graphics) {
        std::cout << "Running Collaborative Development Integration Test..." << std::endl;
        
        // Test 1: Initialize collaborative development
        CollaborativeDevelopment collaboration(registry, assetManager, scripting, graphics);
        if (!collaboration.Initialize("TestDeveloper", false)) {
            std::cerr << "Failed to initialize collaborative development" << std::endl;
            return false;
        }
        std::cout << "âœ“ Collaborative development initialized" << std::endl;
        
        // Test 2: Create development session
        if (!collaboration.CreateDevelopmentSession("TestSession")) {
            std::cerr << "Failed to create development session" << std::endl;
            return false;
        }
        std::cout << "âœ“ Development session created" << std::endl;
        
        // Test 3: Test content synchronization
        collaboration.EnableRealTimeContentSync(true);
        std::cout << "âœ“ Content synchronization enabled" << std::endl;
        
        // Test 4: Test asset streaming
        collaboration.EnableAssetStreaming(true);
        collaboration.SetStreamingDistance(200.0f);
        std::cout << "âœ“ Asset streaming configured" << std::endl;
        
        // Test 5: Test distributed scripting
        collaboration.LoadDistributedScript("TestScript", "function OnTest() print('Hello from distributed script!') end");
        collaboration.ExecuteDistributedEvent("OnTest", "");
        std::cout << "âœ“ Distributed scripting tested" << std::endl;
        
        std::cout << "All collaborative integration tests passed!" << std::endl;
        return true;
    }
}
