#pragma once
//#define DEBUG
#include "settings.h"
#include "atom_model.h"
#include "particle/particleSystem.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofEasyCam mycam;
	// light
	//ofLight pointLight;
	//ofLight spotLight;
	ofLight directionalLight;
	ofLight ambientLight;
	//ofMaterial material;
	//ofColor lightColor;
	//ofColor materialColor;

	Settings settings;
	AtomModel* model;
};