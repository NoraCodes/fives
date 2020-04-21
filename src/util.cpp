#include "util.hpp"
#include "plugin.hpp"

float log_scale_param(float x) {
    return log((x * (EULER - 1.f)) + 1.f);
}

float log_scale_volts(float x) {
    return log_scale_param(clamp(x, 0.01f, 10.f) / 10.f);
}

// Amplify or attenuate the given voltage based on the param value,
// and then log scale.
float log_scale_volts_param(float voltage, float param) {
    float shifted = param - 0.5; // The full range is 1.
    float p = 0.f;
    if (shifted >= 0) {
        p = log_scale_param(shifted) + 1;
    } else {
        p = -log_scale_param(-shifted) + 1;
    }
    return log_scale_volts(voltage) * p; 
}

