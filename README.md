# Fives
## VCVRack modules by NoraCodes

**Fives** is a collection of modules based on the specified physical operation of the
world's most popular integrated circuit: the **LM555 multimode timer**.

![The Fives Astable and Fives Astable C next to each other.](res/lineup.png)

### Why 555?

The 555 isn't generally used as an oscillator in modular instruments because, while it can
easily run in astable mode and generate a clean square wave with a variable duty cycle,
varying the frequency without changing the duty cycle requires a variable
capacitor, which are expensive and hard to integrate.

There is, of course, no such limitation in the world of VCVRack; variable caps are free and
never break down, and voltage-controlled variable capacitors are just as easy.

So, Fives provides modules that closely model what a real 555-based module would look like
as well as modules that apply the power of CV and impossibly accurate and fast variable
capacitors to the same mathematical model.

### Fives Astable

Fives Astable is a low-frequency oscillator and clock generator based on a
physical model of the LM555 timer chip in an astable configuration.
It has two knobs - ON and OFF - which control the high and low time of the oscillator,
and a RESET input which, when above 5 volts, kills the output.

![Two Fives Astable modules, one resetting the other.](res/astable-sample.png)

The total period of oscillation is the on time plus the off time; there is no single
frequency control. It can oscillate as quickly as 9 Hz and as slowly as 0.1Hz
(once every 10 seconds) with a duty cycle around 66%.

### Fives Astable C

The Fives Astable C is a voltage-controlled audio-rate oscillator which extends the slimmer,
simpler Fives Astable with a variable capacitor and CV inputs for the ON, OFF, and PERIOD inputs.
The added PERIOD variable capacitor allows the Fives Astable C to oscillate as quickly as 2.6 kHz
or as slowly as 26 Hz, with a duty cycle as low as 56.5%.

It is tuned to take 1V/Oct input on the PERIOD input by default, with all parameter knobs at 33.3%.
Modifying any of the parameters will detune the module, but that can be fun too.

![A Fives Astable C and a Fives Astable, showing the use of the PERIOD parameter.](res/astable-c-sample.png)

CV applied to the inputs are scaled by the knob parameters.

