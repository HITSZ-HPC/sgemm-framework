#pragma once

// Currently use Row Major storage

#define _A(i, j) a[(i)*lda + (j)]
#define _B(i, j) b[(i)*ldb + (j)]
#define _C(i, j) c[(i)*ldc + (j)]

/* Block sizes */
#define _A_BLOCK_SIZE 128
#define _B_BLOCK_SIZE 512

namespace lib
{
    void sgemm(int m, int n, int k, float *a, int lda,
               float *b, int ldb,
               float *c, int ldc);

} // namespace lib
