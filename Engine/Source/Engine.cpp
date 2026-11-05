#include <iostream>

#include <GLFW/glfw3.h>
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif
#include <dawn/webgpu_cpp_print.h>
#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_glfw.h>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
#include <RenderPass/RenderPass.h>
#include <Game.h>
#include <Renderer.h>
#include <Templates.h>
#if defined(_EDITOR)
#include <Editor.h>
#endif

std::atomic<bool> g_EngineReady{false};

namespace Scene
{
  extern void SceneRender();
};

using namespace Templates;
using namespace Game;

RECT hWndRect = { 0L, 0L, 0L, 0L };

std::unique_ptr<JRenderer> renderer;

bool InitInstance()
{
  renderer = std::make_unique<JRenderer>();
  renderer->Initialize(); 

  renderer->CreateWindow(); 
  renderer->CreateSurface();
  renderer->CreateSwapChainPass(); 

  #if defined(_EDITOR)
  Editor::InitEditor();
  #endif
  return true;
}

void Render() {
  using namespace Scene;

  if (!g_EngineReady.load(std::memory_order_acquire)) {
        // Opcional: Renderizar un frame vacío, color plano o pantalla de carga
        return; 
    }
    
  // 1. Polling de eventos de GLFW (resize, teclado, mouse)

  for(auto& cb:renderer->executionCallback)
  {
    cb();
  }
  renderer->executionCallback.clear();
  
  SceneRender();

  renderer->MoveToNextBuffer();

  // 2. Procesar eventos de WebGPU
  renderer->GetInstance().ProcessEvents();
}

int main() {
  CreateSystemTemplates(); 
  CreateTemplates();
  InitInstance();
  BootGame();

// #if defined(__EMSCRIPTEN__)
  // 0 = Usar requestAnimationFrame del navegador
  // true = Simular bucle infinito interrumpiendo el flujo lineal sin destruir variables globales
  emscripten_set_main_loop(Render, 0, true);
// #else
  // while (!glfwWindowShouldClose(renderer->GetWindow())) {
    // Render();
  // }
// #endif

  return 0;
}