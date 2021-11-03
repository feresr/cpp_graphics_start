#include <iostream>
#include <SDL.h>
#include <glad/glad.h>

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
    while (true)
    {
        time += 0.01f;
        glClearColor(sin(time), cos(time), sin(time * 0.3f), 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_PollEvent(&event);
        if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            break;
        if (event.type == SDL_QUIT)
            break;
        SDL_GL_SwapWindow(window);
    }
}
