#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define N_THREADS 8 // with main thread (1 main + 7 dother)
#define N 500'000'000 // size arr 

void calc_sum(int id, double *arr, double *sum)
{
    int count = N / N_THREADS;
    int left = id * count; // левая граница откуда считаем

    if (id == N_THREADS - 1) // скинем на последний, если массив не делится по потокам нацело
    {
        count += N % N_THREADS;
    }
    //printf("id = %d   count = %d   left = %d    right = %d\n", id, count, left, left + count);
    sum[id] = 0;
    double res = 0;
    for (int i = left; i < left + count; ++i)
    {
        //sum[id] += arr[i];
        res += arr[i];
    }
    sum[id] = res;
}

void generate_arr(double *arr)
{
    srand(time(0));
    for (int i = 0; i < N; ++i)
    {
        arr[i] = (double)(rand() % 10000) / 100;
    }
}

int main()
{
    double *arr = new double[N];
    double *sum = new double[N_THREADS];

    generate_arr(arr);

    auto t_start = system_clock::now();
    
    
    vector<thread> t;
    for (int i = 1; i < N_THREADS; ++i)
    {
        thread tmp(calc_sum, i, arr, sum);
        t.push_back(move(tmp));
    }
    
    // compute count
    int count = N / N_THREADS;
    // main thread compute 
    double res = 0;
    for (int i = 0; i < count; ++i)
    {
        res += arr[i];
    }
    sum[0] = res;


    for (int i = 0; i < N_THREADS - 1; ++i)
    {
        if (t[i].joinable())
            t[i].join();
    }

    res = 0;
    for (int i = 0; i < N_THREADS; ++i)
    {
        res += sum[i];
    }

    auto t_end = system_clock::now();

    long diff = duration_cast<milliseconds>(t_end - t_start).count();
    std::cout << "SUM (using threads: " << N_THREADS << ") is " << res << "  Time (ms): " << diff << std::endl;



    t_start = system_clock::now();
    // verify
    double a = 0;
    for (int i = 0; i < N; ++i)
    {
        a += arr[i];
    }
    t_end = system_clock::now();
    diff = duration_cast<milliseconds>(t_end - t_start).count();
    std::cout << "SUM (main thread) is " << sum[0] << "  Time (ms): " << diff << std::endl;

    //cout << a << "  " << sum[0] << endl;

    delete[] arr;
    delete[] sum;

    return 0;
}