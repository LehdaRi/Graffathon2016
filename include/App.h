#ifndef APP_H
#define APP_H

#include "Window.h"
#include "Canvas.h"
#include "Mesh.h"
#include "GLObjects.h"
#include "Camera.hpp"
#include "Building.hpp"
#include "Renderer.hpp"
#include "TransformationVisitor.hpp"
#include "BSpline.h"

#include <cstdint>

//--------------------

class App {
public:
	App(int argc, char* argv[], MainWindow& window);
	App(const App&) = delete;

	App& operator=(const App&) = delete;

public:
	void     loop               (void);

private:
	void     postprocess        (const GL::Texture& input, int width, int height, GLuint framebuffer = 0);

private:
	// Framework objects.
	MainWindow&         window_;
	Canvas              canvas_;
	Camera              camera_;
	GL::ShaderProgram   shader_;
	GL::ShaderProgram postprocess_shader_;
	GL::Texture         image_;
	GL::Texture         depth_;
	GL::FBO             framebuffer_;
	double              time_;

	// NodeComponentStuffWtfThx1338
	Renderer                renderer_;
    TransformationVisitor   transVisitor_;
	std::vector<Building>   buildings_;

	// Spline.
	BSpline spline_;

	// Skybox.
	GL::Texture         cubemap_;
};
#endif // APP_H
