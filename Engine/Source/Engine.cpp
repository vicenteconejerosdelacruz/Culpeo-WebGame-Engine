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

std::atomic<bool> g_EngineReady{false};

namespace Scene
{
  extern void SceneRender();
};

using namespace Templates;
using namespace Game;

RECT hWndRect = { 0L, 0L, 0L, 0L };

std::unique_ptr<JRenderer> renderer;
wgpu::RenderPipeline pipeline;

bool InitInstance()
{
  renderer = std::make_unique<JRenderer>();
  renderer->Initialize(); 

  renderer->CreateWindow(); 
  renderer->CreateSurface(); 

  return true;
}

const char shaderCode[] = R"(
    @vertex fn vertexMain(@builtin(vertex_index) i : u32) ->
      @builtin(position) vec4f {
        const pos = array(vec2f(0, 1), vec2f(-1, -1), vec2f(1, -1));
        return vec4f(pos[i], 0, 1);
    }
    @fragment fn fragmentMain() -> @location(0) vec4f {
        return vec4f(1, 0, 0, 1);
    }
)";

void CreateRenderPipeline() {
  wgpu::ShaderSourceWGSL wgsl{{.code = shaderCode}};

  wgpu::ShaderModuleDescriptor shaderModuleDescriptor{.nextInChain = &wgsl};
  wgpu::ShaderModule shaderModule =
      renderer->GetDevice().CreateShaderModule(&shaderModuleDescriptor);

  wgpu::ColorTargetState colorTargetState{.format = renderer->format};

  wgpu::FragmentState fragmentState{
      .module = shaderModule, .targetCount = 1, .targets = &colorTargetState};

  wgpu::RenderPipelineDescriptor descriptor{.vertex = {.module = shaderModule},
                                            .fragment = &fragmentState};
  pipeline = renderer->GetDevice().CreateRenderPipeline(&descriptor);
}

void Render() {
  using namespace Scene;

  if (!g_EngineReady.load(std::memory_order_acquire)) {
        // Opcional: Renderizar un frame vacío, color plano o pantalla de carga
        return; 
    }
    
  // 1. Polling de eventos de GLFW (resize, teclado, mouse)
  glfwPollEvents();

  for(auto& cb:renderer->executionCallback)
  {
    cb();
  }
  renderer->executionCallback.clear();
  
  SceneRender();

  /*
  wgpu::SurfaceTexture surfaceTexture;
  renderer->surface.GetCurrentTexture(&surfaceTexture);

  // Validación de seguridad para evitar crashes si la ventana se minimiza o invalida
  if (!surfaceTexture.texture) {
    return;
  }

  wgpu::TextureView view = surfaceTexture.texture.CreateView();

  wgpu::RenderPassColorAttachment attachment{
      .view = view,
      .loadOp = wgpu::LoadOp::Clear,
      .storeOp = wgpu::StoreOp::Store};

  wgpu::RenderPassDescriptor renderpass{.colorAttachmentCount = 1,
                                        .colorAttachments = &attachment};

  wgpu::CommandEncoder encoder = renderer->GetDevice().CreateCommandEncoder();
  wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderpass);
  pass.SetPipeline(pipeline);
  pass.Draw(3);
  pass.End();
  
  wgpu::CommandBuffer commands = encoder.Finish();
  renderer->GetDevice().GetQueue().Submit(1, &commands);

#ifndef __EMSCRIPTEN__
  // Solo en ejecuciones nativas de escritorio (Dawn C++ nativo)
  renderer->surface.Present();
#endif
*/

  // 2. Procesar eventos de WebGPU
  renderer->GetInstance().ProcessEvents();
}

int main() {
  CreateSystemTemplates(); 
  CreateTemplates();
  InitInstance();
  BootGame();
  CreateRenderPipeline();

#if defined(__EMSCRIPTEN__)
  // 0 = Usar requestAnimationFrame del navegador
  // true = Simular bucle infinito interrumpiendo el flujo lineal sin destruir variables globales
  emscripten_set_main_loop(Render, 0, true);
#else
  while (!glfwWindowShouldClose(renderer->GetWindow())) {
    Render();
  }
#endif

  return 0;
}