# Fives
VCVRack modules by NoraCodes

**Fives** is a collection of modules based on the specified physical operation of the
world's most popular integrated circuit: the LM555 multimode timer.

The 555 isn't generally used as an oscillator in modular instruments because, while it can
easily run in astable mode and generate a clean square wave with a variable duty cycle,
varying the frequency without changing the duty cycle requires a *varicap*, or variable
capacitor, which are expensive and hard to integrate.

There is, of course, no such limitation in the world of VCVRack; varicaps are free and
never break down, and voltage-controlled varicaps (which I've never seen in the real world,
though I'm told they do exist) are just as easy.

So, Fives provides modules that closely model what a real 555-based module would look like
as well as modules that apply the power of CV and impossibly accurate and fast varicaps
to the same mathematical model.

## Fives Astable

Fives Astable is a clock generator based on a physical model of the LM555 timer chip.
It has two knobs - ON and OFF - which control the high and low time of the oscillator,
and a RESET input which, when above 5 volts, kills the output.

The total period of oscillation is the on time plus the off time; there is no single
frequency control.

Fives Astable models the rise and fall times of a real LM555 as closely as possible.

The 555 timer in Fives Astable is in a typical 555 astable configuration,
using a 10uF capacitor.
Each knob is modelled as a single 10k potentiometer with a 10 Ohm potentiometer in series.
