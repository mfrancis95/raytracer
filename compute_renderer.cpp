#include "opengl_renderer.h"

struct ComputeRenderer : OpenGLRenderer {

    void render(SDL_Window *window, const Scene &scene) const {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
        SDL_GLContext context = SDL_GL_CreateContext(window);
        glFrontFace(GL_CW);
        GLuint buffers[2], computeShader, computeProgram, fragmentProgram,
            fragmentShader, vertexArray, vertexShader;
        glGenBuffers(2, buffers);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[0]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers[0]);
        glBufferStorage(
            GL_SHADER_STORAGE_BUFFER, 1280 * 960 * 4 * sizeof(float), nullptr, 0
        );
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
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
            computeProgram = glCreateProgram(),
            computeShader = setupShader(GL_COMPUTE_SHADER, "shader.comp")
        );
        glLinkProgram(computeProgram);
        glUseProgram(computeProgram);
        glAttachShader(
            fragmentProgram = glCreateProgram(),
            vertexShader = setupShader(GL_VERTEX_SHADER, "shader.vert")
        );
        glAttachShader(
            fragmentProgram,
            fragmentShader = setupShader(GL_FRAGMENT_SHADER, "shader2.frag")
        );
        glLinkProgram(fragmentProgram);
        glDispatchCompute(1280, 960, 1);
        glUseProgram(fragmentProgram);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        SDL_GL_SwapWindow(window);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(fragmentProgram);
        glDeleteVertexArrays(1, &vertexArray);
        glDeleteShader(computeShader);
        glDeleteProgram(computeProgram);
        glDeleteBuffers(2, buffers);
        SDL_GL_DeleteContext(context);
    }

};

Renderer *createRenderer() {
    return new ComputeRenderer;
}