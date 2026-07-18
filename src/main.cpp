#include <iostream>
#include <GLAD/include/glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <time.h>

#include "hangman.hpp"

void processInput(GLFWwindow* window);
void createShader(unsigned int* shader, auto shader_type, const char** shader_source);

struct WindowSize {
    int w = 1280;
    int h = 720;
} _windowSize;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.1f, 0.15f, 0.15f, 1.0f);\n"
    "}\0"; 

int main()
{
    
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Set glfw options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Prevent resizing
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(_windowSize.w, _windowSize.h, "Hangman", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    assert(status && "Failed to initailize Glad");
    // Set viewport
    glViewport(0, 0, _windowSize.w, _windowSize.h);

    // Load vertex shader
    unsigned int vertexShader;
    createShader(&vertexShader, GL_VERTEX_SHADER, &vertexShaderSource);
    // Load fragment shader
    unsigned int fragmentShader;
    createShader(&fragmentShader, GL_FRAGMENT_SHADER, &fragmentShaderSource);

    // Create shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Verify shader program created successfully
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Delete shaders since they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // try engle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    };

    // Create Vertex Array Object & Vertex Buffer Object
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the VAO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // set attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    clock_t frameTimer = clock();
    float fps = 0.1f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Difference in time between loops converted to seconds
        float frameTime = ((float)(clock() - frameTimer))/CLOCKS_PER_SEC;
        // Check if its been higher than our goal frame time, if it is process the frame
        if(frameTime >= fps){
            processInput(window);
            // Set background color
            glClearColor(0.9f, 0.85f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // Set up shader and VAO
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            // Draw
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);
            glfwPollEvents();
            frameTimer = clock();
        }
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS){
        std::cout << "Mouse button pressed" << std::endl;
    }
}

// Helper function for compiling shaders -- runs a check to make sure they compile correctly
void createShader(unsigned int* shader, auto shader_type, const char** shader_source){
    // compile shader
    *shader = glCreateShader(shader_type);
    glShaderSource(*shader, 1, shader_source, NULL);
    glCompileShader(*shader);
    // Verify shader was compiled correctly    
    int  success;
    char infoLog[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    // Print the error if not
    if(!success)
    {
        const char* shader_type_str;
        switch(shader_type) {
            case GL_VERTEX_SHADER:
                shader_type_str = "VERTEX";
                break;
            case GL_FRAGMENT_SHADER:
                shader_type_str = "FRAGMENT";
                break;
            default:
                std::cout << "ERROR::SHADER\n INVALID SHADER TYPE" << std::endl;
                exit(1);
        }
        glGetShaderInfoLog(*shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shader_type_str << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        // Nothing to do here after shader fais to compile so just exit for now
        exit(1);
    }
}
