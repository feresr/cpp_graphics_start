#include <iostream>
#include <fstream>
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

    // Vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    float quad[] = {
        -1.0, 1.0,  // top left
        -1.0, -1.0, // bottom left
        1.0, 1.0,   // top right
        1.0, -1.0,  // bottom right
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3};

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Vertex shader
    std::string vertexShaderSource;
    std::getline(std::ifstream("../vertex.glsl"), vertexShaderSource, '\0');
    unsigned int vertexShaderId;
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    const char *source = vertexShaderSource.c_str();
    glShaderSource(vertexShaderId, 1, &(source), nullptr);
    glCompileShader(vertexShaderId);

    int success;
    char infoLogi[512];
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, nullptr, infoLogi);
        std::cout << "ERROR:SADER::VERTEX::COMPILATION\n"
                  << infoLogi << std::endl;
    }

    // Fragment shader
    std::string fragmentShaderSource;
    std::getline(std::ifstream("../fragment.glsl"), fragmentShaderSource, '\0');
    unsigned int fragmentShaderId;
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsource = fragmentShaderSource.c_str();
    glShaderSource(fragmentShaderId, 1, &(fsource), nullptr);
    glCompileShader(fragmentShaderId);

    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, 512, nullptr, infoLogi);
        std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION\n"
                  << infoLogi << std::endl;
    }

    // Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShaderId);
    glAttachShader(shaderProgram, fragmentShaderId);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLogi);
        std::cout << "ERROR:PROGRAM::COMPILATION\n"
                  << infoLogi << std::endl;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);


    float resolution[] = {640.f, 480.f};
    auto location = glGetUniformLocation(shaderProgram, "res");
    glUniform2fv(location, 1, resolution);

    GLenum err;
    glClearColor(0.0, 0.0, 0.0, 1.0);
    while (true)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        time += .0166666;
        glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << err << std::endl;
        }
        SDL_PollEvent(&event);
        if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            break;
        if (event.type == SDL_QUIT)
            break;
        SDL_GL_SwapWindow(window);
    }
}
