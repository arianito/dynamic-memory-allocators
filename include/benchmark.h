#pragma once

#include <chrono>
#include <functional>
#include <iostream>

void benchmark(std::string label, std::function<void()> func)
{
    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

    func();

    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << label << ": " << (microseconds / 1000.0f) << "ms" << std::endl;
}
