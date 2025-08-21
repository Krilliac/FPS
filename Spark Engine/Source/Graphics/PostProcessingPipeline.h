/**
 * @file PostProcessingPipeline.h
 * @brief Post-processing effects pipeline
 * @author Spark Engine Team
 * @date 2025
 */

#pragma once

#include <memory>

/**
 * @brief Post-processing effects pipeline system
 */
class PostProcessingPipeline
{
public:
    PostProcessingPipeline() = default;
    ~PostProcessingPipeline() = default;

    /**
     * @brief Initialize post-processing pipeline
     */
    bool Initialize() { return true; }

    /**
     * @brief Shutdown post-processing pipeline
     */
    void Shutdown() {}

    /**
     * @brief Apply post-processing effects
     */
    void Process() {}

    /**
     * @brief Render post-processing effects
     */
    void Render() {}
};