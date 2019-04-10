#include <ctime>
#include "opengl_renderer.h"

struct ComputeRenderer : OpenGLRenderer {

    void render(SDL_Window *window, const Scene &scene) const {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
        SDL_GLContext context = SDL_GL_CreateContext(window);
        glFrontFace(GL_CW);
        GLuint buffers[5], computeShader, computeProgram, fragmentProgram,
            fragmentShader, vertexArray, vertexShader;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        glGenBuffers(5, buffers);
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
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[4]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, buffers[4]);
        glBufferStorage(
            GL_SHADER_STORAGE_BUFFER, 1280 * 960 * 4 * sizeof(float), nullptr, 0
        );
        glAttachShader(
            computeProgram = glCreateProgram(),
            computeShader = setupShader(GL_COMPUTE_SHADER, {
                "include.shader", "shader.comp"
            })
        );
        glLinkProgram(computeProgram);
        glUseProgram(computeProgram);
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
        glAttachShader(
            fragmentProgram = glCreateProgram(),
            vertexShader = setupShader(GL_VERTEX_SHADER, {"shader.vert"})
        );
        glAttachShader(
            fragmentProgram,
            fragmentShader = setupShader(GL_FRAGMENT_SHADER, {"shader2.frag"})
        );
        glLinkProgram(fragmentProgram);
        struct timespec end1, end2, end3, end4, end5, start;
        clock_gettime(CLOCK_MONOTONIC, &start);
        glDispatchCompute(1280, 960, 1);
        clock_gettime(CLOCK_MONOTONIC, &end1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        clock_gettime(CLOCK_MONOTONIC, &end2);
        glUseProgram(fragmentProgram);
        clock_gettime(CLOCK_MONOTONIC, &end3);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        clock_gettime(CLOCK_MONOTONIC, &end4);
        SDL_GL_SwapWindow(window);
        clock_gettime(CLOCK_MONOTONIC, &end5);
        std::cout << "glDispatchCompute\t" << (end1.tv_nsec - start.tv_nsec) / 1000000.0 << std::endl;
        std::cout << "glMemoryBarrier\t\t" << (end2.tv_nsec - start.tv_nsec) / 1000000.0 << std::endl;
        std::cout << "glUseProgram\t\t" << (end3.tv_nsec - start.tv_nsec) / 1000000.0 << std::endl;
        std::cout << "glDrawArrays\t\t" << (end4.tv_nsec - start.tv_nsec) / 1000000.0 << std::endl;
        std::cout << "SDL_GL_SwapWindow\t" << (end5.tv_nsec - start.tv_nsec) / 1000000.0 << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(fragmentProgram);
        glDeleteVertexArrays(1, &vertexArray);
        glDeleteShader(computeShader);
        glDeleteProgram(computeProgram);
        glDeleteBuffers(5, buffers);
        SDL_GL_DeleteContext(context);
    }

};

Renderer *createRenderer() {
    return new ComputeRenderer;
}