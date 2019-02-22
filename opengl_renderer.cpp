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
        GLuint buffers[4], fragmentShader, program, vertexArray, vertexShader;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        glGenBuffers(4, buffers);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        GLbyte positionData[8] = {
            -1, -1,
            -1, 1,
            1, -1,
            1, 1
        };
        glBufferStorage(GL_ARRAY_BUFFER, sizeof(positionData), positionData, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_BYTE, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[1]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers[1]);
        auto lightData = operator new(scene.lights.size() * 24);
        auto offset = 0;
        for (auto &light : scene.lights) {
            auto data = static_cast<float *>(lightData + offset);
            data[0] = light.colour.x;
            data[1] = light.colour.y;
            data[2] = light.colour.z;
            data[3] = light.position.x;
            data[4] = light.position.y;
            data[5] = light.position.z;
            offset += 24;
        }
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, scene.lights.size() * 24, lightData, 0);
        operator delete(lightData);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[2]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffers[2]);
        auto materialData = operator new(scene.materials.size() * 16);
        offset = 0;
        for (auto &material : scene.materials) {
            auto data = static_cast<float *>(materialData + offset);
            data[0] = material.colour.x;
            data[1] = material.colour.y;
            data[2] = material.colour.z;
            data[3] = material.shininess;
            offset += 16;
        }
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, scene.materials.size() * 16, materialData, 0);
        operator delete(materialData);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[3]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffers[3]);
        auto primitiveData = operator new(scene.primitives.size() * 16);
        offset = 0;
        for (auto &primitive: scene.primitives) {
            primitive->serialise(primitiveData + offset);
            offset += 16;
        }
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, scene.primitives.size() * 16, primitiveData, 0);
        operator delete(primitiveData);
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
        glUniform3f(glGetUniformLocation(program, "camera.direction"), scene.camera.direction.x, scene.camera.direction.y, scene.camera.direction.z);
        glUniform3f(glGetUniformLocation(program, "camera.position"), scene.camera.position.x, scene.camera.position.y, scene.camera.position.z);
        glUniform3f(glGetUniformLocation(program, "camera.right"), scene.camera.right.x, scene.camera.right.y, scene.camera.right.z);
        glUniform3f(glGetUniformLocation(program, "camera.up"), scene.camera.up.x, scene.camera.up.y, scene.camera.up.z);
        glUniform1i(glGetUniformLocation(program, "numLights"), scene.lights.size());
        glUniform1i(glGetUniformLocation(program, "numPrimitives"), scene.primitives.size());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        SDL_GL_SwapWindow(window);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        glDeleteBuffers(4, buffers);
        glDeleteVertexArrays(1, &vertexArray);
        SDL_GL_DeleteContext(context);
    }

};

Renderer *createRenderer() {
    return new OpenGLRenderer;
}