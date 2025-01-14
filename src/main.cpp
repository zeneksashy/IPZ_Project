﻿#include "Core/application.h"
#include "Core/scene.h"
#include "Renderer/renderpipeline.h"
#include "Tests/testconnect4.h"

int main(void)
{
    //TODO: add initialization tests everywhere and setup some defaults like camera etc
    App::init(1200, 800);
    App::setVsync(0);

    // FPS counter should go to App
    float dtSum = 0;
    int frameCount = 0;
    auto test3 = std::make_shared<TestConnect4>();
    RenderPipeline renderer;
    while (!App::shouldClose())
    {
        AssetManager::checkForChanges();
        AssetManager::tryReloadAssets();

        float dt = App::getTimeStep();
        dtSum+=dt;
        frameCount++;
        if (frameCount == 20)
        {
            char title[50];
            double ms = dtSum/double(frameCount) * 1000;
            sprintf_s(title, "Test - FPS: %.1f (%.2fms)", 1/ms*1000, ms);
            App::setWindowTitle(title);
            frameCount = 0;
            dtSum = 0;
        }

        GraphicsContext::getCamera()->onUpdate(dt);
        test3->onUpdate(dt);
        renderer.drawScene(test3);
        App::submitFrame();
    }
}
