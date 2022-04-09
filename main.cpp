
#include <iostream>

#include "platformhandle.h"

#include "scene/scene.h"
#include "engine/engine.h"
#include "view/view.h"
#include "camera/camera.h"
#include "renderer/renderer.h"

lightman::Scene* myScene;
lightman::Engine* myEngine;
lightman::View *myView;
lightman::Camera * myCamera;
lightman::SwapChain * mySwapChain;
lightman::Renderer *myRenderer;

void Setup()
{
    myScene = new lightman::Scene();
    myScene->Parse("/Users/XK/Downloads/LuxCore2.1Benchmark/LuxCoreScene/scene.scn");

    myEngine = lightman::Engine::Create(lightman::backend::BackendType::OPENGL);

    myView = new lightman::View();
    myView->SetScene(myScene);

    myCamera = new lightman::Camera();
    myView->SetCamera(myCamera);
    
    void* nativeWindow = lightman::GetNativeWindow();
    mySwapChain = myEngine->CreateSwapChain(nativeWindow);
    myRenderer = myEngine->CreateRender(lightman::RenderType::RASTER_GPU);
}
void PreRender()
{

}

void PostRender()
{

}

void Render()
{
    PreRender();
    if (myRenderer->BeginFrame(mySwapChain))
    {
        myRenderer->RenderFrame(myView);
        myRenderer->EndFrame();
        PostRender();
    }
    
}

void destory()
{
    delete myView;
    delete myScene;
    delete myCamera;

    lightman::Engine::DestroyInstance();
}

int main(int argc, const char* argv[])
{
    lightman::MainWindow(argc, argv, Setup, Render);
    return 0;
}
