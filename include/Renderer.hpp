#ifndef RENDERER_HPP
#define RENDERER_HPP


#include "Visitor.hpp"
#include "MeshComponent.hpp"


class Camera;


class Renderer : public Visitor<Renderer, MeshComponent> {
public:
    Renderer(Camera& camera, uint32_t width, uint32_t height,
             GL::ShaderProgram& shader, GL::FBO& framebuffer);

    void operator()(MeshComponent& component);

private:
    Camera&             camera_;
    uint32_t            width_;
    uint32_t            height_;

    GL::ShaderProgram&  shader_;
    GL::FBO&            framebuffer_;
};


#endif // RENDERER_HPP
