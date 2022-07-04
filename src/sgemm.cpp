#include <include/sgemm.hpp>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <x86intrin.h> // _AVX512

using namespace std;

namespace lib
{

    static void _AddDot4x64(int, const float *, int, const float *, int, float *, int);
    static void InnerKernel(int m, int n, int k, float *a, int lda,
                            float *b, int ldb,
                            float *c, int ldc);
    static void PackMatrixB(int, float *, int, float *);
    static float *packedB = nullptr;

    void sgemm(int m, int n, int k, float *a, int lda,
               float *b, int ldb,
               float *c, int ldc)
    {
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
    }

}