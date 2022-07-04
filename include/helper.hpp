#pragma once
#include <random>

void init_matrix(float *m, size_t s)
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> gen(0.1f, 0.5f);
    for (size_t i = 0; i < s; i++)
    {
        m[i] = gen(e2);
    }
}