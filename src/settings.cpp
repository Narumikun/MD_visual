
#include "ofxDatGui.h"
#include "settings.h"
#include "atom_model.h"



Settings::Settings() {
	gui = new ofxDatGui();
}
void Settings::setup() {
	theme = new PanelTheme();
	theme->setScale(1.3);
	gui->setAutoDraw(false);
	
	//header and footer
	gui->addHeader("Setting Panel");
	gui->addFooter();
	gui->getFooter()->setLabelWhenExpanded("Setting Panel - COLLAPSE");
	gui->getFooter()->setLabelWhenCollapsed("Setting Panel - EXPAND");

	// play controls
	actions = gui->addFolder("Actions", ofColor::blue);
	playButton = actions->addButton("   ** Play");
	pauseButton = actions->addButton("   ** Pause");
	stopButton = actions->addButton("   ** Stop");
	actions->expand();
	gui->addBreak();

	//model settings:: frame num/current/rate/, neighbor_color, neighbor_num, cent_id
	modelFolder = gui->addFolder("Model Settings", ofColor::green);
	//frame related
	//modelFrameNumSlider = modelFolder->addSlider("  Frames", 1, 20, 5);
	//modelFrameNumSlider->setPrecision(0);
	modelCurFrameSlider = modelFolder->addSlider("  Current frame", 0, 0, 0);
	modelCurFrameSlider->setPrecision(0);
	modelFrameRateSlider = modelFolder->addSlider("  Model FR", 1, 25, 1);
	modelFrameRateSlider->setPrecision(0);
	//center/neighbor group related
	modelCenterIdSlider = modelFolder->addSlider("  Center id", 0, 250, 137);
	modelCenterIdSlider->setPrecision(0);
	modelNeighborNumSlider = modelFolder->addSlider("  Neighbor num", 0, 100, 8);
	modelNeighborNumSlider->setPrecision(0);
	modelNeighborRadiusSlider = modelFolder->addSlider("  Radius", 0, 100, 50);
	modelNeighborRadiusSlider->setPrecision(1);
	modelForceFieldToggle = modelFolder->addToggle("  Show force field");
	// change for Opacity to Color mixing
	//modelColorMixingSlider = modelFolder->addSlider("  Opacity", 0, 255, 255);
	modelColorMixingSlider = modelFolder->addSlider("  Color mixing", 0, 255, 20);
	modelColorMixingSlider->setPrecision(0);
	modelCenterColorPicker = modelFolder->addColorPicker("  Center color", ofColor(148, 0, 211));
	modelNeighborColorPicker = modelFolder->addColorPicker("  Neighbor color", ofColor(3, 168, 158));

	modelFolder->expand();
	gui->addBreak();
	
	//light controls
	lightingFolder = gui->addFolder("Lighting", ofColor::yellow);
	lightRotateX = lightingFolder->addSlider("  Theta x", -180, 180, -130);
	lightRotateY = lightingFolder->addSlider("  Theta y", -180, 180, 60);
	lightRotateZ = lightingFolder->addSlider("  Theta z", -180, 180, 30);
	gui->addBreak();
	
	//information board
	infoBoard = gui->addFolder("Panel Settings", ofColor::gray);
	//frame rate
	FRMonitor = infoBoard->addFRM(0.1);
	FRMonitor->setLabel("  Realtime App FR");
	appFRSetter = infoBoard->addSlider("  Max App FR", 10, 120, 60);
	appFRSetter->onSliderEvent(this, &Settings::onSliderEvent);
	opacitySlider = infoBoard->addSlider("  Panel opacity", 0, 1, 1);
	opacitySlider->onSliderEvent(this, &Settings::onSliderEvent);
	scaleInput = infoBoard->addTextInput("  Panel scale", "1.3");
	scaleInput->setInputType(ofxDatGuiInputType::NUMERIC);
	scaleInput->onTextInputEvent(this, &Settings::onTextInputEvent);
	gui->addBreak();

	hintLableShot = gui->addLabel("Press \"s\" to save screenshot.");
	hintLabelCamera = gui->addLabel("Press \"space\" to reset viewport.");

	gui->setTheme(theme, true);
}
void Settings::update() {
	gui->update();
}

void Settings::draw() {
	gui->draw();
}

void Settings::bindEventsToModel(AtomModel* model)
{
	//can also use bind(), but only for slider?
	//binding after setup
	playButton->onButtonEvent(model, &AtomModel::onPlayButton);
	pauseButton->onButtonEvent(model, &AtomModel::onPauseButton);
	stopButton->onButtonEvent(model, &AtomModel::onStopButton);

	// todo load data.
	//modelFrameNumSlider->onSliderEvent(model, &AtomModel::onFrameNumSlider);
	modelCurFrameSlider->onSliderEvent(model, &AtomModel::onCurFrameSlider);
	modelNeighborNumSlider->onSliderEvent(model, &AtomModel::onNeighborNumSlider);
	modelCenterIdSlider->onSliderEvent(model, &AtomModel::onCenterIdSlider);
	modelNeighborRadiusSlider->onSliderEvent(model, &AtomModel::onNeighborRadiusSlider);
	modelFrameRateSlider->onSliderEvent(model, &AtomModel::onFrameRateSlider);
	modelColorMixingSlider->onSliderEvent(model, &AtomModel::onOpacitySlider);
	modelCenterColorPicker->onColorPickerEvent(model, &AtomModel::onCenterColorPicker);
	modelNeighborColorPicker->onColorPickerEvent(model, &AtomModel::onNeighborColorPicker);
	modelForceFieldToggle->onToggleEvent(model, &AtomModel::onForceFieldToggle);
	//update panel like slider max value
	//modelFrameNumSlider->setMax(model->max_frame_num);
}


ofVec3f Settings::getLightOrientation()
{
	return ofVec3f(lightRotateX->getValue(), lightRotateY->getValue(), lightRotateZ->getValue());
}

void Settings::onSliderEvent(ofxDatGuiSliderEvent e)
{
	//cout << "Slider: " << e.target->getLabel() << " - " << e.target->getValue() << endl;
	if (e.target == appFRSetter) {
		float fr = e.target->getValue();
		ofSetFrameRate(fr);
		//cout << "Set frame rate to " << fr << endl;;
	}
	else if (e.target == opacitySlider) {

		gui->setOpacity(e.target->getValue());
	}

}

void Settings::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
	if (e.target == scaleInput) {
		string s = e.target->getText();
		if (s == "") {
			s = "1.3";
		}
		float scale = atof(s.c_str());
		cout << "set theme scale from " << theme->getScale() << " to " << scale << endl;
		if (scale > 0.5 && scale < 3) {
			theme->setScale(scale);
			gui->setTheme(theme, true);
		}
	}
}
