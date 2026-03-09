#include "../dependencies/glad/include/glad/glad.h"
#include "../dependencies/imageLib/stb_image.h"
#include "camera/camera.h"
#include "shader/shader.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <cglm/util.h>
#include <cglm/vec3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
int width = 800;
int height = 600;

void processInput(GLFWwindow *window);
void framebuffer_callback(GLFWwindow *window, int chwidth, int chheight);

void cursor_callback(GLFWwindow *window, double xpos, double ypos);
float mixValue = 0.2f;
vec3 cubePositions[] = {
    {0.0f, 0.0f, -1.0f},  {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},  {-3.8f, -2.0f, -12.3f},
    {2.4f, -0.4f, -3.5f}, {-1.7f, 3.0f, -7.5f}, {1.3f, -2.0f, -2.5f},   {1.5f, 2.0f, -2.5f},
    {1.5f, 0.2f, -1.5f},  {-1.3f, 1.0f, -1.5f}, {-1.4f, -1.0f, -15.3f}, {-2.4f, -2.0f, -10.4f},
};
vec3 camPos = {0.0f, 0.0f, 3.0f};
vec3 origin = {0.0f, 0.0f, -1.0f};
vec3 up = {0.0f, 1.0f, 0.0f};

int main() {
    GLFWwindow *window;
    if (!glfwInit()) {
        printf("failed to init the glfw\n");
        return -1;
    }
    window = glfwCreateWindow(width, height, "Lee", NULL, NULL);
    if (window == NULL) {
        printf("failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("failed to load the glad\n");
        return -1;
    }
    float camSpd = 0.3f;

    CusCamera *cam;
    cam = camera_create(camPos, origin, width, height, camSpd);
    camera_set_fov(cam, 50.0f);
    glfwSetWindowUserPointer(window, cam);
    glfwSetFramebufferSizeCallback(window, framebuffer_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, camera_cursor_callback);
    glfwSetScrollCallback(window, camera_scroll_callback);
    camera_set_scrollLimit(cam, 30.0f);
    camera_set_sen(cam, 0.4f);
    camera_set_Spd(cam, 0.8f);
    float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
                        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
                        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); */
    /* glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes,
     * GL_STATIC_DRAW); */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    Shader *s = shader_create("shaders/rectangle/vertex.glsl", "shaders/rectangle/fragment.glsl");

    if (s == NULL) {
        perror("Error");
        printf("failed to create the shader");
        return -1;
    }
    int imgWidth, imgHeight, nrChannels;
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data =
        stbi_load("/home/sekiro/MyStuff/playingWithOpenGl/src/assets/container.jpg", &imgWidth,
                  &imgHeight, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("failed to load the texture\n");
    };
    stbi_image_free(data);
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load("/home/sekiro/MyStuff/playingWithOpenGl/src/assets/awesomeface.png", &imgWidth,
                     &imgHeight, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("failed to load the second texture\n");
    };
    stbi_image_free(data);
    shader_use(s);
    shader_setInt(s, "texture1", 0);
    shader_setInt(s, "texture2", 1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        camera_processInput(cam, window);
        camera_update(cam);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        shader_use(s);
        unsigned int viewLoc = glGetUniformLocation(s->ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (void *)cam->view);
        unsigned int projectionLoc = glGetUniformLocation(s->ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (void *)cam->projection);
        shader_setFloat(s, "opacity", sin(glfwGetTime()) * sin(glfwGetTime()));
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 12; i++) {
            mat4 model = GLM_MAT4_IDENTITY_INIT;
            glm_translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            vec3 move = {0.0f, 1.0f, 0.0f};
            glm_rotate(model, glm_rad(angle * (float)glfwGetTime()), move);
            unsigned int modelLoc = glGetUniformLocation(s->ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (void *)model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    free(cam);
    glfwTerminate();
    free(s);
    return 0;
};

void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        mixValue -= 0.01f;
        if (mixValue < 0.0f) {
            mixValue = 0.0f;
        }
    }
};

void framebuffer_callback(GLFWwindow *window, int chwidth, int chheight) {
    glViewport(0, 0, chwidth, chheight);
    width = chwidth;
    height = chheight;
    CusCamera *cam = (CusCamera *)glfwGetWindowUserPointer(window);
    cameraViewPortProcess(cam, chwidth, chheight);
};
