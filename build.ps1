<#
.SYNOPSIS
  Configure & build SparkEngine from PowerShell.
.PARAMETER Config
  Build configuration: Debug / Release (default = Debug)
.PARAMETER Gen
  CMake generator (default: "Visual Studio 17 2022")
.PARAMETER Editor
  Switch ON the editor module
.PARAMETER Console
  Switch ON the external console
.PARAMETER AngelScript
  Switch ON AngelScript integration
#>

param(
  [ValidateSet("Debug","Release")][string]$config = "Debug",
  [string]$gen  = "Visual Studio 17 2022",
  [switch]$editor,
  [switch]$console,
  [switch]$angelscript
)

$ErrorActionPreference = "Stop"

$buildDir = "build"
if (!(Test-Path $buildDir)) { New-Item $buildDir -ItemType Directory | Out-Null }
Push-Location $buildDir

$cmakeOpts = @()
if ($editor)      { $cmakeOpts += "-DENABLE_EDITOR=ON" }      else { $cmakeOpts += "-DENABLE_EDITOR=OFF" }
if ($console)     { $cmakeOpts += "-DENABLE_CONSOLE=ON" }     else { $cmakeOpts += "-DENABLE_CONSOLE=OFF" }
if ($angelscript) { $cmakeOpts += "-DENABLE_ANGELSCRIPT=ON" } else { $cmakeOpts += "-DENABLE_ANGELSCRIPT=OFF" }

cmake .. -G "$gen" @cmakeOpts
cmake --build . --config $config --parallel
Pop-Location
