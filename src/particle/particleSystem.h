#pragma once
#include "ofMain.h"
#include "particle.h"
#include "particleRectangle.h"

class particleSystem
{
public:
	particleSystem(ofPoint, ofVec3f _force = ofVec3f(0.,0.,0.));
	~particleSystem();

	void update();
	void display();
	void addParticle();
	void applyForce(ofPoint);
	void changeLocation(ofVec3f _location);

	ofPoint origin;
	ofVec3f force;
	vector<particle*> particles;

};

