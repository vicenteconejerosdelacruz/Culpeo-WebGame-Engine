$run_folder = (Get-Location).Path

# Configuración manual del entorno de emsdk para evitar bloqueos
$emsdk_path = Resolve-Path "$PSScriptRoot\..\..\..\emsdk"

cd $emsdk_path

. .\emsdk_env.ps1

cd $run_folder
emrun index.html