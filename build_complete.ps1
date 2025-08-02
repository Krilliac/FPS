# Complete build script for SparkEngine Unified System
Write-Host "Building SparkEngine Complete Unified System..." -ForegroundColor Green

# Function to check for required tools
function Test-Tool {
    param([string]$ToolName, [string]$Command)
    
    try {
        $null = & $Command --version 2>$null
        Write-Host "âœ“ $ToolName found" -ForegroundColor Green
        return $true
    }
    catch {
        Write-Host "âœ— $ToolName not found" -ForegroundColor Red
        return $false
    }
}

# Check prerequisites
Write-Host "`nChecking prerequisites..." -ForegroundColor Yellow
$cmakeOk = Test-Tool "CMake" "cmake"
$gitOk = Test-Tool "Git" "git"

if (-not $cmakeOk) {
    Write-Host "CMake is required. Please install CMake and add it to PATH." -ForegroundColor Red
    exit 1
}

# Initialize submodules
if ($gitOk -and (Test-Path ".gitmodules")) {
    Write-Host "`nInitializing submodules..." -ForegroundColor Yellow
    & git submodule update --init --recursive
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Warning: Submodule initialization failed" -ForegroundColor Yellow
    }
}

# Clean previous build
if (Test-Path "build") {
    Write-Host "`nCleaning previous build..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force "build"
}

# Generate build files
Write-Host "`nGenerating build files..." -ForegroundColor Yellow
$generateArgs = @(
    "-B", "build",
    "-S", ".",
    "-G", "Visual Studio 17 2022",
    "-A", "x64",
    "-DENABLE_EDITOR=ON",
    "-DENABLE_LUA=ON",
    "-DENABLE_COLLABORATIVE=ON"
)

$generateResult = & cmake @generateArgs
if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake generation failed!" -ForegroundColor Red
    Write-Host "Error output:" -ForegroundColor Red
    Write-Host $generateResult -ForegroundColor Red
    exit 1
}

Write-Host "âœ“ Build files generated successfully" -ForegroundColor Green

# Build the project (Debug first)
Write-Host "`nBuilding project (Debug)..." -ForegroundColor Yellow
$buildResult = & cmake --build build --config Debug --parallel
if ($LASTEXITCODE -ne 0) {
    Write-Host "Debug build failed!" -ForegroundColor Red
    exit 1
}

Write-Host "âœ“ Debug build completed successfully" -ForegroundColor Green

# Build Release version
Write-Host "`nBuilding project (Release)..." -ForegroundColor Yellow
$buildResult = & cmake --build build --config Release --parallel
if ($LASTEXITCODE -ne 0) {
    Write-Host "Release build failed!" -ForegroundColor Red
    exit 1
}

Write-Host "âœ“ Release build completed successfully" -ForegroundColor Green

Write-Host "`nðŸŽ‰ SparkEngine Complete Unified System build successful! ðŸŽ‰" -ForegroundColor Green
Write-Host "`nExecutable locations:" -ForegroundColor Cyan
Write-Host "  Debug:   build/bin/Debug/SparkEngine.exe" -ForegroundColor White
Write-Host "  Release: build/bin/Release/SparkEngine.exe" -ForegroundColor White

# Test run option
$testRun = Read-Host "`nTest run the Debug version? (y/n)"
if ($testRun -eq "y" -or $testRun -eq "Y") {
    Write-Host "`nStarting SparkEngine (Debug)..." -ForegroundColor Green
    if (Test-Path "build/bin/Debug/SparkEngine.exe") {
        & "build/bin/Debug/SparkEngine.exe"
    } else {
        Write-Host "Debug executable not found!" -ForegroundColor Red
    }
}
