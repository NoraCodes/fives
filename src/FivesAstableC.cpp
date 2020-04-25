#include "plugin.hpp"
#include "util.hpp"
#include "AstableChipModel.hpp"

#define DEFAULT_R1 178.f
#define DEFAULT_R2 600.f
#define DEFAULT_C1 4e-6

struct FivesAstableC : Module {
	enum ParamIds {
		ON_PARAM,
		OFF_PARAM,
        PERIOD_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
        RESET_INPUT,
        ON_INPUT,
        OFF_INPUT,
        PERIOD_INPUT,
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

    AstableChipModel* chip;

	FivesAstableC() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configParam(ON_PARAM, 0.f, 1.f, 0.333, "On Time");
		configParam(OFF_PARAM, 0.f, 1.f, 0.333, "Off Time");
		configParam(PERIOD_PARAM, 0.f, 1.f, 0.333, "Period");
        chip = new AstableChipModel(DEFAULT_R1, DEFAULT_R2, DEFAULT_C1);
	}

    void process(const ProcessArgs &args) override {
        if (inputs[ON_INPUT].isConnected()) {
            chip->resistorOne = log_scale_volts_param(inputs[ON_INPUT].getVoltage(), params[ON_PARAM].getValue()) * 10000.f + 5.f;
        } else {
            chip->resistorOne = exp_param_factor_inverted(params[ON_PARAM].getValue()) * DEFAULT_R1; // Ohms; a potentiometer
        }

        if (inputs[OFF_INPUT].isConnected()) {
            chip->resistorTwo = log_scale_volts_param(inputs[OFF_INPUT].getVoltage(), params[OFF_PARAM].getValue()) * 10000.f + 15.f;
        } else {
            chip->resistorTwo = exp_param_factor_inverted(params[OFF_PARAM].getValue()) * DEFAULT_R2; // Ohms; a potentiometer
        }

        if (inputs[PERIOD_INPUT].isConnected()) {
            chip->capacitor = log_scale_volts_param(inputs[PERIOD_INPUT].getVoltage(), params[PERIOD_PARAM].getValue()) * 100e-6 + 10e-6;
        } else {
            chip->capacitor = exp_param_factor_inverted(params[PERIOD_PARAM].getValue()) * DEFAULT_C1; // Farads; a varicap
        }

        if (inputs[RESET_INPUT].isConnected()) {
            chip->vReset = inputs[RESET_INPUT].getVoltage(); 
        }
        
        chip->update(args.sampleTime);

        outputs[OUT_OUTPUT].setVoltage(clamp(chip->vOut, -1.f, 10.f));
    }
};

// Inkscape's coordinate system is backwards!
#define MODULE_HEIGHT 128.5

struct FivesAstableCWidget : ModuleWidget {
	FivesAstableCWidget(FivesAstableC* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/FivesAstableC.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(11.0, MODULE_HEIGHT - 115.0)), module,FivesAstableC::ON_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.0, MODULE_HEIGHT - 115.0)), module, FivesAstableC::OFF_PARAM));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(11.0, MODULE_HEIGHT - 94.0)), module, FivesAstableC::ON_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(30.0, MODULE_HEIGHT - 94.0)), module, FivesAstableC::OFF_INPUT));

		addParam(createParamCentered<RoundLargeBlackKnob>(mm2px(Vec(11.0, MODULE_HEIGHT - 63.0)), module, FivesAstableC::PERIOD_PARAM));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(30.0, MODULE_HEIGHT - 63.0)), module, FivesAstableC::PERIOD_INPUT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(11.0, MODULE_HEIGHT - 34.0)), module, FivesAstableC::RESET_INPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.0, MODULE_HEIGHT - 34.0)), module, FivesAstableC::OUT_OUTPUT));
	}
};


Model* modelFivesAstableC = createModel<FivesAstableC, FivesAstableCWidget>("NoraCodes_FivesAstableC");
