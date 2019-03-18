#include <fstream>
#include "opengl_renderer.h"

const char *OpenGLRenderer::readFile(const char *file) const {
    std::ifstream stream{file, std::ios::ate};
    std::streamoff length = stream.tellg();
    stream.seekg(0, std::ios::beg);
    auto data = new GLchar[length + 1];
    stream.read(data, length);
    data[length] = '\0';
    return data;
}

GLuint OpenGLRenderer::setupShader(const GLint type, std::vector<const char *> files) const {
    auto shader = glCreateShader(type);
    const GLchar *sources[files.size()];
    for (auto i = 0; i < files.size(); i++) {
        sources[i] = readFile(files[i]);
    }
    glShaderSource(shader, files.size(), sources, nullptr);
    glCompileShader(shader);
    char buffer[255];
    glGetShaderInfoLog(shader, 255, nullptr, buffer);
    printf("%s\n", buffer);
    for (auto i = 0; i < files.size(); i++) {
        delete [] sources[i];
    }
    return shader;
}