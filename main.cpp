
#include <iostream>

#include "platformhandle.h"

#include "scene/scene.h"
#include "engine/engine.h"
#include "view/view.h"
#include "camera/camera.h"

lightman::scene::Scene* myScene;
lightman::engine::Engine* myEngine;
lightman::view::View *myView;
lightman::camera::Camera * myCamera;

void Setup()
{
    myScene = new lightman::scene::Scene();
    myScene->Parse("/Users/XK/Downloads/LuxCore2.1Benchmark/LuxCoreScene/scene.scn");

    myEngine = lightman::engine::Engine::Create(lightman::backend::BackendType::OPENGL);

    myView = new lightman::view::View();
    myView->SetScene(myScene);

    myCamera = new lightman::camera::Camera();
    myView->SetCamera(myCamera);
    
    void* temp = nullptr;
    
    temp = lightman::GetNativeWindow();
    
    int xingkai = 1;

}

void destory()
{
    delete myView;
    delete myScene;
    delete myCamera;
}

int main(int argc, const char* argv[])
{
    lightman::MainWindow(argc, argv, Setup);
    return 0;
}
