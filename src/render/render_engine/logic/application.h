#pragma once
#include <queue>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

struct GeomData {
    enum GeomType {
        Plane,
        Sphere,
        Capsule,
        Box
    };
    glm::mat4 transMat;
};

class Application {
public:
    Application() {}
    virtual ~Application() {}

    static Application* GetInstance();
    static void DestroyInstance();

    void* Init(int width, int height, void* window);
    void UnInit();
    void Update();

    void DrawGeom(const GeomData& data) { drawGeomDatas.push(data); }
    void UpdateView(const glm::mat4& view) { viewMat = view; };
    void UpdateProj(const glm::mat4& proj) { projMat = proj; }

protected:
    virtual void _Update() = 0;
    virtual void _UnInit() = 0;
    virtual void _Init() = 0;

protected:
    static Application* instance;

    bool framebufferResized = false;
    int appWidth = 0;
    int appHeight = 0;
    void* window = nullptr;

    std::queue<GeomData> drawGeomDatas;
    glm::mat4 viewMat;
    glm::mat4 projMat;
};

#define APPLICATION_REG(APP_CLASS)\
Application* Application::GetInstance() {\
    if (instance == nullptr) {\
        instance = new APP_CLASS();\
    }\
    return instance;\
}