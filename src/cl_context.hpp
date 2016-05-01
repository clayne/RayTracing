#ifndef CL_CONTEXT_HPP
#define CL_CONTEXT_HPP

#include "scene.hpp"
#include <CL/cl.hpp>
#include <fstream>

class ClContext
{
public:
    ClContext(const cl::Platform& platform, const std::string& source, size_t width, size_t height, size_t cell_resolution);
    void setupBuffers(const Scene& scene);

    void setArgument(size_t index, size_t size, const void* argPtr);
    void writeRandomBuffer(size_t size, const void* ptr);
    void executeKernel(cl_float4* ptr, size_t size, size_t offset);

    bool isValid() const { return valid_; }

private:
    size_t width_;
    size_t height_;
    bool valid_;

    cl::Context context_;
    cl::CommandQueue queue_;
    cl::Kernel kernel_;
    // Memory Buffers
    cl::Buffer pixel_buffer_;
    cl::Buffer random_buffer_;
    cl::Buffer scene_buffer_;
    cl::Buffer index_buffer_;
    cl::Buffer cell_buffer_;

};

#endif // CL_CONTEXT_HPP