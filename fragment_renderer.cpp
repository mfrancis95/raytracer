#include <iostream>
#include "opengl_renderer.h"
#include "timer.h"

struct FragmentRenderer : OpenGLRenderer {

    void render(SDL_Window *window, const Scene &scene) const {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
        SDL_GL_SetSwapInterval(0);
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
        char lightData[scene.lights.size() * LIGHT_SIZE];
        auto offset = 0;
        for (auto &light : scene.lights) {
            light.colour.serialise(lightData + offset);
            light.position.serialise(lightData + offset + 16);
            offset += LIGHT_SIZE;
        }
        glBufferStorage(
            GL_SHADER_STORAGE_BUFFER, scene.lights.size() * LIGHT_SIZE,
            lightData, 0
        );
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[2]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffers[2]);
        char materialData[scene.materials.size() * MATERIAL_SIZE];
        offset = 0;
        for (auto &material : scene.materials) {
            material.colour.serialise(materialData + offset);
            *reinterpret_cast<float *>(materialData + offset + 12) = material.diffuse;
            *reinterpret_cast<float *>(materialData + offset + 16) = material.shininess;
            *reinterpret_cast<float *>(materialData + offset + 20) = material.specular;
            offset += MATERIAL_SIZE;
        }
        glBufferStorage(
            GL_SHADER_STORAGE_BUFFER, scene.materials.size() * MATERIAL_SIZE,
            materialData, 0
        );
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[3]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffers[3]);
        char primitiveData[scene.primitives.size() * PRIMITIVE_SIZE];
        offset = 0;
        for (auto &primitive: scene.primitives) {
            primitive->serialise(primitiveData + offset);
            offset += PRIMITIVE_SIZE;
        }
        glBufferStorage(
            GL_SHADER_STORAGE_BUFFER, scene.primitives.size() * PRIMITIVE_SIZE,
            primitiveData, 0
        );
        glAttachShader(
            program = glCreateProgram(),
            vertexShader = setupShader(GL_VERTEX_SHADER, {"shader.vert"})
        );
        glAttachShader(
            program,
            fragmentShader = setupShader(GL_FRAGMENT_SHADER, {
                "include.shader", "shader.frag"
            })
        );
        glLinkProgram(program);
        glUseProgram(program);
        glUniform3f(0, scene.ambient.x, scene.ambient.y, scene.ambient.z);
        glUniform3f(
            1, scene.camera.direction.x, scene.camera.direction.y,
            scene.camera.direction.z
        );
        glUniform3f(
            2, scene.camera.position.x, scene.camera.position.y,
            scene.camera.position.z
        );
        glUniform3f(
            3, scene.camera.right.x, scene.camera.right.y, scene.camera.right.z
        );
        glUniform3f(4, scene.camera.up.x, scene.camera.up.y, scene.camera.up.z);
        glUniform1i(5, scene.lights.size());
        glUniform1i(6, scene.primitives.size());
        Timer::start();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        auto end1 = Timer::takeTime();
        SDL_GL_SwapWindow(window);
        auto end2 = Timer::takeTime();
        std::cout << "Render timings:" << std::endl;
        std::cout << "glDrawArrays\t\t" << end1 << std::endl;
        std::cout << "SDL_GL_SwapWindow\t" << (end2 - end1) << std::endl;
        std::cout << "Total rendering\t\t" << end2 << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        glDeleteBuffers(4, buffers);
        glDeleteVertexArrays(1, &vertexArray);
        SDL_GL_DeleteContext(context);
    }

};

Renderer *createRenderer() {
    return new FragmentRenderer;
}