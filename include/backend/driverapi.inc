
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

#define PAIR_ARGS_N__(_0, E1, _1, E2, _2, E3, _3, E4, _4, X, ...) PAIR_ARGS_##X
#define PAIR_ARGS_N(M, ...) PAIR_ARGS_N__(0, ##__VA_ARGS__, 4, E, 3, E, 2, E, 1, E, 0)(M, __VA_ARGS__)

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
#define DECL_DRIVER_API_R_N(R, N, ...) \
    DECL_DRIVER_API_RETURN(R, N, PAIR_ARGS_N(ARG, ##__VA_ARGS__), PAIR_ARGS_N(PARAM, ##__VA_ARGS__))

DECL_DRIVER_API_R_N(backend::HwSwapChain, createSwapChain, void*, nativeWindow)
// ----------------------------------------------------------------------------

#undef DECL_DRIVER_API_R_N
#undef PAIR_ARGS_N
#undef PAIR_ARGS_N__
#undef PAIR_ARGS_4
#undef PAIR_ARGS_3
#undef PAIR_ARGS_2
#undef PAIR_ARGS_1
#undef PAIR_ARGS_0
#undef PARAM
#undef ARG

#undef DECL_DRIVER_API_RETURN