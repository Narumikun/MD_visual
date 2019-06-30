#include "particle.h"


//particle构造函数
particle::particle(ofPoint point)
{
	acceleration.set(0, 0, 0); 
	ofTime time = ofGetCurrentTime();
	ofSeedRandom(time.getAsNanoseconds());
	float x = ofRandom(-0.05, 0.05);
	float y = ofRandom(-0.05, 0.05);
	float z = ofRandom(-0.05, 0.05);

	velocity.set(x, y, z);
	location = point;
	lifespan = 255.0;
	mass = 1;
}

particle::~particle()
{
}


void particle::update() {
	velocity += acceleration;
	location += velocity;
	//复位加速度
	acceleration *= 0;
	lifespan -= 5.0;
}

void particle::display() {

	ofColor color = ofColor(166, 52, 169, lifespan);
	//float rand_min = -5;
	//float rand_max = 0;
	//ofTime time = ofGetCurrentTime();
	//ofSeedRandom(time.getAsNanoseconds());
	//color.r = 208 + ofRandom(rand_min, rand_max);
	//color.g = 255 + ofRandom(rand_min, rand_max);
	//color.b = 63 + ofRandom(rand_min, rand_max);

	// shininess is a value between 0 - 128, 128 being the most shiny //
	ofMaterial material;
	material.setShininess(120);
	// the light highlight of the material //
	material.setDiffuseColor(color);
	material.setSpecularColor(color);
	material.setAmbientColor(color);
	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess(64);
	material.begin();

	//cout << location << endl;

	//ofDrawIcoSphere(location.x, location.y, location.z, mass*6);
	ofDrawSphere(location.x, location.y, location.z, 0.08);
}

bool particle::isDead() {
	if (lifespan < 0.0) {
		return true;
	}
	else {
		return false;
	}
}

void particle::applyForce(ofPoint force) {
	force /= mass;
	acceleration += force;
}