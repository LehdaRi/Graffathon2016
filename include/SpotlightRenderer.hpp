#ifndef SPOTLIGHTRENDERER_HPP
#define SPOTLIGHTRENDERER_HPP


#include "Visitor.hpp"
#include "MeshComponent.hpp"


class Camera;

class SpotlightRenderer : public Visitor<SpotlightRenderer, MeshComponent>
{
public:
    SpotlightRenderer(Camera& camera, uint32_t width, uint32_t height,
	                  GL::ShaderProgram& shader, GL::FBO& framebuffer);

    void operator()(MeshComponent& component);

private:
    Camera&             camera_;
    uint32_t            width_;
    uint32_t            height_;

    GL::ShaderProgram&  shader_;
    GL::FBO&            framebuffer_;
};


#endif // SPOTLIGHTRENDERER_HPP
