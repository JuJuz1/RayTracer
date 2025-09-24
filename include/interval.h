#ifndef INCLUDE_INTERVAL_H_
#define INCLUDE_INTERVAL_H_

class Interval {
 public:
    double min, max;

    Interval() noexcept;
    Interval(double min, double max) noexcept;

    double size() const noexcept;

    // Checks whether x is in the range [min, max]
    // If inclusive is false -> range is ]min, max[
    bool contains(double x, bool inclusive = true) const noexcept;

    // Clamps x to the range [min, max]
    double clamp(double x) const noexcept;

    static const Interval empty, universe;
};

#endif // INCLUDE_INTERVAL_H_
