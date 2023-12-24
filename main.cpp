#include <iostream>
#include <fstream>
#include <SDL.h>
#include <glad/glad.h>
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"

#include "Camera.h"

constexpr int FPS = 60;
constexpr double FRAME_DURATION = 1000.0f / FPS;

Camera camera = Camera(glm::vec3(0.0, 0.0, 3.0));

bool processEvent(SDL_Event &event)
{
    camera.processEvent(event);
    if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        return true;
    if (event.type == SDL_QUIT)
        return true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        glViewport(0, 0, event.window.data1 * 2.0, event.window.data2 * 2.0);
    }

    return false;
}

int main(int, char **)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 0;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    std::string name{"3D Graphics"};
    auto *window = SDL_CreateWindow(
        name.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window)
    {
        SDL_Log("Unable to create window SDL: %s", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    // set window properties
    SDL_SetWindowResizable(window, SDL_TRUE);
    SDL_SetWindowMinimumSize(window, 256, 256);

    // enable v-sync
    SDL_GL_SetSwapInterval(1);
    void *context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    gladLoadGL();

    SDL_Event event;
    float time = 0;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    // Vertex shader
    auto shaderProgram = Shader("../vertex.glsl", "../fragment.glsl");

    int resolution[2];
    SDL_GL_GetDrawableSize(window, &resolution[0], &resolution[1]);
    SDL_ShowCursor(0);
    float resolutionf[] = {(float)resolution[0], (float)resolution[1]};

    shaderProgram.use();
    shaderProgram.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    auto location = glGetUniformLocation(shaderProgram.id, "resolution");
    glUniform2fv(location, 1, resolutionf);

    /* Transforms */

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480, 0.1f, 100.0f);
    // trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

    uint32_t frameStart, frameTime;
    GLenum err;
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // shaderProgram.setInt("buzz", 0); // set 0 to "uniform / sampler2d " to it matche to glActiveTexture(GL_TEXTURE)
    // shaderProgram.setInt("woody", 1);
    glViewport(0, 0, 640 * 2, 480 * 2);

    glm::mat4 model = glm::mat4(1.0f);

    Model bp("../assets/backpack/backpack.obj");

    while (true)
    {
        frameStart = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        static glm::vec3 lightPos = glm::vec3{1.2f, 1.0f, 2.0f};
        shaderProgram.use();
        shaderProgram.setVec3("lightPos", lightPos);
        shaderProgram.setVec3("cameraPos", camera.position);
        shaderProgram.setFloat("time", time);
        shaderProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        shaderProgram.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        shaderProgram.setVec3("light.specular", 0.4f, 0.4f, 0.4f);
        shaderProgram.setInt("material.diffuse", 0);
        shaderProgram.setInt("material.specular", 1);

        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 2.5f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(camera.getView()));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(model));

        bp.Draw(shaderProgram);

        while ((err = glGetError()) != GL_NO_ERROR)
            std::cout << err << std::endl;

        bool quit = false;
        while (SDL_PollEvent(&event))
        {
            if (processEvent(event))
            {
                quit = true;
                break;
            }
        }
        if (quit)
            break;
        SDL_GL_SwapWindow(window);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DURATION)
        {
            SDL_Delay(FRAME_DURATION - frameTime);
        }
        time += FRAME_DURATION / 1000;
    }
    SDL_Quit();
}