$engine_folder = (Get-Location).Path

# Resolver la ruta de emsdk relativa a la ubicación del repositorio
$emsdk_path = Resolve-Path "$PSScriptRoot\..\emsdk"

# Configuración manual de variables de entorno para Emscripten
$env:EMSDK = $emsdk_path
$env:EMSDK_NODE = "$emsdk_path\node\16.15.0_64bit\bin\node.exe"
$env:PATH = "$emsdk_path;$emsdk_path\upstream\emscripten;$env:PATH"

Write-Host "[+] Entorno Emscripten configurado manualmente en: $env:EMSDK" -ForegroundColor Green

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

cd $engine_folder

# Compilación secuencial con -j1 para evitar bloqueos en el sistema de compilación
#cmake --build build --target CulpeoEngine -- -j1 -v

cmake --build build --target CulpeoEngine -- -j4 -v | ForEach-Object {
    if ($_ -match " -c\s+(.+\.cpp)$") {
        Write-Host "[Compilando] $($Matches[1])" -ForegroundColor Cyan
    } elseif ($_ -match " -o\s+(.+\.js)$") {
        Write-Host "[Enlazando]   $($Matches[1])" -ForegroundColor Green
    } else {
        $_
    }
}