#include <include/sgemm.hpp>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <x86intrin.h> // _AVX512

using namespace std;

namespace lib
{

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