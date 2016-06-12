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
	//void     raymarch           (int width, int height, GLuint framebuffer = 0);
	void     render_texture        (const GL::Texture& texture, int width, int height, GLuint framebuffer = 0);
	//void     render_mesh           (const Mesh& mesh, int width, int height, GLuint framebuffer = 0);
	//void     render_mesh_on_spline (const Mesh& mesh, int width, int height, GLuint framebuffer = 0);
	//void     render_on_cube        (const GL::Texture& texture, int width, int height, GLuint framebuffer = 0);
	//void     render_on_torus       (const GL::Texture& texture, int width, int height, GLuint framebuffer = 0);


private:
	// Framework objects.
	MainWindow&         window_;
	Canvas              canvas_;
	Camera              camera_;
	GL::ShaderProgram   shader_;
	GL::Texture         image_;
	GL::Texture         depth_;
	GL::FBO             framebuffer_;
	double              time_;

	// CodeComponentStuffWtf
	Renderer                renderer_;
    TransformationVisitor   transVisitor_;
	std::vector<Building>   buildings_;

	// Spline.
	BSpline spline_;

	// Skybox.
	GL::Texture         cubemap_;
};
#endif // APP_H
