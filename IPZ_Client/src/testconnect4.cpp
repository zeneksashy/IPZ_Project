﻿#include "testconnect4.h"
bool intersectPlane(const vec3 &planeNormal, const vec3 &planePos, const vec3 &rayStartPos, const vec3 &ray, vec3 &intersection)
{
    // assuming vectors are all normalized
    float denom = dot(planeNormal, ray);
    if (denom > 1e-6) {
        vec3 p0l0 = planePos - rayStartPos;
        float t = dot(p0l0, planeNormal) / denom;
        if(t>=0)
        {
            intersection = rayStartPos + t*ray;
            return true;
        }
        else
            return false;
    }
    return false;
}
TestConnect4::TestConnect4()
{
    mesh1 = std::make_shared<MeshFile>("../assets/meshes/connect4Board.obj");
    mesh2 = std::make_shared<MeshFile>("../assets/meshes/connect4Puck1.obj");
    mesh3 = std::make_shared<MeshFile>("../assets/meshes/connect4Puck2.obj");
    std::vector<std::filesystem::path> shaderSrcs2 = {
        "../assets/shaders/mesh_frag.glsl",
        "../assets/shaders/mesh_vert.glsl"
    };
    AssetManager::addShader(std::make_shared<Shader>("testMesh", shaderSrcs2));

    std::vector<std::filesystem::path> shaderSrcs = {
        "../assets/shaders/test_frag.glsl",
        "../assets/shaders/test_vert.glsl"
    };
    AssetManager::addShader(std::make_shared<Shader>("test", shaderSrcs));

    auto winSize = App::getWindowSize();
    auto camera = std::make_shared<Camera>(40.f, (float)winSize.x/(float)winSize.y, 0.1f, 1000.f);
    camera->setPosition({0, 7, 20});
    camera->setFocusPoint({0,6,0});
    camera->pointAt({0,6,0});
    Renderer::setCamera(camera);
    auto renderable = std::make_shared<TexturedQuad>("BasicQuad", AssetManager::getShader("test"), MAX_VERTEX_BUFFER_SIZE);
    Renderer::addRenderable(renderable);
    auto renderable2 = std::make_shared<Mesh>("MeshTest", AssetManager::getShader("testMesh"), MAX_VERTEX_BUFFER_SIZE);
    Renderer::addRenderable(renderable2);
    for(int i=0;i<7; i++)
        hPositions[i] = leftSlot + i*hOffset;
    for(int i=0;i<6; i++)
        vPositions[i] = bottom   + i*vOffset;
}

void TestConnect4::onUpdate(float dt)
{
    auto mouseRay = Renderer::getCamera()->getMouseRay();
    auto cameraPos = Renderer::getCamera()->getPos();
    vec3 intersection = {};


    Renderer::begin("MeshTest");
    if(!App::getMouseButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
    {
        if(intersectPlane({0, 0, -1}, {0,0,0.45}, cameraPos, mouseRay, intersection))
        {
            for(int i=0; i<7; i++)
                if(abs(intersection.x - hPositions[i])<=hOffset/2 && intersection.y>0 && intersection.y<10)
                {
                    Renderer::DrawMesh(translate(mat4(1.0f), {hPositions[i],11,-0.45}), rotate(mat4(1.0f), radians(90.f), { 1.0f, 0.0f, 0.0f }), mat4(1), mesh3, {0.906, 0.878, 0.302,0.2});
                }
        }
    }

    Renderer::DrawMesh({0,0,0}, {1,1,1}, mesh1, {0.165, 0.349, 1.000,1});
    Renderer::DrawMesh({1,0,3}, {1,1,1}, mesh2, {0.882, 0.192, 0.161,1});
    Renderer::end();


    Renderer::begin("BasicQuad");
    Renderer::DrawQuad({0,0,0}, {20, 20}, {0.094, 0.141, 0.176,1});
    Renderer::end();
}

