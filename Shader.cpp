#include <fstream>
#include "Shader.h"

unsigned int buildProgram(unsigned int fragmentShader, unsigned int vertexShader)
{
    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    GLint success;
    char infoLogi[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLogi);
        std::cout << "ERROR:PROGRAM::COMPILATION\n"
                  << infoLogi << std::endl;
    }
    return shaderProgram;
}

unsigned int buildShader(std::string &source, GLenum type)
{
    unsigned int shaderId;
    shaderId = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shaderId, 1, &(src), nullptr);
    glCompileShader(shaderId);

    int success;
    char infoLogi[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLogi);
        std::cout << "ERROR:SHADER::COMPILATION\n"
                  << infoLogi << std::endl;
    }

    return shaderId;
}


Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // Vertex shader
    std::string vertexShaderSource;
    std::getline(std::ifstream(vertexPath), vertexShaderSource, '\0');
    auto vertexShaderId = buildShader(vertexShaderSource, GL_VERTEX_SHADER);

    // Fragment shader
    std::string fragmentShaderSource;
    std::getline(std::ifstream(fragmentPath), fragmentShaderSource, '\0');
    auto fragmentShaderId = buildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    id = buildProgram(fragmentShaderId, vertexShaderId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

void Shader::use()
{
    if (id)
        glUseProgram(id);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setVec3(const std::string &name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {

    float values[3] = {x, y, z};
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, values);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}