#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "rsm.h"
#include "scene_frame.h"
#include "visualizer/objects/shapes.h"
#include "visualizer/objects/vobject.h"

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
    VObject triangle = createDefaultTriangle();

    // create shader
    GLuint shaderProgram = createShader();
    int triangleColorLocation = glGetUniformLocation(shaderProgram, "triangleColor");
    if (triangleColorLocation == -1) {
        fprintf(stderr, "Failed to get shader uniform location for \"triangleColor\"\n");
        return EXIT_FAILURE;
    }
    int transformMatrixLocation = glGetUniformLocation(shaderProgram, "transform");
    if (transformMatrixLocation == -1) {
        fprintf(stderr, "Failed to get shader uniform location for \"transform\"\n");
        return EXIT_FAILURE;
    }

    // create shared mem
    printf("[VIS] Opening shm with simulation...\n");
    int fd = shm_open("rocketsim", O_RDONLY | O_EXCL, 0666);
    if (fd == -1) {
        fprintf(stderr, "[VIS] shm_open failed.\n");
        return EXIT_FAILURE;
    }
    SceneFrame* scene = mmap(0, sizeof(SceneFrame), PROT_READ, MAP_SHARED, fd, 0);
    if (scene == MAP_FAILED) {
        printf("[VIS] mmap failed.\n");
        fprintf(stderr, "error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        // keyboard input
        processInput(window);

        // create transformation matrix out of scene data
        vec3 v = rsmV3(scene->x, scene->y, scene->z);
        Mat4 identity = rsmM4Identity();
        Mat4 transform = rsmTranslate(&identity, &v);

        // render
        glClearColor(0.1f, 0.0, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glUseProgram(shaderProgram);
        glUniform4f(triangleColorLocation, scene->r, scene->g, scene->b, 1.0f); // set triangle color according to sim
        glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, transform.data);
        drawVObject(&triangle);

        // glfw: swap buffers & poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    destroyVObject(&triangle);
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
        "uniform Mat4 transform;\n"
        "void main() {\n"
        "   gl_Position = transform * vec4(aPos, 1.0);\n"
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
        "uniform vec4 triangleColor;\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = triangleColor;\n"
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
