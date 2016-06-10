#ifndef APP_H
#define APP_H

//#include "Window.h"
#include "Canvas.h"
#include "Mesh.h"
#include "GLObjects.h"
#include "HeadScene.h"

#include <SFML/Window.hpp>
#include <cstdint>

//--------------------

class App {
public:
	App(int argc, char* argv[], sf::Window& window);
	App(const App&) = delete;

	App& operator=(const App&) = delete;

public:
	void     loop               (void);

private:
	void     raymarch           (int width, int height, GLuint framebuffer = 0);
	void     render_texture     (const GL::Texture& texture, int width, int height, GLuint framebuffer = 0);
	void     render_mesh        (const Mesh& mesh, int width, int height, GLuint framebuffer = 0);
	void     render_on_cube     (const GL::Texture& texture, int width, int height, GLuint framebuffer = 0);
	void     render_on_torus    (const GL::Texture& texture, int width, int height, GLuint framebuffer = 0);
    void     handleEvents       (void);

private:
	// Framework objects.
	sf::Window&       window_;
	uint32_t          ww_, wh_;
	Canvas            canvas_;
	GL::ShaderProgram mesh_shader_;
	GL::ShaderProgram raymarcher_shader_;
	GL::Texture       image_;
	GL::Texture       depth_;
	GL::FBO           framebuffer_;
	double            time_;

	// Head.
	HeadScene head_scene_;
	bool      normals_from_texture_;

	// Skybox.
	Mesh        cube_;
	Mesh        torus_;
	GL::Texture cubemap_;
};
#endif // APP_H
