#include "application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

Application* Application::instance = nullptr;

void Application::DestroyInstance() {
    if (instance) {
        instance->UnInit();
        delete instance;
        instance = nullptr;
    }
}

void Application::Update() {
    _Update();
    glfwPollEvents();
}

void* Application::Init(int w, int h, void* win) {
    appWidth = w;
    appHeight = h;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(appWidth, appHeight, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer((GLFWwindow*)window, this);
    glfwSetFramebufferSizeCallback((GLFWwindow*)window, [](GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
        app->appWidth = width;
        app->appHeight = height;
    });

    _Init();

    return window;
}

void Application::UnInit() {
    _UnInit();

    glfwDestroyWindow((GLFWwindow*)window);
    glfwTerminate();
}

#ifndef RENDER_ENGINE_EMBED
int main() {
    Application* app = Application::GetInstance();
    app->Init(600, 800, nullptr);
    app->Update();
    Application::DestroyInstance();
    return 0;
}
#endif