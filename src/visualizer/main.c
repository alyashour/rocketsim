#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void render(GLFWwindow*);
void processInput(GLFWwindow* window);
void errorCallback(int error, const char* description);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
    printf("Starting Visualizer...\n");

    // glfw: init and configure
    if (!glfwInit()) {
        printf("Failed to load GLFW.\n");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // glfw create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RocketSim Visualizer", NULL, NULL);
    if (!window) {
        printf("Failed to initialize GLFW window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to load glad.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // set error callback
    glfwSetErrorCallback(errorCallback);
 
    // sync to monitor
    glfwSwapInterval(1);

    // redner loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        render(window);

        // glfw: swap buffers & poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    // exit
    exit(EXIT_SUCCESS);
}

void render(GLFWwindow*) { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void processInput(GLFWwindow* window) {
    // handle escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void errorCallback(int, const char* description) { fputs(description, stderr); }


void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}