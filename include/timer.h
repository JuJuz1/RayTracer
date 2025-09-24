#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

#include <chrono>
#include <string>

class Timer {
 public:
    using clock = std::chrono::high_resolution_clock;

    Timer() noexcept;

    // ~Timer() noexcept;

    // TODO: Consider adding a start method

    // Pauses the timer and adds stores the elapsed time
    void pause() noexcept;

    // Resumes the timer to start accumulating time again
    // Calls reset()
    void resume() noexcept;

    // Sets the starting time to now
    void reset() noexcept;

    // Returns the elapsed time as seconds
    // If the timer was paused at any moment, returns the accumulated time instead
    double elapsed() const noexcept;

    void print_elapsed(const std::string& prefix = "", bool show_minutes = true) const noexcept;

 private:
    std::chrono::time_point<clock> start_time;
    double accumulated_time{ 0 };
    bool paused{ false };
};

// Prints progress for a single thread render
void print_progress_single_thread(const Timer& t, double elapsed, int j, int image_height) noexcept;

#endif // INCLUDE_TIMER_H_
