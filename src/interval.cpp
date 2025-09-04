#include "interval.h"
#include "rtweekend.h"

Interval::Interval() noexcept : min(+rt::infinity), max(-rt::infinity) {};

Interval::Interval(double min, double max) noexcept : min(min), max(max) {};

double Interval::size() const {
    return max - min;
}

bool Interval::contains(double x, bool inclusive) const {
    if (inclusive) 
        return min <= x && x <= max;
    
    return min < x && x < max;
}

double Interval::clamp(double x) const {
    if (x < min) return min;
    if (max < x) return max;
    return x;
}

const Interval Interval::empty    = Interval(+rt::infinity, -rt::infinity);
const Interval Interval::universe = Interval(-rt::infinity, +rt::infinity);