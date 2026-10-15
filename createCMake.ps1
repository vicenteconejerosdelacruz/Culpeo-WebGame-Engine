param(
    [Parameter(Mandatory=$false)]
    [ValidateSet("Editor_Debug", "Debug", "Editor_Development", "Development", "Editor_Release", "Release")]
    [string]$Configuration = "Development"
)

$engine_folder = (Get-Location).Path

function Ensure-GitSubmodules {
    Write-Host "[*] Verifying Git submodules..." -ForegroundColor Cyan
    $submoduleStatus = git submodule status
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[-] Error: Not a valid Git repository or the command failed." -ForegroundColor Red
        exit 1
    }

    $needsUpdate =$false
    foreach ($line in $submoduleStatus) {
        if ($line.Trim().StartsWith("-")) {
            $needsUpdate =$true
            break
        }
    }

    if ($needsUpdate) {
        Write-Host "[!] Uninitialized submodules detected." -ForegroundColor Yellow
        Write-Host "[*] Please run the following command manually:" -ForegroundColor Cyan
        Write-Host "    git submodule update --init --recursive" -ForegroundColor White
        exit 1
    } else {
        Write-Host "[+] All submodules are ready." -ForegroundColor Green
    }
}

Ensure-GitSubmodules

# Configuración manual del entorno de Emscripten para evitar bloqueos
$emsdk_path = Resolve-Path "$PSScriptRoot\..\emsdk"
$env:EMSDK =$emsdk_path
$env:EMSDK_NODE = "$emsdk_path\node\16.15.0_64bit\bin\node.exe"
$env:PATH = "$emsdk_path;$emsdk_path\upstream\emscripten;$env:PATH"

cd $engine_folder

# Mapear la configuración del motor a un CMAKE_BUILD_TYPE válido para las SDKs y flags
$sdkBuildType = "Release"
$extraCFlags = "-DEMSCRIPTEN=1 -D__EMSCRIPTEN__=1 -Wno-error=deprecated-pragma"

switch ($Configuration) {
    { $_ -in "Editor_Debug", "Debug" } { $sdkBuildType = "Debug" }
    { $_ -in "Editor_Development", "Development", "Editor_Release", "Release" } {
        $sdkBuildType = "Release"
        if ($Configuration -match "Development") { $extraCFlags += " -g3" }
        else { $extraCFlags += " -O3" }
    }
}

# Carpeta de build aislada por configuración
$buildDir = "build/$Configuration"

Write-Host "[*] Generando CMake Cache en: $buildDir" -ForegroundColor Cyan

emcmake cmake -B "$buildDir" -G Ninja `
    -DCMAKE_BUILD_TYPE="$sdkBuildType" `
    -DCULPEO_CONFIG="$Configuration" `
    -DCMAKE_C_FLAGS="$extraCFlags" `
    -DCMAKE_CXX_FLAGS="$extraCFlags"