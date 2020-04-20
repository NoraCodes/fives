#include "plugin.hpp"


struct Fives : Module {
	enum ParamIds {
		ON_PARAM,
		OFF_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

    float phase = 0.f;

	Fives() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(ON_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OFF_PARAM, 0.f, 1.f, 0.f, "");
	}

    void process(const ProcessArgs &args) override {
        float onTime = params[ON_PARAM].getValue();
        clamp(onTime, -4.f, 4.f);
        phase += onTime * args.sampleTime;
        if (phase > 0.5f) phase -= 1.f;
        float sine = std::sin(2.f * M_PI * phase);
        outputs[OUT_OUTPUT].setVoltage(5.f * sine);
    }
};


struct FivesWidget : ModuleWidget {
	FivesWidget(Fives* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Fives.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(25.527, 28.753)), module, Fives::ON_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(26.539, 52.481)), module, Fives::OFF_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.885, 101.286)), module, Fives::OUT_OUTPUT));
	}
};


Model* modelFives = createModel<Fives, FivesWidget>("Fives");
