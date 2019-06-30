#include "particleRectangle.h"

//派生类构造函数
particleRectangle::particleRectangle(ofPoint point):particle(point)
{
}


particleRectangle::~particleRectangle()
{
}

void particleRectangle::display() {

	ofSetColor(103, 160, 237, lifespan);
	ofDrawRectangle(location, 12, 12);
}