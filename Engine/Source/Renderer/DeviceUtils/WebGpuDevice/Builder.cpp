#include "Builder.h"
#include <iostream>
#include <dawn/webgpu_cpp_print.h>

namespace DeviceUtils
{
    wgpu::Instance CreateInstance()
    {
        static const auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
        wgpu::InstanceDescriptor instanceDesc{.requiredFeatureCount = 1, .requiredFeatures = &kTimedWaitAny};
        return wgpu::CreateInstance(&instanceDesc);
    }

    wgpu::Adapter GetAdapter(wgpu::Instance& instance)
    {
        wgpu::Adapter adapter;
        wgpu::Future f1 = instance.RequestAdapter(
            nullptr, wgpu::CallbackMode::WaitAnyOnly,
            [&adapter](wgpu::RequestAdapterStatus status, wgpu::Adapter a,
                wgpu::StringView message) {
                if (status != wgpu::RequestAdapterStatus::Success) {
                std::cout << "RequestAdapter: " << message << "\n";
                exit(0);
                }
                adapter = std::move(a);
            });
        instance.WaitAny(f1, UINT64_MAX);
        return adapter;
    }

	wgpu::Device CreateDevice(wgpu::Instance& instance, wgpu::Adapter& adapter)
	{
        wgpu::Device device;
		wgpu::DeviceDescriptor desc{};
        desc.SetUncapturedErrorCallback([](const wgpu::Device&,
                                            wgpu::ErrorType errorType,
                                            wgpu::StringView message) {
            std::cout << "Error: " << errorType << " - message: " << message << "\n";
        });

        wgpu::Future f2 = adapter.RequestDevice(
            &desc, wgpu::CallbackMode::WaitAnyOnly,
            [&device](wgpu::RequestDeviceStatus status, wgpu::Device d,
                wgpu::StringView message) {
                if (status != wgpu::RequestDeviceStatus::Success) {
                std::cout << "RequestDevice: " << message << "\n";
                exit(0);
                }
                device = std::move(d);
            });
        instance.WaitAny(f2, UINT64_MAX);
        return device;
	}

    wgpu::Surface CreateSurface(wgpu::Instance& instance, GLFWwindow* window)
    {
        return wgpu::glfw::CreateSurfaceForWindow(instance, window);
    }
}