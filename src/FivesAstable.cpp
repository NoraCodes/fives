#include "plugin.hpp"


struct FivesAstable : Module {
	enum ParamIds {
		ON_PARAM,
		OFF_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
        RESET_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
    enum State {
        OFF,
        RISING,
        ON,
        FALLING
    };

    State state = OFF;
    float timeInState = 0.f; // Seconds
    float capacitorValue = 0.000100; // Farads
    float tRise = 0.0000001; // Typical - from TI datasheet for LM555
    float tFall = 0.0000001; // Typical - from TI datasheet for LM555

	FivesAstable() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(ON_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OFF_PARAM, 0.f, 1.f, 0.f, "");
	}

    void advanceState() {
        if (state == OFF) {
            state = RISING;
        } else if (state == RISING) {
            state = ON;
        } else if (state == ON) {
            state = FALLING;
        } else if (state == FALLING) {
            state = OFF;
        }
    }

    void process(const ProcessArgs &args) override {
        float onResistor = log(params[ON_PARAM].getValue() + 1.f) * 10000.f + 5.f; // Ohms; a potentiometer
        float offResistor = log(params[OFF_PARAM].getValue() + 1.f) * 10000.f + 15.f; // Ohms; a potentiometer
        float vReset = inputs[RESET_INPUT].getVoltage();
        bool resetting = vReset >= 4.85;

        // How long we should stay in the current state
        float tState = tRise;
        if (state == ON) {
            tState = log(2.f) * (onResistor + offResistor) * capacitorValue;
        } else if (state == OFF) {
            tState = log(2.f) * (offResistor) * capacitorValue;
        } else if (state == RISING || state == FALLING) {
            tState = tRise;
        }

        timeInState += args.sampleTime;
        if (resetting) {
            // Reset instantly changes the ON or RISING states to FALLING
            if (state == ON) {
                state = FALLING;
                timeInState = 0.f;
            } else if (state == RISING) {
                state = FALLING;
                timeInState = clamp(tState - timeInState, 0.f, tState);
            }
        }

        // Change state if we're out of time, but not if already off and held there.
        if (timeInState >= tState & !(resetting && state == OFF)) {
            advanceState();
            timeInState = 0.f;
        }

        float vOut = 0.f;
        if (state == ON) {
            vOut = 10.f;
        } else if (state == OFF) {
            vOut = 0.f;
        } else if (state == RISING) {
            float pDone = clamp(timeInState, 0.f, tState) / tState; 
            vOut = pDone * 10.f;
        } else if (state == FALLING) {
            float pDone = 1 - (clamp(timeInState, 0.f, tState) / tState); 
            vOut = pDone * 10.f;
        }

        outputs[OUT_OUTPUT].setVoltage(clamp(vOut, -1.f, 10.f));
    }
};

// Inkscape's coordinate system is backwards!
#define MODULE_HEIGHT 128.5

struct FivesAstableWidget : ModuleWidget {
	FivesAstableWidget(FivesAstable* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/FivesAstable.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.7, MODULE_HEIGHT - 113.5)), module,FivesAstable::ON_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.7, MODULE_HEIGHT - 96.5)), module, FivesAstable::OFF_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.7, MODULE_HEIGHT - 65.0)), module, FivesAstable::OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(12.7, MODULE_HEIGHT - 37.0)), module, FivesAstable::OUT_OUTPUT));
	}
};


Model* modelFivesAstable = createModel<FivesAstable, FivesAstableWidget>("NoraCodes_FivesAstable");
