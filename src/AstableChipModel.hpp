#ifndef __ASTABLE_CHIP_MODEL_H_INCLUDED__
#define __ASTABLE_CHIP_MODEL_H_INCLUDED__
#include "plugin.hpp"
// The 555, in astable mode, wants to be either on or off,
// but can also be moving between these two states.
enum AstableState {
    OFF,
    RISING,
    ON,
    FALLING
};

class AstableChipModel {
    public:
    // The value of the capacitor used to tank charge for the oscillator.
    float capacitor;
    // The value of the time-on resistor
    float resistorOne;
    // The value of the time-off resistor
    float resistorTwo;
    // The incoming reset voltage.
    float vReset = 0.f;
    // The outgoing oscillator voltage.
    float vOut = 0.f;
    AstableChipModel(float r1, float r2, float c);
    void update(float dt);

    private:
    AstableState state = AstableState::OFF;
    // The time spent in the current state so far.
    float timeInState = 0.f;

    // How long the chip takes to actually commit a rising or falling signal.
    float tRise = 0.0000001; // Typical - from TI datasheet for LM555
    float tFall = 0.0000001; // Typical - from TI datasheet for LM555

    AstableState nextState();
    bool isResetting(); 
    float totalDesignTimeForCurrentState();
};

#endif //__ASTABLE_CHIP_MODEL_H_INCLUDED__
