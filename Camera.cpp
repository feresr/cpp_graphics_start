#include "Camera.h"

glm::mat4 Camera::getView()
{
    return glm::lookAt(Camera::position, Camera::position + Camera::front, Camera::up);
}

glm::vec2 prevMouse = glm::vec2(0.0, 0.0);
float yaw = 0.0f;
float pitch = 0.0f;

void Camera::processEvent(SDL_Event &event) {
    if (event.type == SDL_MOUSEMOTION) {
        if (prevMouse == glm::vec2(0.0, 0.0)) {
            prevMouse = glm::vec2(event.motion.x, event.motion.y);
        }
        glm::vec2 delta = prevMouse - glm::vec2(event.motion.x, event.motion.y);
        delta = delta * 0.5f;
        yaw -= delta.x;
        pitch += delta.y;
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
        prevMouse = glm::vec2(event.motion.x, event.motion.y);
    }

    if (event.type == SDL_KEYDOWN)
    {
        const float cameraSpeed = 0.01f;
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_S:
            position -= cameraSpeed + front;
            break;
        case SDL_SCANCODE_W:
            position += cameraSpeed + front;
            break;
        case SDL_SCANCODE_A:
            position -= (glm::cross(front, up) * 0.1f);
            position = glm::normalize(front);
            break;
        case SDL_SCANCODE_D:
            break;
        default:
            break;
        }
    }
}