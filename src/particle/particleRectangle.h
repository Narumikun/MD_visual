#pragma once
#include "particle.h"

class particleRectangle :
	public particle
{
public:
	particleRectangle(ofPoint);
	~particleRectangle();

	void display();
};

