#include "../dependencies/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstdio>

void frambuffer_callback_window(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

float vertices[] = {0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

float rectangle[] = {
    // top left,bottom left ,bottom right, top right
    0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f,

};

int indexs[] = {0, 1, 3, 1, 2, 3};

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout(location=0) in vec3 aPos;\n"
                                 "void main(){\n"
                                 "gl_Position=vec4(aPos,1.0);\n"
                                 "}\0";

const char *fragShaderSource = "#version 330 core\n"
                               "out vec4 FragColor;\n"
                               "void main(){\n"
                               "FragColor=vec4(1.0f,0.5f,0.2f,1.0f);\n"
                               "};\0";

int main() {

    GLFWwindow *window;
    if (!glfwInit()) {
        printf("Failed to initialize the glfw\n");
        return -1;
    }

    window = glfwCreateWindow(800, 600, "LittleTriangle", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create the window \n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to load up the glad\n");
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, frambuffer_callback_window);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("failed to compile the vertex shader %s \n", infoLog);
    };

    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);

    int fragSuccess;
    char fragInfoLog[512];
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragSuccess);
    if (!fragSuccess) {
        glGetShaderInfoLog(fragShader, 512, NULL, fragInfoLog);
        printf("failed to compile the frag shader %s\n", fragInfoLog);
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
};

void frambuffer_callback_window(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
};

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
