# SparkEngine Documentation System

A comprehensive wiki and API documentation system that automatically extracts documentation from header files and creates a beautiful, searchable wiki.

## 🌟 Features

- **Automatic Documentation Generation**: Extracts API documentation from C++ header files using Doxygen
- **Beautiful Wiki Interface**: Clean, modern web interface with responsive design
- **Auto-Update System**: Monitors source files for changes and regenerates documentation automatically
- **Module Organization**: Organizes documentation by engine modules (Audio, Graphics, Core, etc.)
- **Searchable API Reference**: Full-text search across all classes, functions, and documentation
- **Class Diagrams & Graphs**: Visual representations of class hierarchies and dependencies
- **Source Code Browser**: Syntax-highlighted source code with cross-references

## 📁 Generated Documentation Structure

```
docs/
├── Doxyfile                    # Doxygen configuration
├── generate-docs.sh           # Main documentation generation script
├── auto-update.sh            # Auto-update monitoring script
├── README.md                 # This file
├── output/                   # Doxygen output (HTML, diagrams, etc.)
│   └── html/                # Generated HTML documentation
└── wiki/                    # Wiki website
    ├── index.html           # Main wiki homepage
    ├── audio.html           # Audio module page
    ├── graphics.html        # Graphics module page
    ├── core.html            # Core module page
    └── output/              # Copy of Doxygen output for wiki
        └── html/            # Full API documentation
```

## 🚀 Quick Start

### Generate Documentation

```bash
# Navigate to docs directory
cd docs/

# Generate documentation (one-time)
./generate-docs.sh

# View the documentation
open wiki/index.html
# or serve with HTTP server:
cd wiki && python3 -m http.server 8000
# then visit http://localhost:8000
```

### Auto-Update System

```bash
# Start continuous monitoring (runs in background)
./auto-update.sh monitor

# Check once for changes and update if needed
./auto-update.sh check

# Force regenerate documentation
./auto-update.sh force

# Show current status
./auto-update.sh status
```

## 📚 Documentation Features

### Main Wiki Homepage

The main wiki homepage (`wiki/index.html`) provides:

- **Beautiful Overview**: Modern design with gradient backgrounds and card-based navigation
- **Quick Navigation**: Direct links to different sections of documentation
- **Feature Highlights**: Overview of engine capabilities and modules
- **Auto-Update Info**: Information about the documentation update system

### API Documentation

The full API documentation includes:

- **Class Reference**: Complete list of all classes with detailed documentation
- **Module Organization**: Documentation organized by engine modules
- **Function Documentation**: Detailed parameter and return value information
- **Source Code**: Syntax-highlighted source code with cross-references
- **Diagrams**: Class inheritance diagrams and collaboration graphs
- **Search Function**: Real-time search across all documentation

### Supported Engine Modules

The documentation system automatically extracts and organizes documentation for:

- **Audio System**: XAudio2-based 3D audio engine with console integration
- **Graphics Engine**: DirectX 11 rendering pipeline with shaders and effects
- **Core Systems**: Main engine framework and global declarations
- **Input Management**: Keyboard and mouse input handling
- **Physics System**: Collision detection and rigid body physics
- **Camera System**: First-person camera with smooth controls
- **Game Framework**: Game objects and scene management
- **Editor Integration**: ImGui-based visual editor components
- **Utility Systems**: Timers, logging, crash handling, and helper functions

## 🔧 Configuration

### Doxygen Configuration

The `Doxyfile` is configured for optimal documentation generation:

- **Input Sources**: Automatically scans `Spark Engine/Source/` and `SparkEditor/Source/`
- **Output Format**: HTML with search functionality and responsive design
- **Documentation Style**: Supports Doxygen-style comments (`/** */`) with `@brief`, `@param`, `@return`
- **Diagrams**: Generates class diagrams, collaboration graphs, and include dependencies
- **Source Browser**: Includes source code browser with syntax highlighting

### Auto-Update Configuration

The auto-update system monitors:

- All `.h` and `.hpp` files in `Spark Engine/Source/`
- All `.h` and `.hpp` files in `SparkEditor/Source/`
- Checks for changes every 30 seconds when monitoring
- Uses MD5 checksums to detect file modifications
- Includes lock file mechanism to prevent concurrent updates

## 📖 Writing Documentation

### Documentation Style

The engine uses Doxygen-style documentation comments:

```cpp
/**
 * @file AudioEngine.h
 * @brief XAudio2-based audio engine with comprehensive console integration
 * @author Spark Engine Team
 * @date 2025
 * 
 * This class provides a comprehensive audio system built on XAudio2, supporting
 * both 2D and 3D audio playback, sound effect management, volume controls, and
 * an object pool system for efficient audio source management.
 */

/**
 * @brief Main audio engine class with comprehensive console integration
 * 
 * The AudioEngine class manages all audio operations for the Spark Engine using
 * XAudio2 as the underlying audio API. Features include:
 * - XAudio2-based audio playback with hardware acceleration
 * - 2D and 3D spatial audio positioning
 * - Sound effect loading and management
 * 
 * @note The engine uses an object pool to efficiently reuse audio sources
 * @warning Initialize() must be called before any audio operations
 */
class AudioEngine
{
public:
    /**
     * @brief Initialize the audio engine with XAudio2
     * 
     * Sets up the XAudio2 engine, creates the mastering voice, and initializes
     * the audio source object pool with the specified number of sources.
     * 
     * @param maxSources Maximum number of simultaneous audio sources
     * @return HRESULT indicating success or failure of audio initialization
     * @note A typical value for maxSources is 32-64 for most games
     */
    HRESULT Initialize(size_t maxSources);
};
```

### Documentation Tags

Supported Doxygen tags:

- `@file` - File description
- `@brief` - Brief description
- `@param` - Parameter description
- `@return` - Return value description
- `@note` - Additional notes
- `@warning` - Warnings and cautions
- `@see` - Cross-references
- `@example` - Usage examples
- `@todo` - Todo items
- `@bug` - Known bugs
- `@deprecated` - Deprecated features

## 🛠️ Maintenance

### Updating Documentation

The documentation is automatically updated when:

1. **File Changes**: Any header file modification triggers regeneration
2. **Manual Update**: Running `./generate-docs.sh` manually
3. **Forced Update**: Using `./auto-update.sh force`

### Troubleshooting

Common issues and solutions:

1. **Missing Dependencies**:
   ```bash
   sudo apt install doxygen graphviz
   ```

2. **Permission Issues**:
   ```bash
   chmod +x generate-docs.sh auto-update.sh
   ```

3. **Path Issues**: Ensure scripts are run from the `docs/` directory

4. **Build Errors**: Check that header files are properly formatted with valid C++ syntax

### Performance Considerations

- **Generation Time**: Large codebases may take 1-2 minutes to generate
- **File Monitoring**: Auto-update checks files every 30 seconds
- **Memory Usage**: Doxygen may use significant memory for large projects
- **Diagram Generation**: GraphViz diagrams increase generation time but improve documentation quality

## 🎯 Integration

### Build System Integration

To integrate with the existing build system, add to your `CMakeLists.txt`:

```cmake
# Add documentation target
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/docs/generate-docs.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()
```

### CI/CD Integration

For GitHub Actions or other CI systems:

```yaml
- name: Generate Documentation
  run: |
    sudo apt install doxygen graphviz
    cd docs
    ./generate-docs.sh
    
- name: Deploy to GitHub Pages
  uses: peaceiris/actions-gh-pages@v3
  with:
    github_token: ${{ secrets.GITHUB_TOKEN }}
    publish_dir: ./docs/wiki
```

## 📄 License

This documentation system is part of the SparkEngine project and follows the same MIT license.