param(
    [Parameter(Mandatory=$false)]
    [ValidateSet("Editor_Debug", "Debug", "Editor_Development", "Development", "Editor_Release", "Release")]
    [string]$Configuration = "Development"
)

$engine_folder = (Get-Location).Path

# Resolver la ruta de emsdk relativa a la ubicación del repositorio
$emsdk_path = Resolve-Path "$PSScriptRoot\..\emsdk"

# Configuración manual de variables de entorno para Emscripten
$env:EMSDK =$emsdk_path
$env:EMSDK_NODE = "$emsdk_path\node\16.15.0_64bit\bin\node.exe"
$env:PATH = "$emsdk_path;$emsdk_path\upstream\emscripten;$env:PATH"

Write-Host "[+] Entorno Emscripten configurado manualmente en: $env:EMSDK" -ForegroundColor Green
Write-Host "[*] Compilando Engine en configuración: $Configuration" -ForegroundColor Cyan

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

cd $engine_folder

# Carpeta de build aislada por configuración para evitar conflictos de Ninja
$buildDir = "build/$Configuration"

# Mapear configuraciones de Culpeo a CMAKE_BUILD_TYPE y Flags de símbolos
$cmakeBuildType = "Release"
$extraCFlags = "-DEMSCRIPTEN=1 -D__EMSCRIPTEN__=1 -Wno-error=deprecated-pragma"

switch ($Configuration) {
    { $_ -in "Editor_Debug", "Debug" } {
        $cmakeBuildType = "Debug"
    }
    { $_ -in "Editor_Development", "Development" } {
        $cmakeBuildType = "Release"
        $extraCFlags += " -g3" # Símbolos DWARF completos para desarrollo
    }
    "Editor_Release" {
        $cmakeBuildType = "Release"
        $extraCFlags += " -O3" # Release puro optimizado sin símbolos pesados
    }
    "Release" {
        $cmakeBuildType = "Release"
        $extraCFlags += " -O3"
    }
}

# Usar emcmake y la carpeta de build aislada por configuración
emcmake cmake -B "$buildDir" -S . `
    -DCMAKE_BUILD_TYPE="$cmakeBuildType" `
    -DCULPEO_CONFIG="$Configuration" `
    -DCMAKE_C_FLAGS="$extraCFlags" `
    -DCMAKE_CXX_FLAGS="$extraCFlags"

# Compilación apuntando a la carpeta de build específica
cmake --build "$buildDir" --target CulpeoEngine -- -j4 -v | ForEach-Object {
    if ($_ -match " -c\s+(.+\.cpp)$") {
        Write-Host "[Compilando] $($Matches[1])" -ForegroundColor Cyan
    } elseif ($_ -match " -o\s+(.+\.js)$") {
        Write-Host "[Enlazando]   $($Matches[1])" -ForegroundColor Green
    } else {
        $_
    }
}