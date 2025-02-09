#pragma once
#include<iostream>
#include<chrono>

using namespace std;
using namespace chrono;

class Timer
{
    chrono::steady_clock::time_point start_time;// = chrono::steady_clock::now();
    chrono::steady_clock::time_point end_time;// = chrono::steady_clock::now();
    int time{};



public:
    void start() { start_time = steady_clock::now(); }
    void stop(char c)
    {
        end_time = steady_clock::now();

        switch (c)
        {
        case 'u':
            time = duration_cast<microseconds>(end_time - start_time).count();
            cout << time << " uSeconds Elapsed\n";
            break;
        case 'm':
            time = duration_cast<milliseconds>(end_time - start_time).count();
            cout << time << " Milliseconds Elapsed\n";
            break;
        case 's':
        case 'S':
            time = duration_cast<seconds>(end_time - start_time).count();
            cout << time << " Seconds Elapsed\n";
            break;
        case 'M':
            time = duration_cast<minutes>(end_time - start_time).count();
            cout << time << " Minutes\n";
            break;
        }
    }
};