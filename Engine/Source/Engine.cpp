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
#include <Renderer.h>
#include <Templates.h>

using namespace Templates;

std::unique_ptr<JRenderer> renderer;

//wgpu::Instance instance;
//wgpu::Adapter adapter;
//wgpu::Device device;
wgpu::RenderPipeline pipeline;

//wgpu::Surface surface;
//wgpu::TextureFormat format;
//GLFWwindow* window = nullptr; // <--- Ahora es global para accederla fácilmente

// Modificado para aceptar ancho y alto dinámicos
// void ConfigureSurface(uint32_t width, uint32_t height) {
//   if (width == 0 || height == 0) return; // Evitar tamaños inválidos si se minimiza

//   wgpu::SurfaceCapabilities capabilities;
//   surface.GetCapabilities(renderer->GetAdapter(), &capabilities);
//   format = capabilities.formats[0];

//   wgpu::SurfaceConfiguration config{.device = renderer->GetDevice(),
//                                     .format = format,
//                                     .width = width,
//                                     .height = height};
//   surface.Configure(&config);
// }

// Función dedicada para actualizar/redimensionar la swapchain del surface
// void ResizeSwapChain(int width, int height) {
//   if (width == 0 || height == 0) return; // Evitar tamaños inválidos si se minimiza

//   wgpu::SurfaceCapabilities capabilities;
//   surface.GetCapabilities(renderer->GetAdapter(), &capabilities);
//   format = capabilities.formats[0];

//   wgpu::SurfaceConfiguration config{
//       .device = renderer->GetDevice(),
//       .format = format,
//       .width = static_cast<uint32_t>(width),
//       .height = static_cast<uint32_t>(height)
//   };
//   surface.Configure(&config);
// }

bool InitInstance()
{
  CreateSystemTemplates(); // Llamada a la función para crear plantillas del sistema

  renderer = std::make_unique<JRenderer>();
  renderer->Initialize(); // Inicializa el renderer

  renderer->CreateWindow(); // Crea la ventana
  renderer->CreateSurface(); // Crea la surface

  return true;
}

void Init() {

  InitInstance();

  /*
  static const auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
  wgpu::InstanceDescriptor instanceDesc{.requiredFeatureCount = 1,
                                        .requiredFeatures = &kTimedWaitAny};
  instance = wgpu::CreateInstance(&instanceDesc);

  wgpu::Future f1 = instance.RequestAdapter(
      nullptr, wgpu::CallbackMode::WaitAnyOnly,
      [](wgpu::RequestAdapterStatus status, wgpu::Adapter a,
         wgpu::StringView message) {
        if (status != wgpu::RequestAdapterStatus::Success) {
          std::cout << "RequestAdapter: " << message << "\n";
          exit(0);
        }
        adapter = std::move(a);
      });
  instance.WaitAny(f1, UINT64_MAX);

  wgpu::DeviceDescriptor desc{};
  desc.SetUncapturedErrorCallback([](const wgpu::Device&,
                                     wgpu::ErrorType errorType,
                                     wgpu::StringView message) {
    std::cout << "Error: " << errorType << " - message: " << message << "\n";
  });

  wgpu::Future f2 = adapter.RequestDevice(
      &desc, wgpu::CallbackMode::WaitAnyOnly,
      [](wgpu::RequestDeviceStatus status, wgpu::Device d,
         wgpu::StringView message) {
        if (status != wgpu::RequestDeviceStatus::Success) {
          std::cout << "RequestDevice: " << message << "\n";
          exit(0);
        }
        device = std::move(d);
      });
  instance.WaitAny(f2, UINT64_MAX);
  */
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

void InitGraphics() {

  // Obtener el tamaño inicial real del framebuffer del canvas/ventana
  //int width, height;
  //glfwGetFramebufferSize(window, &width, &height);
  
  //ConfigureSurface(width, height);
  CreateRenderPipeline();
}

void Start() {
  // if (!glfwInit()) {
  //   return;
  // }

  // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  
  // // Tamaño inicial por defecto (el navegador lo estirará si usas CSS width/height 100%)
  // window = glfwCreateWindow(512, 512, "WebGPU window", nullptr, nullptr);
  // surface = wgpu::glfw::CreateSurfaceForWindow(renderer->GetInstance(), window);

  // // 3. Registrar el callback para cuando el navegador cambie de tamaño el canvas
  // glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
  //   ResizeSwapChain(width, height);
  // });

  InitGraphics();

#if defined(__EMSCRIPTEN__)
  emscripten_set_main_loop(Render, 0, false);
#else
  while (!glfwWindowShouldClose(renderer->GetWindow())) {
    Render();
  }
#endif
}

int main() {
  Init();
  Start();
}