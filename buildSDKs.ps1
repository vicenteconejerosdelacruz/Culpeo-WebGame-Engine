param(
    [Parameter(Mandatory=$false)]
    [ValidateSet("Debug", "Release")]
    [string]$BuildType = "Release"
)

$engine_folder = (Get-Location).Path

function Ensure-GitSubmodules {
    Write-Host "[*] Verifying Git submodules..." -ForegroundColor Cyan
    $submoduleStatus = git submodule status
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[-] Error: Not a valid Git repository or the command failed." -ForegroundColor Red
        exit 1
    }

    $needsUpdate = $false
    foreach ($line in $submoduleStatus) {
        if ($line.Trim().StartsWith("-")) {
            $needsUpdate = $true
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

# Configuración manual del entorno de emsdk para evitar bloqueos
$emsdk_path = Resolve-Path "$PSScriptRoot\..\emsdk"
$env:EMSDK = $emsdk_path
$env:EMSDK_NODE = "$emsdk_path\node\16.15.0_64bit\bin\node.exe"
$env:PATH = "$emsdk_path;$emsdk_path\upstream\emscripten;$env:PATH"

cd $engine_folder

# Carpeta de build aislada por tipo de build para las SDKs
$sdkBuildDir = "build_$BuildType"

# Definir flags requeridos para Emscripten en las SDKs
$extraCFlags = "-DEMSCRIPTEN=1 -D__EMSCRIPTEN__=1 -Wno-error=deprecated-pragma"
if ($BuildType -eq "Debug") {
    $extraCFlags += " -g3"
} else {
    $extraCFlags += " -O3"
}

Write-Host "[*] Configurando CMake para SDKs en modo $BuildType ($sdkBuildDir)..." -ForegroundColor Cyan

emcmake cmake -B "$sdkBuildDir" -S . -G Ninja `
    -DCMAKE_BUILD_TYPE="$BuildType" `
    -DCMAKE_C_FLAGS="$extraCFlags" `
    -DCMAKE_CXX_FLAGS="$extraCFlags"

# Compilación secuencial con -j1 para evitar bloqueos y filtrado de salida
cmake --build "$sdkBuildDir" --target WasmSDKs -- -j1 -v | ForEach-Object {
    if ($_ -match " -c\s+(.+\.cpp)$") {
        Write-Host "[Compilando] $($Matches[1])" -ForegroundColor Cyan
    } elseif ($_ -match " -o\s+(.+\.js)$") {
        Write-Host "[Enlazando]   $($Matches[1])" -ForegroundColor Green
    } else {
        $_
    }
}