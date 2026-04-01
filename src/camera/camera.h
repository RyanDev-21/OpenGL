#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef enum { Yaw, Pitch, Row, All } AllowMovement;
typedef struct {
    vec3 camPos;
    vec3 origin;
    vec3 up;
    mat4 view;
    mat4 projection;
    float yaw; // naturally looking down
    float pitch;
    float fov;
    float camSpd;
    float lastX;
    float lastY;
    int width;
    int height;
    AllowMovement allowField;
    float lastFrame;
    bool firstMouse;
    float scrollLimit;
    float sen;
} CusCamera;
CusCamera *camera_create(vec3 camPos, vec3 origin, float width, float height, float camSpd);
void camera_update(CusCamera *cam);
void camera_set_fov(CusCamera *cam, float fov);
void camera_set_scrollLimit(CusCamera *cam, float scrollLimit);
void camera_processInput(CusCamera *cam, GLFWwindow *window);
void camera_activate(CusCamera *cam, AllowMovement field);
void camera_set_Spd(CusCamera *cam, float camSpeed);
void cameraViewPortProcess(CusCamera *cam, int width, int height);
void camera_cursor_callback(GLFWwindow *window, double xpos, double ypos);
void camera_scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
void camera_set_sen(CusCamera *cam, float sen);
#endif
