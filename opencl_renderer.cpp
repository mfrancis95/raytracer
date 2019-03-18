#include <CL/opencl.h>
#include "opengl_renderer.h"

struct OpenCLRenderer : OpenGLRenderer {

    void render(SDL_Window *window, const Scene &scene) const {
        cl_platform_id platform;
        clGetPlatformIDs(1, &platform, nullptr);
        cl_device_id device;
        clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
        cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, nullptr);
        cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, nullptr, nullptr);
        const char *source = readFile("kernel.cl");
        cl_program program = clCreateProgramWithSource(context, 1, &source, nullptr, nullptr);
        clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
        cl_kernel kernel = clCreateKernel(program, "main", nullptr);
        cl_mem buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int), nullptr, nullptr);
        const size_t local_work_size = 1, global_work_size = 1;
        clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &global_work_size, &local_work_size, 0, nullptr, nullptr);
        clFinish(queue);
        int value;
        clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, sizeof(int), &value, 0, nullptr, nullptr);
        std::cout << value << std::endl;
    }

};

Renderer *createRenderer() {
    return new OpenCLRenderer;
}