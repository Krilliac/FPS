# SparkEngine COMPLETE Submodule Setup Script with Fixed Array Handling
# Sets up ALL dependencies for the full unified system
Write-Host "=== SPARKENGINE COMPLETE SUBMODULE SETUP ===" -ForegroundColor Cyan

# Global tracking arrays
$global:setupResults = @()
$global:failureDetails = @()

# Function to safely add submodule with detailed failure tracking
function Add-SafeSubmodule {
    param(
        [string[]]$Urls,
        [string]$Path,
        [string]$Name,
        [string]$Branch = "master",
        [string]$Category = "Unknown"
    )
    
    Write-Host "`nSetting up $Name..." -ForegroundColor Yellow
    
    $result = [PSCustomObject]@{
        Name = $Name
        Category = $Category
        Path = $Path
        Success = $false
        Method = ""
        URL = ""
        Error = ""
        Warnings = @()
    }
    
    if (Test-Path $Path) {
        Write-Host "$Name directory already exists" -ForegroundColor Yellow
        $result.Warnings += "Directory already existed"
        
        # Check if it's a git repository
        if (Test-Path "$Path/.git") {
            Write-Host "$Name appears to be a git repository" -ForegroundColor Green
            $result.Success = $true
            $result.Method = "Already exists (git repo)"
            $global:setupResults += $result
            return $true
        } else {
            Write-Host "$Name directory exists but is not a git repository" -ForegroundColor Yellow
            $response = Read-Host "Remove existing directory and clone fresh? (y/n)"
            if ($response -eq "y" -or $response -eq "Y") {
                try {
                    Remove-Item -Recurse -Force $Path
                    $result.Warnings += "Removed existing non-git directory"
                } catch {
                    $result.Error = "Failed to remove existing directory: $($_.Exception.Message)"
                    $result.Method = "Directory removal failed"
                    $global:setupResults += $result
                    $global:failureDetails += $result
                    return $false
                }
            } else {
                $result.Error = "User chose not to remove existing directory"
                $result.Method = "User cancelled"
                $global:setupResults += $result
                $global:failureDetails += $result
                return $false
            }
        }
    }
    
    $lastError = ""
    $attemptCount = 0
    
    foreach ($url in $Urls) {
        $attemptCount++
        Write-Host "Trying URL $attemptCount/$($Urls.Count): $url" -ForegroundColor Gray
        
        try {
            # Try to add as submodule first
            Write-Host "  Attempting submodule add..." -ForegroundColor Gray
            $submoduleOutput = & git submodule add --branch $Branch $url $Path 2>&1
            $submoduleExitCode = $LASTEXITCODE
            
            if ($submoduleExitCode -eq 0) {
                Write-Host "✓ $Name added as submodule from $url" -ForegroundColor Green
                $result.Success = $true
                $result.Method = "Git submodule"
                $result.URL = $url
                $global:setupResults += $result
                return $true
            } else {
                $lastError = "Submodule failed: $submoduleOutput"
                Write-Host "  Submodule add failed: $submoduleOutput" -ForegroundColor Red
                Write-Host "  Trying direct clone..." -ForegroundColor Yellow
                
                # If submodule fails, try direct clone
                $cloneOutput = & git clone --branch $Branch $url $Path 2>&1
                $cloneExitCode = $LASTEXITCODE
                
                if ($cloneExitCode -eq 0) {
                    Write-Host "✓ $Name cloned directly from $url" -ForegroundColor Green
                    $result.Success = $true
                    $result.Method = "Direct git clone"
                    $result.URL = $url
                    $result.Warnings += "Submodule add failed, used direct clone"
                    $global:setupResults += $result
                    return $true
                } else {
                    $lastError = "Clone failed: $cloneOutput"
                    Write-Host "  Clone failed: $cloneOutput" -ForegroundColor Red
                }
            }
        } catch {
            $lastError = "Exception: $($_.Exception.Message)"
            Write-Host "  Exception with URL $url`: $($_.Exception.Message)" -ForegroundColor Red
        }
        
        # If this wasn't the last URL, add separator
        if ($attemptCount -lt $Urls.Count) {
            Write-Host "  Trying next URL..." -ForegroundColor Gray
        }
    }
    
    # All URLs failed
    $result.Error = $lastError
    $result.Method = "All URLs failed"
    $global:setupResults += $result
    $global:failureDetails += $result
    
    Write-Host "✗ Failed to setup $Name from any URL" -ForegroundColor Red
    Write-Host "  Last error: $lastError" -ForegroundColor Red
    return $false
}

# Function to create CMakeLists.txt for libraries that don't have one
function Create-CMakeForLibrary {
    param(
        [string]$Path,
        [string]$LibraryName,
        [string]$CMakeContent
    )
    
    $cmakePath = Join-Path $Path "CMakeLists.txt"
    if (-not (Test-Path $cmakePath)) {
        try {
            Set-Content $cmakePath $CMakeContent -Encoding UTF8
            Write-Host "Created CMakeLists.txt for $LibraryName" -ForegroundColor Green
        } catch {
            Write-Host "Failed to create CMakeLists.txt for $LibraryName`: $($_.Exception.Message)" -ForegroundColor Red
        }
    }
}

# Function to count successful results by category
function Get-CategorySuccessCount {
    param([string]$CategoryName)
    return ($global:setupResults | Where-Object { $_.Category -eq $CategoryName -and $_.Success }).Count
}

# Function to count total results by category
function Get-CategoryTotalCount {
    param([string]$CategoryName)
    return ($global:setupResults | Where-Object { $_.Category -eq $CategoryName }).Count
}

# Check git availability and configuration
Write-Host "`nChecking git configuration..." -ForegroundColor Yellow
try {
    $gitVersion = & git --version 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Git available: $gitVersion" -ForegroundColor Green
    } else {
        Write-Host "✗ Git not available or not working" -ForegroundColor Red
        Write-Host "Please install Git and ensure it's in your PATH" -ForegroundColor Red
        exit 1
    }
} catch {
    Write-Host "✗ Git not found: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

# Check if we're in a git repository
if (-not (Test-Path ".git")) {
    Write-Host "Not in a git repository. Initializing..." -ForegroundColor Yellow
    try {
        & git init
        & git config core.autocrlf false  # Prevent line ending issues
        Write-Host "✓ Git repository initialized" -ForegroundColor Green
    } catch {
        Write-Host "✗ Failed to initialize git repository: $($_.Exception.Message)" -ForegroundColor Red
    }
}

# Create ThirdParty directory structure
$directories = @(
    "ThirdParty",
    "ThirdParty/Graphics",
    "ThirdParty/Audio", 
    "ThirdParty/Physics",
    "ThirdParty/Networking",
    "ThirdParty/Scripting",
    "ThirdParty/Utilities",
    "ThirdParty/Math",
    "ThirdParty/Compression",
    "ThirdParty/Serialization",
    "ThirdParty/Testing",
    "ThirdParty/AI",
    "ThirdParty/Database"
)

foreach ($dir in $directories) {
    if (-not (Test-Path $dir)) {
        try {
            New-Item -ItemType Directory -Path $dir -Force | Out-Null
            Write-Host "Created directory: $dir" -ForegroundColor Green
        } catch {
            Write-Host "Failed to create directory $dir`: $($_.Exception.Message)" -ForegroundColor Red
        }
    }
}

# Setup ALL dependencies for complete system
$success = 0
$total = 0

Write-Host "`n=== CORE ENGINE DEPENDENCIES ===" -ForegroundColor Magenta

# EnTT (Entity Component System)
$total++
if (Add-SafeSubmodule @("https://github.com/skypjack/entt.git") "ThirdParty/entt" "EnTT" "master" "Core") {
    $success++
}

# ImGui (User Interface)
$total++
if (Add-SafeSubmodule @("https://github.com/ocornut/imgui.git") "ThirdParty/imgui" "ImGui" "master" "Core") {
    $success++
    
    # Create CMakeLists.txt for ImGui
    $imguiCMake = @'
cmake_minimum_required(VERSION 3.16)
project(imgui)

set(IMGUI_SOURCES
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    backends/imgui_impl_win32.cpp
    backends/imgui_impl_dx11.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC d3d11 dxgi)
target_compile_definitions(imgui PUBLIC 
    IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS
)

set_target_properties(imgui PROPERTIES 
    FOLDER "ThirdParty"
    CXX_STANDARD 17
)

if(NOT TARGET imgui::imgui)
    add_library(imgui::imgui ALIAS imgui)
endif()
'@
    Create-CMakeForLibrary "ThirdParty/imgui" "ImGui" $imguiCMake
}

# STB (Image/Audio/Font loading)
$total++
if (Add-SafeSubmodule @("https://github.com/nothings/stb.git") "ThirdParty/stb" "STB" "master" "Core") {
    $success++
}

# Sol2 (Lua C++ binding)
$total++
if (Add-SafeSubmodule @("https://github.com/ThePhD/sol2.git") "ThirdParty/sol2" "Sol2" "develop" "Scripting") {
    $success++
}

Write-Host "`n=== SCRIPTING DEPENDENCIES ===" -ForegroundColor Magenta

# Lua (if Sol2 doesn't include it)
$total++
if (Add-SafeSubmodule @("https://github.com/lua/lua.git") "ThirdParty/lua" "Lua" "master" "Scripting") {
    $success++
}

Write-Host "`n=== GRAPHICS DEPENDENCIES ===" -ForegroundColor Magenta

# DirectXTK (DirectX Toolkit)
$total++
if (Add-SafeSubmodule @("https://github.com/Microsoft/DirectXTK.git") "ThirdParty/Graphics/DirectXTK" "DirectXTK" "main" "Graphics") {
    $success++
}

# Assimp (3D Model loading)
$total++
if (Add-SafeSubmodule @("https://github.com/assimp/assimp.git") "ThirdParty/Graphics/assimp" "Assimp" "master" "Graphics") {
    $success++
}

Write-Host "`n=== PHYSICS DEPENDENCIES ===" -ForegroundColor Magenta

# PhysX (NVIDIA Physics)
$total++
if (Add-SafeSubmodule @("https://github.com/NVIDIAGameWorks/PhysX.git") "ThirdParty/Physics/PhysX" "PhysX" "4.1" "Physics") {
    $success++
}

# Bullet Physics (Alternative physics engine)
$total++
if (Add-SafeSubmodule @("https://github.com/bulletphysics/bullet3.git") "ThirdParty/Physics/Bullet" "Bullet3" "master" "Physics") {
    $success++
}

Write-Host "`n=== AUDIO DEPENDENCIES ===" -ForegroundColor Magenta

# OpenAL Soft (3D Audio)
$total++
if (Add-SafeSubmodule @("https://github.com/kcat/openal-soft.git") "ThirdParty/Audio/OpenAL" "OpenAL-Soft" "master" "Audio") {
    $success++
}

Write-Host "`n=== NETWORKING DEPENDENCIES ===" -ForegroundColor Magenta

# ENet (UDP networking library)
$total++
if (Add-SafeSubmodule @("https://github.com/lsalzman/enet.git") "ThirdParty/Networking/ENet" "ENet" "master" "Networking") {
    $success++
}

Write-Host "`n=== UTILITY DEPENDENCIES ===" -ForegroundColor Magenta

# fmt (Fast formatting library)
$total++
if (Add-SafeSubmodule @("https://github.com/fmtlib/fmt.git") "ThirdParty/Utilities/fmt" "fmt" "master" "Utilities") {
    $success++
}

# spdlog (Fast logging library)
$total++
if (Add-SafeSubmodule @("https://github.com/gabime/spdlog.git") "ThirdParty/Utilities/spdlog" "spdlog" "v1.x" "Utilities") {
    $success++
}

Write-Host "`n=== MATH DEPENDENCIES ===" -ForegroundColor Magenta

# GLM (OpenGL Mathematics)
$total++
if (Add-SafeSubmodule @("https://github.com/g-truc/glm.git") "ThirdParty/Math/glm" "GLM" "master" "Math") {
    $success++
}

Write-Host "`n=== SERIALIZATION DEPENDENCIES ===" -ForegroundColor Magenta

# nlohmann/json (JSON for Modern C++)
$total++
if (Add-SafeSubmodule @("https://github.com/nlohmann/json.git") "ThirdParty/Serialization/json" "nlohmann-json" "develop" "Serialization") {
    $success++
}

Write-Host "`n=== TESTING DEPENDENCIES ===" -ForegroundColor Magenta

# Google Test (Unit testing)
$total++
if (Add-SafeSubmodule @("https://github.com/google/googletest.git") "ThirdParty/Testing/googletest" "GoogleTest" "main" "Testing") {
    $success++
}

Write-Host "`n=== SETUP COMPLETE ===" -ForegroundColor Cyan

# Create detailed summary report
$summaryPath = "submodule_setup_report.txt"

# Calculate category statistics using the helper functions
$coreSuccess = Get-CategorySuccessCount "Core"
$coreTotal = Get-CategoryTotalCount "Core"
$graphicsSuccess = Get-CategorySuccessCount "Graphics"
$graphicsTotal = Get-CategoryTotalCount "Graphics"
$physicsSuccess = Get-CategorySuccessCount "Physics"
$physicsTotal = Get-CategoryTotalCount "Physics"
$audioSuccess = Get-CategorySuccessCount "Audio"
$audioTotal = Get-CategoryTotalCount "Audio"
$networkingSuccess = Get-CategorySuccessCount "Networking"
$networkingTotal = Get-CategoryTotalCount "Networking"
$scriptingSuccess = Get-CategorySuccessCount "Scripting"
$scriptingTotal = Get-CategoryTotalCount "Scripting"
$utilitiesSuccess = Get-CategorySuccessCount "Utilities"
$utilitiesTotal = Get-CategoryTotalCount "Utilities"
$mathSuccess = Get-CategorySuccessCount "Math"
$mathTotal = Get-CategoryTotalCount "Math"
$serializationSuccess = Get-CategorySuccessCount "Serialization"
$serializationTotal = Get-CategoryTotalCount "Serialization"
$testingSuccess = Get-CategorySuccessCount "Testing"
$testingTotal = Get-CategoryTotalCount "Testing"

$successRate = if ($total -gt 0) { [math]::Round(($success/$total)*100, 2) } else { 0 }

function Format-CategoryResults($categoryName) {
    $results = $global:setupResults | Where-Object { $_.Category -eq $categoryName }
    $output = "`n$categoryName LIBRARIES:`n"
    $output += "=" * ($categoryName.Length + 11) + "`n"
    
    if ($results.Count -eq 0) {
        $output += "  No libraries in this category`n"
        return $output
    }
    
    foreach ($result in $results) {
        $status = if ($result.Success) { "✓" } else { "✗" }
        $color = if ($result.Success) { "SUCCESS" } else { "FAILED" }
        
        $output += "  $status $($result.Name) - $color`n"
        
        if ($result.Success) {
            $output += "    Method: $($result.Method)`n"
            if ($result.URL) {
                $output += "    URL: $($result.URL)`n"
            }
            if ($result.Warnings.Count -gt 0) {
                $output += "    Warnings: $($result.Warnings -join ', ')`n"
            }
        } else {
            $output += "    Error: $($result.Error)`n"
            $output += "    Path: $($result.Path)`n"
        }
        $output += "`n"
    }
    
    return $output
}

# Calculate optional library success
$optionalSuccess = $audioSuccess + $networkingSuccess + $utilitiesSuccess + $mathSuccess + $serializationSuccess + $scriptingSuccess + $testingSuccess
$optionalTotal = $audioTotal + $networkingTotal + $utilitiesTotal + $mathTotal + $serializationTotal + $scriptingTotal + $testingTotal

$report = @"
SparkEngine Complete Submodule Setup Report
===========================================
Date: $(Get-Date)
Success Rate: $success / $total ($successRate%)
Overall Status: $(if($success -eq $total) { "ALL DEPENDENCIES READY" } elseif($success -gt ($total * 0.8)) { "MOSTLY READY - Some optional libraries missing" } elseif($success -gt ($total * 0.5)) { "PARTIALLY READY - Several dependencies missing" } else { "NEEDS ATTENTION - Many dependencies failed" })

SUMMARY BY IMPORTANCE:
Critical (Core Engine): $coreSuccess / $coreTotal successful
Important (Graphics/Physics): $($graphicsSuccess + $physicsSuccess) / $($graphicsTotal + $physicsTotal) successful
Optional (Audio/Networking/Utils): $optionalSuccess / $optionalTotal successful

$(Format-CategoryResults "Core")
$(Format-CategoryResults "Graphics")
$(Format-CategoryResults "Physics")
$(Format-CategoryResults "Audio")
$(Format-CategoryResults "Networking")
$(Format-CategoryResults "Scripting")
$(Format-CategoryResults "Utilities")
$(Format-CategoryResults "Math")
$(Format-CategoryResults "Serialization")
$(Format-CategoryResults "Testing")

DETAILED FAILURE ANALYSIS:
=========================
$( if ($global:failureDetails.Count -eq 0) {
    "No failures detected!"
} else {
    $failureAnalysis = ""
    foreach ($failure in $global:failureDetails) {
        $failureAnalysis += "FAILED: $($failure.Name) ($($failure.Category))`n"
        $failureAnalysis += "  Path: $($failure.Path)`n"
        $failureAnalysis += "  Error: $($failure.Error)`n"
        $failureAnalysis += "  Attempted Method: $($failure.Method)`n"
        $failureAnalysis += "`n"
    }
    $failureAnalysis
})

RECOMMENDATIONS:
===============
$( if ($success -eq $total) {
    "✓ All dependencies are ready! You can proceed with building SparkEngine."
} elseif ($coreSuccess -lt $coreTotal) {
    "⚠ CRITICAL: Core dependencies are missing. SparkEngine may not build properly.
   Priority: Fix EnTT, ImGui, STB, and Sol2 first."
} elseif ($success -gt ($total * 0.7)) {
    "✓ Core systems are ready. Missing dependencies are mostly optional.
   You can build SparkEngine, but some features may be disabled."
} else {
    "⚠ Many dependencies failed. Check your internet connection and git configuration.
   Consider running the script again or manually downloading missing libraries."
})

COMMON SOLUTIONS FOR FAILURES:
==============================
1. Network Issues:
   - Check internet connection
   - Try running script again (some failures may be temporary)
   - Check if corporate firewall is blocking git

2. Git Configuration Issues:
   - Ensure git is installed and in PATH
   - Check git credentials: git config --global user.name "Your Name"
   - Check git credentials: git config --global user.email "your@email.com"

3. Permission Issues:
   - Run PowerShell as Administrator
   - Check if antivirus is blocking file operations

4. Branch/Repository Issues:
   - Some repositories may have changed default branches
   - Try manually cloning failed repositories with different branch names

5. Disk Space:
   - Ensure sufficient disk space (recommend 10+ GB free)
   - Some repositories are very large (PhysX, Assimp, etc.)

NEXT STEPS:
===========
1. Review failures above and attempt fixes
2. For missing optional libraries, disable related CMake features:
   cmake -DENABLE_[FEATURE]=OFF
3. Build with available dependencies:
   cmake -B build -S . -G "Visual Studio 17 2022" -A x64
4. If core dependencies failed, focus on fixing those first

Generated: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
"@

Set-Content $summaryPath $report -Encoding UTF8

# Display summary
Write-Host "Successfully set up: $success / $total dependencies" -ForegroundColor $(if($success -eq $total) { "Green" } elseif($success -gt ($total * 0.8)) { "Yellow" } else { "Red" })
Write-Host "Detailed report saved to: $summaryPath" -ForegroundColor Cyan

# Show critical failures immediately
if ($global:failureDetails.Count -gt 0) {
    Write-Host "`n=== IMMEDIATE ATTENTION NEEDED ===" -ForegroundColor Red
    $criticalFailures = $global:failureDetails | Where-Object { $_.Category -eq "Core" }
    if ($criticalFailures.Count -gt 0) {
        Write-Host "CRITICAL FAILURES (Core Dependencies):" -ForegroundColor Red
        foreach ($failure in $criticalFailures) {
            Write-Host "  ✗ $($failure.Name): $($failure.Error)" -ForegroundColor Red
        }
    }
    
    $otherFailures = $global:failureDetails | Where-Object { $_.Category -ne "Core" }
    if ($otherFailures.Count -gt 0) {
        Write-Host "`nOther Failures (Optional Features):" -ForegroundColor Yellow
        foreach ($failure in $otherFailures) {
            Write-Host "  ⚠ $($failure.Name): $($failure.Error)" -ForegroundColor Yellow
        }
    }
}

# Initialize submodules if any were added
if ($success -gt 0) {
    Write-Host "`nInitializing and updating submodules..." -ForegroundColor Yellow
    try {
        & git submodule update --init --recursive --jobs 4
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✓ Submodules initialized successfully" -ForegroundColor Green
        } else {
            Write-Host "⚠ Some submodules may have failed to initialize" -ForegroundColor Yellow
        }
    } catch {
        Write-Host "✗ Failed to initialize submodules: $($_.Exception.Message)" -ForegroundColor Red
    }
}

Write-Host "`n🚀 SparkEngine Complete Submodule Setup Finished! 🚀" -ForegroundColor Green
Write-Host "Check the detailed report at: $summaryPath" -ForegroundColor Cyan