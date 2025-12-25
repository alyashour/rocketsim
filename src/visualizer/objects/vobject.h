/**
 * VObject is a visualizer object that can immediately be drawn.
 * It has already been buffered into the graphics card.
 */
#ifndef RS_VOBJECT_H
#define RS_VOBJECT_H

#include <glad/glad.h>

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint drawMode;
    GLsizei count; // num of primitives that make up object
} VObject;

VObject createVObject();

void drawVObject(const VObject* const o);

void destroyVObject(VObject* const o);

#endif // RS_VOBJECT_H
