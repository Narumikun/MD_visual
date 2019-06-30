#include "particleSystem.h"



particleSystem::particleSystem(ofPoint location, ofVec3f _force)
{
	origin = location;
	force = _force;
}


particleSystem::~particleSystem()
{
}


void particleSystem::addParticle() {
	particles.push_back(new particle(origin));
}

void particleSystem::update() {
	for (int i = 0; i < particles.size(); i++) {
		particles.at(i)->applyForce(force);
		particles.at(i)->update();
		if (particles.at(i)->isDead()) {
			particles.erase(particles.begin() + i);
		}
	}
}

void particleSystem::display() {
	//cout << particles.size() << endl;
	for (int i = 0; i < particles.size(); i++) {
		particles.at(i)->display();
		//cout << particles.at(i)->location << endl;
	}
}

void particleSystem::applyForce(ofPoint f) {
	for (int i = 0; i < particles.size(); i++) {
		particles.at(i)->applyForce(f);
	}
	/* c++11ÐÂÓï·¨
	for (auto &p : particles) {
		p.applyForce(f);
	}
	*/
}


void particleSystem::changeLocation(ofVec3f _location) {
	origin.x = _location[0];
	origin.y = _location[1];
	origin.z = _location[2];

}