#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "objects/sim_object.h"
#include "objects/triangle.h"

// settings
static const unsigned int SCR_WIDTH = 800;
static const unsigned int SCR_HEIGHT = 800;

void render(GLFWwindow*);
void processInput(GLFWwindow* window);
void errorCallback(int error, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

/// Creates a RocketSim Visualizer window.
/// Returns a pointer to this window.
/// Window is owned by GLFW. Free using glfwDestroyWindow()
/// 
/// Properties:
/// - Window dim = SCR_HEIGHT x SCR_WIDTH
/// - Window title is "RocketSim Visualizer"
/// - Context is selected on return.
///
/// Returns NULL on failure.
GLFWwindow* createWindow();

/**
 * @brief Creates the Visualizer shader program.
 * @return 0 if failed, int otherwise.
 */
GLuint createShader();

int main() {
    printf("Starting Visualizer...\n");

    // init GLFW
    // glfw: init and configure
    if (!glfwInit()) {
        printf("Failed to load GLFW.\n");
        return EXIT_FAILURE;
    }

    // get window
    GLFWwindow* window = createWindow();
    if (window == NULL) {
        fprintf(stderr, "Failed to create Visualizer window!\n");   
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to load glad.\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // set error callback
    glfwSetErrorCallback(errorCallback);
    // sync to monitor
    glfwSwapInterval(1);

    // create triangle
    TriangleObj* triangle = createTriangle();

    // create shader
    GLuint shaderProgram = createShader();
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glUseProgram(shaderProgram);
        drawTriangle(triangle);

        // glfw: swap buffers & poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    destroyTriangle(triangle);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return EXIT_SUCCESS;
}

void render(GLFWwindow*) { }

void processInput(GLFWwindow* window) {
    // handle escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void errorCallback(int, const char* description) { fputs(description, stderr); }

void framebufferSizeCallback(GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); }

GLFWwindow* createWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RocketSim Visualizer", NULL, NULL);
    if (!window) {
        printf("Failed to initialize RocketSim Visualizer window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return window;
}

GLuint createShader() {
    // create shaders
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex Shader Compilation Failed:\n%s", infoLog);
        return 0;
    }
    const char* fragShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        printf("Fragment Shader Compilation Failed:\n%s", infoLog);
        return 0;
    }

    // create shader
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Error failed to link shader program:\n%s", infoLog);
        return 0;
    }
    glDeleteShader(vertexShader); // delete old shader
    glDeleteShader(fragShader); // delete old shader

    return shaderProgram;
}
