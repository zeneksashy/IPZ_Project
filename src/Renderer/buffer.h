﻿#pragma once
#include <glm.hpp>
#include "glad/glad.h"
#include <vector>
#include <memory>

using namespace glm;

class BufferElement{
    friend class BufferLayout;
    friend class VertexArray;

public:
    enum DataType{
        Int,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        };

    BufferElement(DataType type, const char* name, bool normalized = false)
        : name(name), type(type), normalized(normalized){}

    inline static uint typeComponentCount(DataType type){
        switch (type)
        {
        case Int:    return 1;
        case Float:  return 1;
        case Float2: return 2;
        case Float3: return 3;
        case Float4: return 4;
        case Mat3:   return 3;
        case Mat4:   return 4;
        }
        return 0;
    }
    inline static uint typeComponentSize(DataType type){
        switch (type)
        {
        case Int:    return 4;
        case Float:  return 4;
        case Float2: return 4*2;
        case Float3: return 4*3;
        case Float4: return 4*4;
        case Mat3:   return 4*9;
        case Mat4:   return 4*16;
        }
        return 0;
    }

    inline static uint typeToNative(DataType type){
        switch (type)
        {
        case Int:    return GL_INT;
        case Float:
        case Float2:
        case Float3:
        case Float4:
        case Mat3:
        case Mat4:   return GL_FLOAT;
        }
        return 0;
    }

private:
    const char* name;
    const DataType type;
    uint size = 0;
    uint offset = 0;
    bool normalized = false;
};



class BufferLayout{
    friend class VertexArray;
public:
    BufferLayout(std::initializer_list<BufferElement> elements)
        : elements(elements)
    {
        processElements();
    }

    uint stride(){return m_stride;}
    void appendElement(BufferElement element);

private:
    uint m_stride = 0;
    std::vector<BufferElement> elements;

    void processElements();

};



class VertexBuffer{
    friend class VertexArray;

public:
    VertexBuffer(const BufferLayout& layout, size_t size, void* data = nullptr);
    ~VertexBuffer();

    void bind();
    void unbind();
    void setData(const void* data, size_t size);
    size_t size(){return m_size;}
    BufferLayout layout(){return m_layout;}

private:
    uint id = 0;
    size_t m_size = 0;
    BufferLayout m_layout;

};



class IndexBuffer{

public:
    IndexBuffer(size_t size, uint16* indices = nullptr);
    ~IndexBuffer();

    void bind();
    void unbind();
    void setData(const uint16* data, uint size);
    uint count(){return m_size/sizeof(uint16);}

private:
    uint id = 0;
    uint m_size = 0;
};



class VertexArray{

public:
    VertexArray(std::initializer_list<std::shared_ptr<VertexBuffer>> vBuffers,
                std::shared_ptr<IndexBuffer> iBuffer);
    VertexArray();
    ~VertexArray();

    void bind();
    void unbind();

    void addVBuffer(std::shared_ptr<VertexBuffer> buffer);
    void popVBuffer(); // remove last added vBuffer
    void setIBuffer(std::shared_ptr<IndexBuffer> buffer);
    void clearVBuffers();

    std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers(){return vBuffers;}
    std::shared_ptr<IndexBuffer> indexBuffer(){return iBuffer;}

private:
    uint id = 0;
    std::vector<std::shared_ptr<VertexBuffer>> vBuffers;
    std::shared_ptr<IndexBuffer> iBuffer;
    uint vBufferIndex = 0;

};

struct FrameBufferAttachment
{
    GLenum format = 0;
    GLenum type = 0;
    bool renderBuffer = true;
};


class Texture;
class FrameBuffer
{
    //TODO: fill out the functions, and remember to blit to screen for now
public:
    FrameBuffer(){};
    FrameBuffer(uint width, uint height, std::vector<FrameBufferAttachment> colorAtachments,
                FrameBufferAttachment depthAttachment = {}, uint samples = 1);
    void resize(uint width, uint height);
    void bind();
    void bind(std::vector<GLenum> attachments);
    void unbind();
    void blitToFrontBuffer();

private:
    void update();

    uint id      = 0;
    uint width   = 0;
    uint height  = 0;
    uint samples = 1;
    int maxAttachments = 0;

    std::vector<FrameBufferAttachment> colorAttachments;
    FrameBufferAttachment depthAttachment = {};

    std::vector<uint> rboIds;

    std::vector<std::shared_ptr<Texture>> attachedTextures; //give weak pointer if we take from here
    std::shared_ptr<Texture> depthTexture = nullptr; //could be depth+stencil
};
