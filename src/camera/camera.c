#include "camera.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/vec3.h>

float cal_deltaFrame(CusCamera *cam) {
    float currentFrame = glfwGetTime();
    float deltaFrame;
    deltaFrame = currentFrame - cam->lastFrame;
    cam->lastFrame = currentFrame;
    return deltaFrame;
}

void process_yaw(CusCamera *cam, GLFWwindow *window);
CusCamera *camera_create(vec3 camPosV, vec3 origin, float width, float height, float camSpd) {
    CusCamera *cam = malloc(sizeof(CusCamera));
    float deltaFrame = cal_deltaFrame(cam);
    float camSpeed = camSpd * deltaFrame;

    cam->yaw = -90.0f;
    cam->fov = 20.0f;
    cam->camSpd = 0.01f;
    cam->pitch = 0.0f;
    vec3 upVec = {0.0f, 1.0f, 0.0f};
    cam->lastX = 400.0f;

    cam->lastY = 200.0f;
    cam->sen = 0.01f;
    cam->allowField = All;
    cameraViewPortProcess(cam, width, height);
    glm_vec3_copy(upVec, cam->up);
    glm_vec3_copy(camPosV, cam->camPos);
    glm_vec3_copy(origin, cam->origin);

    vec3 direction;
    // the reason we multiply the cos(pitch) is when the y changes it effects on both
    // x-z plane so need to recalculate
    // when you look upwards the direction of y when it becomes 90 the values beocme 0 so
    // it just automatically streched those x-z coordinate to zero and then when your y
    // axis angel become 0 again it becomes 1
    direction[0] = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
    direction[1] = sin(glm_rad(cam->pitch)); // this is the change in y which is sin (theta)
    direction[2] = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
    glm_normalize_to(direction, cam->origin);

    vec3 sum;
    glm_vec3_add(cam->camPos, cam->origin, sum);
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_lookat(cam->camPos, sum, cam->up, view);
    glm_mat4_copy(view, cam->view);
    mat4 projection;
    glm_perspective(glm_rad(cam->fov), (float)cam->width / (float)cam->height, 0.1f, 100.0f,
                    projection);
    glm_mat4_copy(projection, cam->projection);
    return cam;
};

void camera_set_fov(CusCamera *cam, float fov) { cam->fov = fov; };
void camera_activate(CusCamera *cam, AllowMovement field) { cam->allowField = field; };
void camera_set_Spd(CusCamera *cam, float camSpeed) { cam->camSpd = camSpeed; }
void camera_processInput(CusCamera *cam, GLFWwindow *window) { process_yaw(cam, window); };

void process_yaw(CusCamera *cam, GLFWwindow *window) {
    float localSpeed = cam->camSpd * 0.05;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 scaled;
        glm_vec3_scale_as(cam->origin, localSpeed, scaled);
        glm_vec3_add(cam->camPos, scaled, cam->camPos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 scaled;
        glm_vec3_scale_as(cam->origin, localSpeed, scaled);
        glm_vec3_inv(scaled);
        glm_vec3_add(cam->camPos, scaled, cam->camPos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 cross;
        glm_vec3_cross(cam->origin, cam->up, cross);
        glm_vec3_normalize(cross);
        glm_vec3_scale_as(cross, localSpeed, cross);
        glm_vec3_inv(cross);
        glm_vec3_add(cam->camPos, cross, cam->camPos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 cross;
        glm_vec3_cross(cam->origin, cam->up, cross);
        glm_vec3_normalize(cross);
        glm_vec3_scale_as(cross, localSpeed, cross);
        glm_vec3_add(cam->camPos, cross, cam->camPos);
    }
}
void camera_cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    CusCamera *cam = (CusCamera *)glfwGetWindowUserPointer(window);
    if (cam->firstMouse) {
        cam->lastX = xpos;
        cam->lastY = ypos;
        cam->firstMouse = false;
    }
    float xOffset = xpos - cam->lastX;
    float yOffset = cam->lastY - ypos;
    cam->lastX = xpos;
    cam->lastY = ypos;
    xOffset *= cam->sen * 0.02;
    yOffset *= cam->sen * 0.02;
    if (cam->pitch > 89.0f) {
        cam->pitch = 89.0f;
    } else if (cam->pitch < -89.0f) {
        cam->pitch = -89.0f;
    }
    cam->yaw += xOffset;
    cam->pitch += yOffset;
}
void camera_update(CusCamera *cam) {
    vec3 direction;
    direction[0] = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
    direction[1] = sin(glm_rad(cam->pitch)); // this is the change in y which is sin (theta)
    direction[2] = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
    glm_normalize_to(direction, cam->origin);

    vec3 sum;
    glm_vec3_add(cam->camPos, cam->origin, sum);
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_lookat(cam->camPos, sum, cam->up, view);
    glm_mat4_copy(view, cam->view);
    mat4 projection;
    glm_perspective(glm_rad(cam->fov), (float)cam->width / (float)cam->height, 0.1f, 100.0f,
                    projection);
    glm_mat4_copy(projection, cam->projection);
}
void cameraViewPortProcess(CusCamera *cam, int width, int height) {
    cam->width = width;
    cam->height = height;
    mat4 projection;
    glm_perspective(glm_rad(cam->fov), (float)cam->width / (float)cam->height, 0.1f, 100.0f,
                    projection);
    glm_mat4_copy(projection, cam->projection);
}
void camera_scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    CusCamera *cam = (CusCamera *)glfwGetWindowUserPointer(window);
    cam->fov -= (float)yOffset;
    if (cam->fov < 1.0f) {
        cam->fov = 1.0f;
    } else if (cam->fov > cam->scrollLimit) {
        cam->fov = cam->scrollLimit;
    }
}

void camera_set_scrollLimit(CusCamera *cam, float scrollLimit) { cam->scrollLimit = scrollLimit; }

void camera_set_sen(CusCamera *cam, float sen) { cam->sen = sen; };
