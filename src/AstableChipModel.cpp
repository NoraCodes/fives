#include "AstableChipModel.hpp"

AstableChipModel::AstableChipModel(float r1, float r2, float c) {
    this->resistorOne = r1;
    this->resistorTwo = r2;
    this->capacitor = c;
}

AstableState AstableChipModel::nextState() {
    bool resetting = this->isResetting();
    if (this->state == AstableState::OFF) {
        if (resetting) { return OFF; }
        return RISING;
    } else if (this->state == RISING) {
        if (resetting) { return FALLING; }
        return ON;
    } else if (this->state == ON) {
        return FALLING;
    } else if (this->state == FALLING) {
        return OFF;
    } else {
        return OFF;
    }
}

bool AstableChipModel::isResetting() {
    return this->vReset >= 4.85;
}

float AstableChipModel::totalDesignTimeForCurrentState() {
    // How long we should stay in the current state
    if (this->state == ON) {
        return log(2.f) * (this->resistorOne + this->resistorTwo) * this->capacitor;
    } else if (this->state == OFF) {
        return log(2.f) * (this->resistorTwo) * this->capacitor;
    } else if (this->state == RISING || state == FALLING) {
        return this->tRise;
    } else {
        return this->tRise;
    }
}

void AstableChipModel::update(float dt) {
    this->timeInState += dt;
    float tState = this->totalDesignTimeForCurrentState();

    // State change by time, or by resetting.
    if (this->isResetting() || this->timeInState >= tState) {
        // Move to the next state.
        this->state = this->nextState();
        if (this->isResetting() && this->state == FALLING) {
            // Special case: reversing direction on a rising edge
            // means discharging that gate.
            this->timeInState = clamp(tState - timeInState, 0.f, tState);
        } else {
            // Otherwise just start the cycle over.
            this->timeInState = 0.f;
        }
    }

    if (this->state == ON) {
        this->vOut = 10.f;
    } else if (this->state == OFF) {
        this->vOut = 0.f;
    } else if (this->state == RISING) {
        float pDone = clamp(timeInState, 0.f, tState) / tState; 
        this->vOut = pDone * 10.f;
    } else if (this->state == FALLING) {
        float pDone = 1 - (clamp(timeInState, 0.f, tState) / tState); 
        this->vOut = pDone * 10.f;
    }
};


