#define GL_GLEXT_PROTOTYPES

#include <fstream>
#include <GL/gl.h>
#include "renderer.h"

static const char *readFile(const char *file) {
    std::ifstream stream{file, std::ios::ate};
    std::streamoff length = stream.tellg();
    stream.seekg(0, std::ios::beg);
    auto data = new char[length + 1];
    stream.read(data, length);
    data[length] = '\0';
    return data;
}

static GLuint setupShader(const GLint type, const char *file) {
    auto shader = glCreateShader(type);
    auto shaderSource = readFile(file);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    delete [] shaderSource;
    return shader;
}

struct OpenGLRenderer : Renderer {

    void render(SDL_Window *window, const Scene &scene) const {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
        SDL_GLContext context = SDL_GL_CreateContext(window);
        glFrontFace(GL_CW);
        GLuint buffer, fragmentShader, program, vertexArray, vertexShader;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        GLbyte positionData[8] = {
            -1, -1,
            -1, 1,
            1, -1,
            1, 1
        };
        glBufferStorage(GL_ARRAY_BUFFER, sizeof(positionData), positionData, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_BYTE, GL_FALSE, 0, nullptr);
        glAttachShader(
            program = glCreateProgram(),
            vertexShader = setupShader(GL_VERTEX_SHADER, "shader.vert")
        );
        glAttachShader(
            program,
            fragmentShader = setupShader(GL_FRAGMENT_SHADER, "shader.frag")
        );
        glLinkProgram(program);
        glUseProgram(program);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        SDL_GL_SwapWindow(window);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        glDeleteBuffers(1, &buffer);
        glDeleteVertexArrays(1, &vertexArray);
        SDL_GL_DeleteContext(context);
    }

};

Renderer *createRenderer() {
    return new OpenGLRenderer;
}