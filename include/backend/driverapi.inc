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
// Note: MSVC musc eanble https://learn.microsoft.com/en-us/cpp/build/reference/zc-preprocessor?view=msvc-170
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly
#define EXPAND(x) x

#define ARG(T, P) T P
#define PARAM(T, P) P

#define PAIR_ARGS_0(M, ...)
#define PAIR_ARGS_1(M, X, Y, ...) M(X, Y)
#define PAIR_ARGS_2(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_1(M, __VA_ARGS__))
#define PAIR_ARGS_3(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_2(M, __VA_ARGS__))
#define PAIR_ARGS_4(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_3(M, __VA_ARGS__))
#define PAIR_ARGS_5(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_4(M, __VA_ARGS__))
#define PAIR_ARGS_6(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_5(M, __VA_ARGS__))
#define PAIR_ARGS_7(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_6(M, __VA_ARGS__))
#define PAIR_ARGS_8(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_7(M, __VA_ARGS__))
#define PAIR_ARGS_9(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_8(M, __VA_ARGS__))
#define PAIR_ARGS_10(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_9(M, __VA_ARGS__))

#define PAIR_ARGS_N__(_0, E1, _1, E2, _2, E3, _3, E4, _4, E5, _5, E6, _6, E7, _7, E8, _8, E9, _9, E10, _10, X, ...) PAIR_ARGS_##X
#define PAIR_ARGS_N(M, ...) EXPAND(EXPAND(PAIR_ARGS_N__(0, ##__VA_ARGS__, 10, E, 9, E, 8, E, 7, E, 6, E, 5, E, 4, E, 3, E, 2, E, 1, E, 0))(M, __VA_ARGS__))

// ----------------------------------------------------------------------------
#define DECL_DRIVER_API_N(N, ...) \
    DECL_DRIVER_API(N, PAIR_ARGS_N(ARG, ##__VA_ARGS__), PAIR_ARGS_N(PARAM, ##__VA_ARGS__))

#define DECL_DRIVER_API_0(N)      DECL_DRIVER_API(N, int dummy=0, dummy)

DECL_DRIVER_API_N(MakeCurrent, backend::HwSwapChain*, schDraw)

DECL_DRIVER_API_N(Commit, backend::HwSwapChain*, sch)

DECL_DRIVER_API_N(SetVertexBufferObject,
        backend::HwVertexBuffer*, vbh,
        uint32_t, index,
        backend::HwBufferObject*, bufferObject)
DECL_DRIVER_API_N(UpdateBufferObject,
        backend::HwBufferObject*, boh,
        void*, data,
        size_t, dataSize,
        uint32_t, byteOffset)
DECL_DRIVER_API_N(UpdateIndexBuffer,
        backend::HwIndexBuffer*, ibh,
        void*, data,
        size_t, dataSize,
        uint32_t, byteOffset)
DECL_DRIVER_API_N(SetRenderPrimitiveBuffer,
        backend::HwRenderPrimitive*, rph,
        backend::HwVertexBuffer*, vbh,
        backend::HwIndexBuffer*, ibh)
DECL_DRIVER_API_N(draw, backend::HwProgram*, program, backend::HwRenderPrimitive*, rph)
DECL_DRIVER_API_N(BindUniformBuffer,
        uint32_t, index,
        backend::HwBufferObject*, ubh)
DECL_DRIVER_API_N(BeginRenderPass,
        backend::HwRenderTarget*, rth,
        const backend::RenderPassParams&, params)
DECL_DRIVER_API_N(BindSamplers, uint32_t, index,
        const backend::HwTexture*, th)
DECL_DRIVER_API_N(Update2DImage,
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

// ----------------------------------------------------------------------------
// Currently No references between HwObject are holded.
// Please Destroy EveryThing that you created.
// ----------------------------------------------------------------------------
DECL_DRIVER_API_N(DestroyRenderPrimitive,
        backend::HwRenderPrimitive*, rp)
DECL_DRIVER_API_N(DestroyIndexBuffer,
        backend::HwIndexBuffer*, ib)
DECL_DRIVER_API_N(DestroyVertexBuffer,
        backend::HwVertexBuffer*, vb)
DECL_DRIVER_API_N(DestroyBufferObject,
        backend::HwBufferObject*, bo)
DECL_DRIVER_API_N(DestroySamplerGroup,
        backend::HwSamplerGroup*, sg)
DECL_DRIVER_API_N(DestroyProgram,
        backend::HwProgram*, pm)
DECL_DRIVER_API_N(DestroyTexture,
        backend::HwTexture*, tex)
DECL_DRIVER_API_N(DestroyRenderTarget,
        backend::HwRenderTarget*, mrt)
DECL_DRIVER_API_N(DestroySwapChain,
        backend::HwSwapChain*, sc)

DECL_DRIVER_API_0(endRenderPass)

// ----------------------------------------------------------------------------
#define DECL_DRIVER_API_R_N(R, N, ...) \
    DECL_DRIVER_API_RETURN(R, N, PAIR_ARGS_N(ARG, ##__VA_ARGS__), PAIR_ARGS_N(PARAM, ##__VA_ARGS__))

#define DECL_DRIVER_API_R_0(R,N)      DECL_DRIVER_API_RETURN(R, N, int dummy=0, dummy)

DECL_DRIVER_API_R_N(backend::HwSwapChain*, CreateSwapChain, void*, nativeWindow)
DECL_DRIVER_API_R_0(backend::HwRenderPrimitive*, CreateRenderPrimitive)
DECL_DRIVER_API_R_N(backend::HwVertexBuffer*, CreateVertexBuffer,
        uint8_t, bufferCount,
        uint8_t, attributeCount,
        uint32_t, vertexCount,
        const backend::AttributeArray&, attributes)
DECL_DRIVER_API_R_N(backend::HwBufferObject*, CreateBufferObject,
        uint32_t, byteCount,
        backend::BufferObjectBinding, bindingType,
        backend::BufferUsage, usage)
DECL_DRIVER_API_R_N(backend::HwIndexBuffer*, CreateIndexBuffer,
        backend::ElementType, elementType,
        uint32_t, indexCount,
        backend::BufferUsage, usage)
DECL_DRIVER_API_R_N(backend::HwProgram*, CreateProgram,
        const std::string&, vertexShader,
        const std::string&, fragShader,
        UniformBlockInfo, ubInfo)
DECL_DRIVER_API_R_N(backend::HwTexture*, CreateTexture,
        backend::SamplerType, target,
        uint8_t, levels,
        backend::TextureFormat, format,
        uint8_t, samples,
        uint32_t, width,
        uint32_t, height,
        uint32_t, depth,
        backend::TextureUsage, usage)
DECL_DRIVER_API_R_N(backend::HwRenderTarget*, CreateRenderTarget,
        backend::TargetBufferFlags, targetsFlags,
        uint32_t, width,
        uint32_t, height,
        uint8_t, samples,
        backend::MRT, color,
        backend::TargetBufferInfo, depth,
        backend::TargetBufferInfo, stencil)
// bind to location in the order of names
DECL_DRIVER_API_R_N(bool, BindSamplerLocations, 
        backend::HwProgram*, program,
        std::vector<std::string>, samplerNames)

DECL_DRIVER_API_R_0(backend::HwSamplerGroup*, CreateSamplerGroup)
DECL_DRIVER_API_R_0(backend::HwRenderTarget*, CreateDefaultRenderTarget)
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
