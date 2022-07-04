#include <iostream>
#include <chrono>
#include <x86intrin.h>

#include "include/sgemm.hpp"
#include "include/helper.hpp"

using Clock = std::chrono::system_clock;

#define M 1024
#define N 1024
#define K 1024

#define WARMUP_ROUND 2
#define HOT_ROUND 100

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
        lib::sgemm(M, N, K, A,
                   K, B, N, C, N);
    }

    // Compute Cycle
    auto time_start = Clock::now();
    for (size_t i = 0; i < HOT_ROUND; i++)
    {
        lib::sgemm(M, N, K, A,
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
