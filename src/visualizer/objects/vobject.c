#include "vobject.h"

#include <

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
    if (!t) return;

    if (t->vbo)
        glDeleteBuffers(1, &t->vbo);

    if (t->vao)
        glDeleteVertexArrays(1, &t->vao);

    t->vbo = 0;
    t->vao = 0;
}
