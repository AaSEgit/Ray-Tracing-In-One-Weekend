#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

// manages real-valued intervals with a minimum and a maximum
class interval {
    public:
        double min, max;

        interval() : min(+infinity), max(-infinity) {}  // Default interval is empty

        interval(double min, double max) : min(min), max(max) {}

        double size() const {
            return max - min;
        }

        // returns true if x is within the bounds of the interval
        // NOTE: includes min and max interval values
        bool contains(double x) const {
            return min <= x && x <= max;
        }

        // returns true if x is within the bounds of the interval
        // NOTE: does NOT include min and max interval values
        bool surrounds(double x) const {
            return min < x && x < max;
        }

        // clamps/bounds sample area color components to [min, max]
        double clamp(double x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        static const interval empty, universe;
};

const interval interval::empty      = interval(+infinity, -infinity);
const interval interval::universe   = interval(-infinity, +infinity);

#endif