#include "util.hpp"
#include "plugin.hpp"

float log_scale_param(float x) {
    return log((x * (EULER - 1.f)) + 1.f);
}

float log_scale_volts(float x) {
    return log_scale_param(clamp(x, 0.01f, 10.f) / 10.f);
}

