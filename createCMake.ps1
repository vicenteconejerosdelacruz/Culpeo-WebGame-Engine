$engine_folder=&"pwd"

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

cd ../emsdk
./emsdk_env.ps1

cd $engine_folder

#emcmake cmake -B build -G Ninja -DCMAKE_CXX_FLAGS="-DEMSCRIPTEN=1 -D__EMSCRIPTEN__=1 -Wno-error=deprecated-pragma"
emcmake cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-DEMSCRIPTEN=1 -D__EMSCRIPTEN__=1 -Wno-error=deprecated-pragma"