#include "timer.h"

#include <iostream>
#include <iomanip>

Timer::Timer() noexcept : start_time{ clock::now() } {}

// Timer::~Timer() noexcept {
//     print_elapsed();
// }

void Timer::pause() noexcept {
    if (!paused)
        accumulated_time += elapsed();
    
    paused = true;
}

void Timer::resume() noexcept {
    paused = false;
    reset();
}

void Timer::reset() noexcept {
    start_time = clock::now();
}

double Timer::elapsed() const noexcept {
    if (0 < accumulated_time)
        return accumulated_time;
    
    const auto now{ clock::now() };
    return std::chrono::duration<double>(now - start_time).count();
}

void Timer::print_elapsed(const std::string& prefix, bool show_minutes) const noexcept {
        const double seconds{ elapsed() };
        std::cout << prefix 
                  << std::fixed << std::setprecision(3) << seconds << "s";
        
        if (show_minutes)
            std::cout << " (" << std::setprecision(1) << seconds / 60.0 << "m)";

        std::cout << "\n" << std::flush;
}

// Utility functions

void print_progress_single_thread(const Timer& t, double elapsed, int j, int image_height) noexcept {
    const int scanlines_done{ j + 1 };
    const int scanlines_remaining{ image_height - scanlines_done };

    const double elapsed_min{ elapsed / 60.0 };
    const double eta{ elapsed / scanlines_done * scanlines_remaining };
    const double eta_min{ eta / 60.0 };

    std::cout << "\rScanlines remaining: " << (image_height - 1 - j)
                << " | Elapsed time: " << std::fixed << std::setprecision(3) << elapsed << "s" 
                << " (" << std::setprecision(1) << elapsed_min << "m)"
                << std::setprecision(3)
                << " | ETA: " << eta << "s" 
                << " (" << std::setprecision(1) << eta_min << "m)"
                << ' ' // Padding
                << std::flush;
}