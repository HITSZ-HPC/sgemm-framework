#include <iostream>
#include <chrono>
#include <x86intrin.h>

#ifdef OPENBLAS
#include "openblas-serial/cblas.h"
#endif
#ifdef MKL
#include "cblas.h"
#endif

#include "include/sgemm.hpp"
#include "include/helper.hpp"

using Clock = std::chrono::system_clock;

#define M 1024
#define N 1024
#define K 1024

#define WARMUP_ROUND 2
#define HOT_ROUND 10

void gemm(int m, int n, int k, float *a, int lda, float *b, int ldb, float *c, int ldc)
{
#ifdef USER
    lib::sgemm(m, n, k, a, lda, b, ldb, c, ldc);
#endif
#ifdef OPENBLAS
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1, a, lda, b, ldb, 0, c, ldc);
#endif
#ifdef MKL
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1, a, lda, b, ldb, 0, c, ldc);
#endif
#ifdef BASELINE
    // (M, K) * (K, N) ==> (M, N)
    for (int ii = 0; ii < m; ++ii)
    {
        for (int jj = 0; jj < n; ++jj)
        {
            for (int kk = 0; kk < k; ++kk)
            {
                c[ii * ldc + jj] += a[ii * lda + kk] * b[kk * ldb + jj];
            }
        }
    }
#endif
}

int main(int argc, char const *argv[])
{
    // Init
    float *A = (float *)_mm_malloc(sizeof(float) * M * K, 64);
    float *B = (float *)_mm_malloc(sizeof(float) * K * N, 64);
    float *C = (float *)_mm_malloc(sizeof(float) * M * N, 64);
    init_matrix(A, M * K);
    init_matrix(B, K * N);

    // Warmup Cycle
    for (size_t i = 0; i < WARMUP_ROUND; i++)
    {
        gemm(M, N, K, A,
             K, B, N, C, N);
    }

    // Compute Cycle
    auto time_start = Clock::now();
    for (size_t i = 0; i < HOT_ROUND; i++)
    {
        gemm(M, N, K, A,
             K, B, N, C, N);
    }
    auto time_end = Clock::now();
    auto time_duration = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);
    double time_in_sec = (double)time_duration.count() * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

    // Statistics
    double FLOPS = ((double)2 * M * N * K / time_in_sec) * HOT_ROUND;
    double GFLOPS = FLOPS / 1024 / 1024 / 1024;

    // Output result
    std::cout << "Time consumed: " << time_in_sec << " s"
              << "\n";
    std::cout << "SGEMM performance: " << GFLOPS << " GFlops" << std::endl;

    return 0;
}
