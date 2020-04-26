#ifndef __UTIL_HPP_INCLUDED__
#define __UTIL_HPP_INCLUDED__

#define EULER 2.7182818

float log_scale_param(float x);
float log_scale_param_inverted(float x);
float log_scale_volts(float x);
float log_scale_volts_inverted(float x);
float log_scale_volts_param(float voltage, float param);
float exp_param_factor(float x);
float exp_param_factor_inverted(float x);
float lin_scale_cv(float x);
float exp_cv_tone(float x);

#endif //__UTIL_HPP_INCLUDED__
