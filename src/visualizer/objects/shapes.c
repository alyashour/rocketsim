#include "shapes.h"
#include "vobject.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const float TRIANGLE_VERTICES[] = {
   -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f, 
    0.0f, 0.5f, 0.0f 
};

VObject createDefaultTriangle() {
    return createTriangle9f(
        -0.5, -0.5, 0.0f,
        0.5, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    );
}

VObject createTriangle(float vertices[static 9]) {
    VObject v = createVObject();

    v.count = 3; // 3 points
    v.drawMode = GL_TRIANGLES;

    // VAO time baby
    glGenVertexArrays(1, &v.vao);

    // bind VAO & save triangle
    glBindVertexArray(v.vao);

    // create triangle vbo
    glGenBuffers(1, &v.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, v.vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

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

    return v;
}

VObject createTriangle9f(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3
) {
    const float vertices[] = {
        x1, y1, z1,
        x2, y2, z2,
        x3, y3, z3 
    };

    return createTriangle(vertices);
}
