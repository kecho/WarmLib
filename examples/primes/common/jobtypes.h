#ifndef PRIME_JOB_TYPES

#define MAX_PRIME_NUMBERS 100

struct ComputePrimesJob
{
    int initialNumber;
    int endNumber;
};

struct ComputedPrimesResponse
{
    int primeList[MAX_PRIME_NUMBERS];
    int count;
};

#endif
