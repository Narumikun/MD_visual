#include "ofApp.h"
#include <windows.h>

//--------------------------------------------------------------
void ofApp::setup() {
	settings = Settings();
	settings.setup();
	model = new AtomModel(&settings);
	model->setup(5, "data/atom_data/atom_info_");
	settings.bindEventsToModel(model);
	settings.directionalLight.setParent(easy_cam, true);
	easy_cam.setDistance(model->getAxisLength() * 2);
	ofResetElapsedTimeCounter();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (settings.gui->getMouseDown()) {
		easy_cam.disableMouseInput();
	}
	else {
		easy_cam.enableMouseInput();
	}
	model->update();
	settings.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableDepthTest();
	ofBackgroundGradient(ofColor::white, ofColor::dimGray);

	// enable lighting //
	ofEnableLighting();
	settings.directionalLight.enable();

	easy_cam.begin();
	model->draw();
	easy_cam.end();

	settings.directionalLight.disable();
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
		easy_cam.reset();
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