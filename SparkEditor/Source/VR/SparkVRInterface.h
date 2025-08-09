/**
 * @file SparkVRInterface_stub.h
 * @brief Stub implementation of VR Interface system to fix build issues
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

#include <string>
#include <vector>
#include <functional>

namespace SparkEditor {

/**
 * @brief VR Interface system stub (simplified to avoid compilation issues)
 */
class SparkVRInterface {
public:
    /**
     * @brief Constructor
     */
    SparkVRInterface() = default;

    /**
     * @brief Destructor
     */
    ~SparkVRInterface() = default;

    /**
     * @brief Initialize VR system (stub)
     */
    bool Initialize() {
        return false; // VR not available in stub
    }

    /**
     * @brief Shutdown VR system (stub)
     */
    void Shutdown() {
        // Stub implementation
    }

    /**
     * @brief Check if VR is available (stub)
     */
    bool IsVRAvailable() const {
        return false;
    }
};

} // namespace SparkEditor