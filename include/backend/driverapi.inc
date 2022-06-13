
#ifndef DECL_DRIVER_API
#error "The DECL_DRIVER_API() macro must be defined before including this file"
#define DECL_DRIVER_API(M, D, P)
#endif

#ifndef DECL_DRIVER_API_RETURN
#error "The DECL_DRIVER_API_RETURN() macro must be defined before including this file"
#define DECL_DRIVER_API_RETURN(R, M, D, P)
#endif

// ----------------------------------------------------------------------------
// https://gcc.gnu.org/onlinedocs/gcc/Variadic-Macros.html
// ##__VA_ARGS__, if no arguement provided, the ## would help to remove the comma automatically
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
#define ARG(T, P) T P
#define PARAM(T, P) P

#define PAIR_ARGS_0(M, ...)
#define PAIR_ARGS_1(M, X, Y, ...) M(X, Y)
#define PAIR_ARGS_2(M, X, Y, ...) M(X, Y), PAIR_ARGS_1(M, __VA_ARGS__)
#define PAIR_ARGS_3(M, X, Y, ...) M(X, Y), PAIR_ARGS_2(M, __VA_ARGS__)
#define PAIR_ARGS_4(M, X, Y, ...) M(X, Y), PAIR_ARGS_3(M, __VA_ARGS__)
#define PAIR_ARGS_5(M, X, Y, ...) M(X, Y), PAIR_ARGS_4(M, __VA_ARGS__)
#define PAIR_ARGS_6(M, X, Y, ...) M(X, Y), PAIR_ARGS_5(M, __VA_ARGS__)
#define PAIR_ARGS_7(M, X, Y, ...) M(X, Y), PAIR_ARGS_6(M, __VA_ARGS__)
#define PAIR_ARGS_8(M, X, Y, ...) M(X, Y), PAIR_ARGS_7(M, __VA_ARGS__)
#define PAIR_ARGS_9(M, X, Y, ...) M(X, Y), PAIR_ARGS_8(M, __VA_ARGS__)
#define PAIR_ARGS_10(M, X, Y, ...) M(X, Y), PAIR_ARGS_9(M, __VA_ARGS__)

#define PAIR_ARGS_N__(_0, E1, _1, E2, _2, E3, _3, E4, _4, E5, _5, E6, _6, E7, _7, E8, _8, E9, _9, E10, _10, X, ...) PAIR_ARGS_##X
#define PAIR_ARGS_N(M, ...) PAIR_ARGS_N__(0, ##__VA_ARGS__, 10, E, 9, E, 8, E, 7, E, 6, E, 5, E, 4, E, 3, E, 2, E, 1, E, 0)(M, __VA_ARGS__)

// ----------------------------------------------------------------------------
#define DECL_DRIVER_API_N(N, ...) \
    DECL_DRIVER_API(N, PAIR_ARGS_N(ARG, ##__VA_ARGS__), PAIR_ARGS_N(PARAM, ##__VA_ARGS__))

#define DECL_DRIVER_API_0(N)      DECL_DRIVER_API(N, int dummy=0, dummy)

DECL_DRIVER_API_N(makeCurrent, backend::HwSwapChain*, schDraw)

DECL_DRIVER_API_N(Commit, backend::HwSwapChain*, sch)

DECL_DRIVER_API_N(setVertexBufferObject,
        backend::HwVertexBuffer*, vbh,
        uint32_t, index,
        backend::HwBufferObject*, bufferObject)
DECL_DRIVER_API_N(updateBufferObject,
        backend::HwBufferObject*, boh,
        void*, data,
        size_t, dataSize,
        uint32_t, byteOffset)
DECL_DRIVER_API_N(updateIndexBuffer,
        backend::HwIndexBuffer*, ibh,
        void*, data,
        size_t, dataSize,
        uint32_t, byteOffset)
DECL_DRIVER_API_N(setRenderPrimitiveBuffer,
        backend::HwRenderPrimitive*, rph,
        backend::HwVertexBuffer*, vbh,
        backend::HwIndexBuffer*, ibh)
DECL_DRIVER_API_N(draw, backend::HwProgram*, program, backend::HwRenderPrimitive*, rph)
DECL_DRIVER_API_N(bindUniformBuffer,
        uint32_t, index,
        backend::HwBufferObject*, ubh)
DECL_DRIVER_API_N(beginRenderPass,
        backend::HwRenderTarget*, rth,
        const backend::RenderPassParams&, params)
DECL_DRIVER_API_N(bindSamplers, uint32_t, index,
        const backend::HwTexture*, th)
DECL_DRIVER_API_N(update2DImage,
        backend::HwTexture*, th,
        uint32_t, level,
        uint32_t, xoffset,
        uint32_t, yoffset,
        uint32_t, width,
        uint32_t, height,
        void*, data,
        size_t, dataSize,
        backend::PixelDataFormat, format,
        backend::PixelDataType, type)

DECL_DRIVER_API_0(endRenderPass)

// ----------------------------------------------------------------------------
#define DECL_DRIVER_API_R_N(R, N, ...) \
    DECL_DRIVER_API_RETURN(R, N, PAIR_ARGS_N(ARG, ##__VA_ARGS__), PAIR_ARGS_N(PARAM, ##__VA_ARGS__))

#define DECL_DRIVER_API_R_0(R,N)      DECL_DRIVER_API_RETURN(R, N, int dummy=0, dummy)

DECL_DRIVER_API_R_N(backend::HwSwapChain*, CreateSwapChain, void*, nativeWindow)
DECL_DRIVER_API_R_N(backend::HwRenderPrimitive*, createRenderPrimitive)
DECL_DRIVER_API_R_N(backend::HwVertexBuffer*, createVertexBuffer,
        uint8_t, bufferCount,
        uint8_t, attributeCount,
        uint32_t, vertexCount,
        const backend::AttributeArray&, attributes)
DECL_DRIVER_API_R_N(backend::HwBufferObject*, createBufferObject,
        uint32_t, byteCount,
        backend::BufferObjectBinding, bindingType,
        backend::BufferUsage, usage)
DECL_DRIVER_API_R_N(backend::HwIndexBuffer*, createIndexBuffer,
        backend::ElementType, elementType,
        uint32_t, indexCount,
        backend::BufferUsage, usage)
DECL_DRIVER_API_R_N(backend::HwProgram*, createProgram,
        const std::string&, vertexShader,
        const std::string&, fragShader,
        UniformBlockInfo, ubInfo)
DECL_DRIVER_API_R_N(backend::HwTexture*, createTexture,
        backend::SamplerType, target,
        uint8_t, levels,
        backend::TextureFormat, format,
        uint8_t, samples,
        uint32_t, width,
        uint32_t, height,
        uint32_t, depth,
        backend::TextureUsage, usage)
DECL_DRIVER_API_R_N(backend::HwRenderTarget*, createRenderTarget,
        backend::TargetBufferFlags, targetsFlags,
        uint32_t, width,
        uint32_t, height,
        uint8_t, samples,
        backend::MRT, color,
        backend::TargetBufferInfo, depth,
        backend::TargetBufferInfo, stencil)

DECL_DRIVER_API_R_N(backend::HwSamplerGroup*, createSamplerGroup)
DECL_DRIVER_API_R_0(backend::HwRenderTarget*, createDefaultRenderTarget)
// ----------------------------------------------------------------------------


#undef PAIR_ARGS_N
#undef PAIR_ARGS_N__
#undef PAIR_ARGS_4
#undef PAIR_ARGS_3
#undef PAIR_ARGS_2
#undef PAIR_ARGS_1
#undef PAIR_ARGS_0
#undef PARAM
#undef ARG

#undef DECL_DRIVER_API_0
#undef DECL_DRIVER_API_N
#undef DECL_DRIVER_API
#undef DECL_DRIVER_API_R_N
#undef DECL_DRIVER_API_RETURN
