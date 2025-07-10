#include "application.h"

Application* Application::instance = nullptr;

void Application::DestroyInstance() {
    if (instance) {
        instance->UnInit();
        delete instance;
        instance = nullptr;
    }
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