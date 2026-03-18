#include <glad.h>
#include <glfw3.h>

#include <shaderClass.cpp>
#include <iostream>
#include <cmath>  // Required for sin()

// Callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ============================================================================
// SHADER SOURCES -- for simplicity, we've put them in the shader header file
// ============================================================================
// ============================================================================
// MAIN
// ============================================================================

int main()
{
    // ------------------------------------------------------------------------
    // 1. GLFW: Initialize and configure
    // ------------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // ------------------------------------------------------------------------
    // 2. GLFW: Create window
    // ------------------------------------------------------------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ------------------------------------------------------------------------
    // 3. GLAD: Load OpenGL function pointers
    // ------------------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");// this is just a naming fashion (could make it any thing)

    // ------------------------------------------------------------------------
    // 5. SET UP VERTEX DATA
    // ------------------------------------------------------------------------

    // First triangle (left side)

    float vertices[] = {
        // positions         // colors (for vertex shader)
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  //bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, //bottom left
        0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f  //top
    };

    // Create VAOs and VBOs
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Configure first triangle
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute: location 0, 3 floats, stride 3*sizeof(float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // glBindVertexArray(0); 
    // the unbinding the VAO could be achived afterwards (for cleanliness of course)
    // just because the other VAO calss won't modify this VAO (happens accidentallly). 
    // A rare situation, but better safe than sorry


    // ------------------------------------------------------------------------
    // 6. RENDER LOOP
    // ------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ------------------------------------------------------------------------
    // 7. CLEANUP
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram); is now handled by the shader class

    glfwTerminate();
    return 0;
}

// Process input: close window on ESC
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Resize viewport when window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}