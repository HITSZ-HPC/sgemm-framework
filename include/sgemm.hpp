#pragma once

namespace lib
{
    void sgemm(int m, int n, int k, float *a, int lda,
               float *b, int ldb,
               float *c, int ldc);

} // namespace lib
