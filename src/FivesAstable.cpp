#include "plugin.hpp"
#include "util.hpp"
#include "AstableChipModel.hpp"

#define DEFAULT_R1 5000.f
#define DEFAULT_R2 5000.f
#define DEFAULT_C1 100e-6

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

    AstableChipModel* chip;

	FivesAstable() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(ON_PARAM, 0.f, 1.f, 0.333, "On Time");
		configParam(OFF_PARAM, 0.f, 1.f, 0.333, "Off Time");
        chip = new AstableChipModel(DEFAULT_R1, DEFAULT_R2, DEFAULT_C1);
	}

    void process(const ProcessArgs &args) override {
        chip->resistorOne = exp_param_factor_inverted(params[ON_PARAM].getValue()) * DEFAULT_R1; // Ohms; a potentiometer
        chip->resistorTwo = exp_param_factor_inverted(params[OFF_PARAM].getValue()) * DEFAULT_R2; // Ohms; a potentiometer
        chip->vReset = inputs[RESET_INPUT].getVoltage(); 
        
        chip->update(args.sampleTime);

        outputs[OUT_OUTPUT].setVoltage(clamp(chip->vOut, -1.f, 10.f));
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

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.7, MODULE_HEIGHT - 65.0)), module, FivesAstable::RESET_INPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(12.7, MODULE_HEIGHT - 37.0)), module, FivesAstable::OUT_OUTPUT));
	}
};


Model* modelFivesAstable = createModel<FivesAstable, FivesAstableWidget>("NoraCodes_FivesAstable");
