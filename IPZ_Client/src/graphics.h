#pragma once
#define PI 3.14159265358979323846
#include "olcPixelGameEngine.h"
#define ASSERT_NOT_REACHED _ASSERT_EXPR(true, "ASSERT_NOT_REACHED");

namespace IPZ {

struct vec3{
    float x,y,z;
};

struct Camera{
    olc::vf2d pos {0,0};
    float angle = 0;
    float pitch = 5.5f;
    float zoom  = 5;
};

enum Orientation{
    North,
    South,
    East,
    West,
    Up,
    Down
};

struct Sprite{
    olc::Sprite* sprite = nullptr;
    olc::Decal*  decal  = nullptr;
    olc::vf2d pos;
    vec3 points3d[4];

    Sprite()
    {
        sprite = new olc::Sprite();
        decal  = new olc::Decal(sprite);
    }
    Sprite(const std::string& file)
    {
        sprite = new olc::Sprite(file);
        decal  = new olc::Decal(sprite);
    }
    ~Sprite()
    {
        delete sprite;
        delete decal;
    }

    int32_t width()
    {
        return sprite->width;
    }
    int32_t height()
    {
        return sprite->height;
    }

    void translate3d(Camera camera, olc::vi2d screenSize, Orientation orientation = Up)
    {
        switch (orientation) {
        case Up:
            points3d[2] = { 0.0f, 0.0f, camera.zoom * (float)sprite->width };
            points3d[3] = { 0.0f, 0.0f, 0.0f };
            points3d[0] = { camera.zoom *(float)sprite->width, 0.0f, 0.0f };
            points3d[1] = { camera.zoom *(float)sprite->width, 0.0f, camera.zoom *(float)sprite->width };
            break;
        case Down:
        case North:
        case South:
        case East:
        case West:
            ASSERT_NOT_REACHED;
        }

        float sinAngle = sin(camera.angle);
        float cosAngle = cos(camera.angle);
        float sinPitch = sin(camera.pitch);
        float cosPitch = cos(camera.pitch);
//        vec3 camPos = {camera.pos.x, 0, camera.pos.y};

        for(int i=0; i<4; i++)
        {
            points3d[i].x += pos.x * camera.zoom - camera.pos.x;
            points3d[i].z += pos.y * camera.zoom - camera.pos.y;

            float x = points3d[i].x;
            float z = points3d[i].z;

            points3d[i].x = x * cosAngle + z * sinAngle;
            points3d[i].z = x * -sinAngle + z * cosAngle;

            points3d[i].y = points3d[i].y * cosPitch - points3d[i].z * sinPitch;
            points3d[i].z = points3d[i].y * sinPitch + points3d[i].z * cosPitch;

            points3d[i].x += screenSize.x * 0.5f;
            points3d[i].y += screenSize.y * 0.5f;
        }
    }
    std::array<olc::vf2d, 4> getWarpedPoints()
    {
        std::array<olc::vf2d, 4> points;
        for(int i=0; i<4; i++)
            points[i] = {points3d[i].x, points3d[i].y};
        return points;
    }
};




}



