#include "util.hpp"
#include "plugin.hpp"

// Scale from 0 to 1 lin to 0 to 1 with an aggressive log
float log_scale_param(float x) {
    return log((10.f * x * (EULER - 1.f)) + 1.f) / 1.26;
}

// Scale from 0 to 1 lin to 1 to 0 log
float log_scale_param_inverted(float x) {
    return 1.f - log_scale_param(x);
}

// Scale from 0 to 10 lin to 0 to 1 log
float log_scale_volts(float x) {
    return log_scale_param(clamp(x, 0.01f, 10.f) / 10.f);
}

float log_scale_volts_inverted(float x) {
    return 1.f - log_scale_volts(x);
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

// Map a value between 0 and 1 to between 0.1 and 10, exponential
float exp_param_factor(float x) {
    return pow(10.f, 3.f * x - 2.f);
}

// Map a value between 0 and 1 to between 10 and 0.1, exponential
float exp_param_factor_inverted(float x) {
    return exp_param_factor(1.f - x);
}

// Map a value from -10 to 10 to between 0 and 1, linear
float lin_scale_cv(float x) {
    return (x + 10.f) / 20.f;
}

// 1V/Oct from voltage
float exp_cv_tone(float x) {
    return pow(2, x);
}
