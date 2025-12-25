// TODO: add EBO

#include "triangle.h"
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const float TRIANGLE_VERTICES[] = {
   -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f, 
    0.0f, 0.5f, 0.0f 
};

struct TriangleObj {
    unsigned int vao;
    unsigned int vbo;
};

TriangleObj* createTriangle() {
    TriangleObj* t = malloc(sizeof(TriangleObj));

    // VAO time baby
    glGenVertexArrays(1, &t->vao);

    // bind VAO & save triangle
    glBindVertexArray(t->vao);

    // create triangle vbo
    glGenBuffers(1, &t->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, t->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TRIANGLE_VERTICES), TRIANGLE_VERTICES, GL_STATIC_DRAW);

    // attrib pointers
    glVertexAttribPointer(
        0, // first attrib in the vertex shader
        3, // size of vertex attrib (3 because vec3) 
        GL_FLOAT, // data type
        GL_FALSE, // should the data be normalized?
        3 * sizeof(float), // the stride between consecutive attributes
        (void*) 0
    );
    glEnableVertexAttribArray(0);

    return t;
}

void drawTriangle(const TriangleObj* const t) {
    glBindVertexArray(t->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0); // unbind after
}

/**
 * @brief Destroys all data that the triangle owns on the GPU
 * @invariant relevant context is current
 * @param t 
 */
void destroyTriangle(TriangleObj* const t) {
    if (!t) return;

    if (t->vbo)
        glDeleteBuffers(1, &t->vbo);

    if (t->vao)
        glDeleteVertexArrays(1, &t->vao);

    t->vbo = 0;
    t->vao = 0;

    free(t);
}
