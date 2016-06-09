#include "Clock.hpp"

#include <iostream>


void Clock::begin(void) {
    begin_ = std::chrono::steady_clock::now();
}

void Clock::end(const std::string& caption, Clock::CastType cast) {
    end_ = std::chrono::steady_clock::now();

    switch (cast) {
        case SECONDS:
        {
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(end_-begin_);
            std::cout << caption << diff.count() << " s" << std::endl;
        }
        break;

        case MILLISECONDS:
        {
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end_-begin_);
            std::cout << caption << diff.count() << " ms" << std::endl;
        }
        break;

        case MICROSECONDS:
        {
            auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end_-begin_);
            std::cout << caption << diff.count() << " us" << std::endl;
        }
        break;

        case NANOSECONDS:
        {
            auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end_-begin_);
            std::cout << caption << diff.count() << " ns" << std::endl;
        }
        break;
    }
}
