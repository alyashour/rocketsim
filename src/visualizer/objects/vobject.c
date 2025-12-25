#include "vobject.h"

#include <glad/glad.h>

VObject createVObject() {
    return (VObject) {
        .vbo = 0,
        .vao = 0,
        .ebo = 0,
        .drawMode = GL_TRIANGLES,
    };
}

void drawVObject(const VObject* const o) {
    glBindVertexArray(o->vao);
    glDrawArrays(o->drawMode, 0, o->count);
    glBindVertexArray(0); // unbind after
}

/**
 * @brief Destroys all data that the triangle owns on the GPU.
 * @invariant relevant context is current.
 * Sets all VObject fields to 0.
 * 
 * @warning DOES NOT FREE POINTER. Call `free()` manually if needed.
 * @param o Object to be destroyed
 */
void destroyVObject(VObject* const o) {
    if (!o) return;

    if (o->vbo)
        glDeleteBuffers(1, &o->vbo);

    if (o->vao)
        glDeleteVertexArrays(1, &o->vao);

    o->vbo = 0;
    o->vao = 0;
}
