﻿#include "testconnect4.h"
#include <sstream>
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
Move pickRandomTopMove(std::vector<std::pair<Move, double>> moves) //input sorted
{
    std::pair<Move, double> bestMove = moves.front();
    std::vector<std::pair<Move, double>> bestMoves;
    for(size_t i=0; i<moves.size(); i++)
    {
        auto move = moves[i];
        if(move.second>bestMove.second)
            bestMove = move;
    }
    std::wstringstream stream;
    stream<<"Computer moves: ";
    if(std::isinf(bestMove.second))
        return bestMove.first;

    for(auto move:moves)
    {
        if(move.second>=bestMove.second-abs(bestMove.second*0.05f))
        {
            stream<<"!!("<<move.first.x<<","<<move.first.y<<") g:"<<move.second<<"!! ";
            bestMoves.push_back(move);
        }
        else
            stream<<"  ("<<move.first.x<<","<<move.first.y<<") g:"<<move.second<<"   ";
    }
    std::wcout<<stream.str()<<"\n";
    int random = glm::linearRand<int>(0, bestMoves.size()-1);
    ASSERT(bestMoves.size());
    return bestMoves[random].first;
}
TestConnect4::TestConnect4()
{
    mesh1 = std::make_shared<MeshFile>("../assets/meshes/connect4Board.obj");
    mesh2 = std::make_shared<MeshFile>("../assets/meshes/connect4Puck1.obj");
    mesh3 = std::make_shared<MeshFile>("../assets/meshes/connect4Puck2.obj");

    AssetManager::addAsset(mesh1);
    AssetManager::addAsset(mesh2);
    AssetManager::addAsset(mesh3);


    std::vector<std::filesystem::path> shaderSrcs1 = {
        "../assets/shaders/basic_pbr.fs",
        "../assets/shaders/basic_pbr.vs"
    };
    AssetManager::addShader(std::make_shared<Shader>("basic_pbr", shaderSrcs1));

    pbrShader = AssetManager::getShader("basic_pbr");

    camera = GraphicsContext::getCamera();
    camera->setFov(50.f);
    camera->setPosition({0, 7, 20});
    camera->setFocusPoint({0,6,0});

    auto board = getEntity({mesh1->mesh()}, {0,0,0});
    board->setOverrideColor({0.165, 0.349, 1.000, 1});

    auto puck1 = getEntity({mesh2->mesh()}, {1,0,3});
    puck1->setOverrideColor(red);

    auto puck2 = getEntity({mesh3->mesh()}, {-1,0,3});
    puck2->setOverrideColor(yellow);

    previewPuck = getEntity({mesh3->mesh()}, {0, 0, 0}, quat({radians(90.f), 0, 0}));
    previewPuck->setOverrideColor(yellow);
    previewPuck->overrideColor.a = 0.2f;

    directionalLight.pos = {20, 25, 25};
    directionalLight.color = {1,1,1};

    for(int i=0;i<7; i++)
        hPositions[i] = leftSlot + i*hOffset;
    for(int i=0;i<6; i++)
        vPositions[i] = top      - i*vOffset;

    c4 = new Connect4(7, 6);
    searcher = new alpha_beta_searcher<Move, true>(3,true);

}

void TestConnect4::onStart()
{

}

void TestConnect4::onUpdate(float dt)
{
    auto mouseRay  = camera->getMouseRay();
    auto cameraPos = camera->getPos();
    vec3 intersection = {};

    previewPuck->renderable = false;
    if(!App::getMouseButtonHeld(GLFW_MOUSE_BUTTON_RIGHT) && !animating && c4->userTurn && c4->is_terminal() == std::nullopt)
    {
        if(intersectPlane({0, 0, -1}, {0,0,0.45}, cameraPos, mouseRay, intersection))
        {
            for(int i=0; i<7; i++)
                if(abs(intersection.x - hPositions[i])<=hOffset/2 && intersection.y>0 && intersection.y<10)
                {
                    currentMove = c4->createMove(i);
                    if(currentMove.h_grade>=0)
                    {
                        if(App::getMouseButton(GLFW_MOUSE_BUTTON_LEFT))
                        {
                            puckInPlay = getEntity({mesh3->mesh()}, {hPositions[i],11,-0.45}, quat({radians(90.f), 0, 0}));
                            puckInPlay->setOverrideColor(yellow);
                            animating = true;
                        }
                        else
                        {
                            previewPuck->renderable = true;
                            previewPuck->pos = {hPositions[i],11,-0.45};
                        }
                    }
                }
        }
    }
    if(!animating && !c4->userTurn && c4->is_terminal() == std::nullopt)
    {
        searcher->do_search(*c4);
        auto moves = searcher->get_scores();
        currentMove = pickRandomTopMove(moves);

        puckInPlay = getEntity({mesh3->mesh()}, {0,0,0}, quat({radians(90.f), 0, 0}));
        puckInPlay->setOverrideColor(red);

        animating = true;
    }

    if(animating)
    {
        ASSERT(puckInPlay);
        ySpeed += 20*dt;
        yPos -= ySpeed*dt;

        puckInPlay->pos = {hPositions[currentMove.x], yPos,-0.45};

        if(yPos<=vPositions[currentMove.y])
        {
            animating = false;
            puckInPlay->pos.y = vPositions[currentMove.y];
            puckInPlay = nullptr;
            ySpeed = 0;
            yPos = 11;
            c4->commitMove(currentMove);
        }
    }

}
