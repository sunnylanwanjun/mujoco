#pragma once

class Application {
public:
    Application() {}
    virtual ~Application() {}

    static Application* GetInstance();
    static void DestroyInstance();

    virtual void* Init(int width, int height, void* window) = 0;
    virtual void UnInit() = 0;
    virtual void Update() = 0;

private:
    static Application* instance;
};

#define APPLICATION_REG(APP_CLASS)\
Application* Application::GetInstance() {\
    if (instance == nullptr) {\
        instance = new APP_CLASS();\
    }\
    return instance;\
}