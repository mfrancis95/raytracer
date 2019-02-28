#include <fstream>
#include "opengl_renderer.h"

const char *OpenGLRenderer::readFile(const char *file) const {
    std::ifstream stream{file, std::ios::ate};
    std::streamoff length = stream.tellg();
    stream.seekg(0, std::ios::beg);
    auto data = new char[length + 1];
    stream.read(data, length);
    data[length] = '\0';
    return data;
}

GLuint OpenGLRenderer::setupShader(const GLint type, const char *file) const {
    auto shader = glCreateShader(type);
    auto shaderSource = readFile(file);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    delete [] shaderSource;
    return shader;
}