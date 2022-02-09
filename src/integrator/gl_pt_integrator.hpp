/*****************************************************************************
 MIT License

 Copyright(c) 2022 Alexander Veselov

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this softwareand associated documentation files(the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions :

 The above copyright noticeand this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 *****************************************************************************/

#pragma once

#include "integrator.hpp"
#include "gpu_wrappers/gl_framebuffer.hpp"
#include "gpu_wrappers/gl_graphics_pipeline.hpp"
#include "gpu_wrappers/gl_compute_pipeline.hpp"
#include "glm/matrix.hpp"

class GLPathTraceIntegrator : public Integrator
{
public:
    GLPathTraceIntegrator(std::uint32_t width, std::uint32_t height,
        AccelerationStructure& acc_structure, std::uint32_t out_image);
    void UploadGPUData(Scene const& scene, AccelerationStructure const& acc_structure) override;
    void SetCameraData(Camera const& camera) override;
    void EnableWhiteFurnace(bool enable) override;
    void SetMaxBounces(std::uint32_t max_bounces) override;
    void SetSamplerType(SamplerType sampler_type) override;
    void SetAOV(AOV aov) override;
    void EnableDenoiser(bool enable) override;

protected:
    void Reset() override;
    void AdvanceSampleCount() override;
    void GenerateRays() override;
    void IntersectRays(std::uint32_t bounce) override;
    void ComputeAOVs() override;
    void ShadeMissedRays(std::uint32_t bounce) override;
    void ShadeSurfaceHits(std::uint32_t bounce) override;
    void IntersectShadowRays() override;
    void AccumulateDirectSamples() override;
    void ClearOutgoingRayCounter(std::uint32_t bounce) override;
    void ClearShadowRayCounter() override;
    void Denoise() override;
    void CopyHistoryBuffers() override;
    void ResolveRadiance() override;

private:
    void CreateKernels();

    GLFramebuffer framebuffer_;
    GraphicsPipeline graphics_pipeline_;
    ComputePipeline copy_pipeline_;

    // Pipelines
    std::unique_ptr<ComputePipeline> reset_pipeline_;
    std::unique_ptr<ComputePipeline> raygen_pipeline_;
    std::unique_ptr<ComputePipeline> intersect_pipeline_;
    std::unique_ptr<ComputePipeline> miss_pipeline_;
    std::unique_ptr<ComputePipeline> aov_pipeline_;
    std::unique_ptr<ComputePipeline> hit_surface_pipeline_;
    std::unique_ptr<ComputePipeline> accumulate_direct_samples_pipeline_;
    std::unique_ptr<ComputePipeline> clear_counter_pipeline_;
    std::unique_ptr<ComputePipeline> increment_counter_pipeline_;
    std::unique_ptr<ComputePipeline> temporal_accumulation_pipeline_;
    std::unique_ptr<ComputePipeline> resolve_pipeline_;

    GLuint radiance_image_;

    GLuint out_image_;

    // Scene buffers
    GLuint triangle_buffer_;

    // Acceleration structure
    GLuint rt_triangle_buffer_;
    GLuint nodes_buffer_;

    GLuint rays_buffer_[2]; // 2 buffers for incoming-outgoing rays
    GLuint shadow_rays_buffer_;
    GLuint pixel_indices_buffer_[2];
    GLuint shadow_pixel_indices_buffer_;
    GLuint ray_counter_buffer_[2];
    GLuint shadow_ray_counter_buffer_;
    GLuint hits_buffer_;
    GLuint shadow_hits_buffer_;
    GLuint throughputs_buffer_;
    GLuint sample_counter_buffer_;
    GLuint direct_light_samples_buffer_;

    std::uint32_t num_triangles_;
    Camera camera = {};
    Camera prev_camera = {};
    glm::mat4 view_proj_matrix_;
};
