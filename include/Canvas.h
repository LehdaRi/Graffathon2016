#ifndef CANVAS_H
#define CANVAS_H

#include "GLObjects.h"

//--------------------

class Canvas {
public:
        Canvas(void);
        Canvas(const Canvas&) = delete;

        Canvas& operator=(const Canvas&) = delete;
public:
        GL::Buffer position_buffer_;
        GL::Buffer texcoord_buffer_;
        GL::VAO vao_;
        size_t num_vertices_;
        GLenum primitive_type_;
};

#endif // CANVAS_H
