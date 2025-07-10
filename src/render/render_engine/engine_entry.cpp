#include "engine_entry.h"
#include "logic/application.h"

void* EngineInit(int width, int height, void* window) {
    return Application::GetInstance()->Init(width, height, window);
}

void EngineUpdate() {
    Application::GetInstance()->Update();
}

void EngineUnInit() {
    Application::GetInstance()->DestroyInstance();
}