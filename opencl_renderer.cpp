#include <CL/opencl.h>
#include "opengl_renderer.h"

struct OpenCLRenderer : OpenGLRenderer {

    void render(SDL_Window *window, const Scene &scene) const {
        cl_platform_id platform;
        clGetPlatformIDs(1, &platform, nullptr);
        cl_device_id device;
        clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
        cl_context clContext = clCreateContext(
            nullptr, 1, &device, nullptr, nullptr, nullptr
        );
        cl_command_queue queue = clCreateCommandQueueWithProperties(
            clContext, device, nullptr, nullptr
        );
        const char *source = readFile("kernel.cl");
        cl_program clProgram = clCreateProgramWithSource(
            clContext, 1, &source, nullptr, nullptr
        );
        clBuildProgram(clProgram, 0, nullptr, nullptr, nullptr, nullptr);
        cl_kernel kernel = clCreateKernel(clProgram, "main", nullptr);
        cl_mem buffer = clCreateBuffer(
            clContext, CL_MEM_WRITE_ONLY, 1280 * 960 * 4 * sizeof(float),
            nullptr, nullptr
        );
        clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
        const size_t local_work_size = 1, global_work_size = 1280 * 960;
        clEnqueueNDRangeKernel(
            queue, kernel, 1, nullptr, &global_work_size, &local_work_size, 0,
            nullptr, nullptr
        );
        clFinish(queue);
        float *colours = new float[1280 * 960 * 4];
        clEnqueueReadBuffer(
            queue, buffer, CL_TRUE, 0, 1280 * 960 * 4 * sizeof(float), colours,
            0, nullptr, nullptr
        );
        clReleaseMemObject(buffer);
        clReleaseKernel(kernel);
        clReleaseProgram(clProgram);
        clReleaseCommandQueue(queue);
        clReleaseContext(clContext);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
        SDL_GLContext glContext = SDL_GL_CreateContext(window);
        glFrontFace(GL_CW);
        GLuint buffers[2], glProgram, fragmentShader, vertexArray, vertexShader;
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);
        glGenBuffers(2, buffers);
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
        glAttachShader(
            glProgram = glCreateProgram(),
            vertexShader = setupShader(GL_VERTEX_SHADER, {"shader.vert"})
        );
        glAttachShader(
            glProgram,
            fragmentShader = setupShader(GL_FRAGMENT_SHADER, {"shader2.frag"})
        );
        glLinkProgram(glProgram);
        glUseProgram(glProgram);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[1]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, buffers[1]);
        glBufferStorage(
            GL_SHADER_STORAGE_BUFFER, 1280 * 960 * 4 * sizeof(float), colours,
            0
        );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        SDL_GL_SwapWindow(window);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(glProgram);
        glDeleteBuffers(2, buffers);
        glDeleteVertexArrays(1, &vertexArray);
        SDL_GL_DeleteContext(glContext);
        delete [] colours;
    }

};

Renderer *createRenderer() {
    return new OpenCLRenderer;
}