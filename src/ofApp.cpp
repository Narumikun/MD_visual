#include "ofApp.h"
#include <windows.h>

//--------------------------------------------------------------
void ofApp::setup() {
	settings = Settings();
	settings.setup();
	model = new AtomModel(&settings);
	model->setup(50, "data/atom_data/atom_info_");
	settings.bindEventsToModel(model);

	// turn on smooth lighting //
	ofSetSmoothLighting(true);

	// Directional Lights emit light based on their orientation, regardless of their position //
	directionalLight.setDiffuseColor(ofColor(255, 255, 255));
	directionalLight.setSpecularColor(ofColor(255, 255, 255));
	directionalLight.setDirectional();
	directionalLight.setParent(mycam, true);

	mycam.setDistance(model->getAxisLength() * 2);
	ofResetElapsedTimeCounter();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (settings.gui->getMouseDown()) {
		mycam.disableMouseInput();
	}
	else {
		mycam.enableMouseInput();
	}
	model->update();
	settings.update();
	// dirction x-right,y-up,z-camera/depth,(0,0,0)
	directionalLight.setOrientation(settings.getLightOrientation());
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableDepthTest();
	ofBackgroundGradient(ofColor::white, ofColor::dimGray);

	// enable lighting //
	ofEnableLighting();
	directionalLight.enable();

	mycam.begin();
	model->draw();
	mycam.end();

	directionalLight.disable();
	ofDisableLighting();
	//end light and model drawing
	ofDisableDepthTest();
	settings.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 's') {
		// WARNING: ofSaveScreen(fp), fp is default relative to data/, not relative to *.exe.
		string fp = "screenshots/screenshot-" + ofGetTimestampString() + ".png";
		ofLogNotice() << "take screenshot: " << fp;
		ofSleepMillis(1000);
		ofSaveScreen(fp);
	}
	else if (key == ' ') {
		ofLogNotice() << "reset camera.";
		mycam.reset();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	//button: left 0, center 1, right 2
	if (button == 0) {

	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}