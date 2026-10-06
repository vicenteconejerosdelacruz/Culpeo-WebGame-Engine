#pragma once

#include <GLFW/glfw3.h>
#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_glfw.h>

namespace DeviceUtils
{
    wgpu::Instance CreateInstance();
    wgpu::Adapter GetAdapter(wgpu::Instance& instance);
	wgpu::Device CreateDevice(wgpu::Instance& instance, wgpu::Adapter& adapter);
    wgpu::Surface CreateSurface(wgpu::Instance& instance, GLFWwindow* window);

}