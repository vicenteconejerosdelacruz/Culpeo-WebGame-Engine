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
#include <Game.h>
#include <Renderer.h>
#include <Templates.h>

using namespace Templates;
using namespace Game;

std::unique_ptr<JRenderer> renderer;

wgpu::RenderPipeline pipeline;

bool InitInstance()
{
  CreateSystemTemplates(); // Llamada a la función para crear plantillas del sistema

  renderer = std::make_unique<JRenderer>();
  renderer->Initialize(); // Inicializa el renderer

  renderer->CreateWindow(); // Crea la ventana
  renderer->CreateSurface(); // Crea la surface

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
  // 1. Polling de eventos de GLFW en cada frame (necesario para detectar redimensionamientos del navegador)
  glfwPollEvents();

  wgpu::SurfaceTexture surfaceTexture;
  renderer->surface.GetCurrentTexture(&surfaceTexture);

  wgpu::RenderPassColorAttachment attachment{
      .view = surfaceTexture.texture.CreateView(),
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

  // 2. Presentar la textura y procesar eventos de WebGPU en cada frame
  renderer->surface.Present();
  renderer->GetInstance().ProcessEvents();
}

int main() {
  
  InitInstance();
  BootGame();
  
  CreateRenderPipeline();

#if defined(__EMSCRIPTEN__)
  emscripten_set_main_loop(Render, 0, false);
#else
  while (!glfwWindowShouldClose(renderer->GetWindow())) {
    Render();
  }
#endif

  return 0;
}