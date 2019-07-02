#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "ofMain.h"
#include "marching_cubes.h"
#include "particle/particle.h"
#include "particle/particleSystem.h"


class Atom {
public:
	// atom attributes
	int id = 0, group_id = 0, mole_id = 0;
	string group_type = "", element = "";
	double f_e = 0., f_r = 0., charge = 0., mass = 0.;
	vector<ofVec3f> coordinate;

	Atom() {};
	// read atom information from json. get all attributes.
	Atom(ofJson atm_js, float axis_length);
	// just update coordiante in different frame.
	void update(ofJson atm_js, float axis_length);
};

class AtomGroup {
public:
	AtomGroup() {};
	AtomGroup(int _group_id, int _mole_id, string _group_type);
	~AtomGroup();
	// atom group attributes, save atom in a map<atom_id atom>
	int group_id = 0, mole_id = 0;
	string group_type = "";
	map<int, Atom*> atom_map;

	void update(int frame_no);
	void draw(int frame_no, ofColor color = ofColor(3, 168, 158, 240));

	void appendAtomToGroup(Atom *_atom);
	ofVec3f getCenter(int cur_frame);

private:
	// meta ball
	MarchingCubes iso;
	bool set_iso = false;
	ofVec3f iso_scale;
	bool cal_center[1000] = { false };
	ofVec3f center[1000];
};

class Atom3D {
public:
	// save atom group in a map<atom_group_id atom_group>
	vector<float> axis_length;
	int frame_num = 0, max_frame_num=0, max_group_id = 0;
	map<int, AtomGroup*> group_map;

	Atom3D() {};
	~Atom3D();
	void appendAtomTo3D(Atom* _atom);
	int loadData(string prefix, int frames);

	// draw paticle system
	void setupParticle(int cur_frame, int cent_id, vector<int> neighbor_id, int neighbor_num);
	void updateParticle();
	void drawParticle();

	vector<int> getNeighborGroupId(const int center_group_id, float r = 15.f, int cur_frames = 0);

private:
	vector<particleSystem*> ps;
	bool _draw_particle = true;
	vector<int> _argSort(vector<float> ivec, vector<int> arg_vec);

	// calculate force
	float calVdw(const Atom* atom1, const Atom* atom2, float r);
	float calElec(const Atom* atom1, const Atom* atom2, float r);
	float calForce(const Atom* atom1, const Atom* atom2, int frame_no);
};

class Axis {
public:
	Axis();
	Axis(float _l);
	void update(float _l);
	void draw();
	void draw(ofColor color);
	float length;

private:
	const int connect[12][2] = { {0,1}, {0,2}, {0,4}, {3,1}, {3,2}, {3,7}, {5,4}, {1,5}, {5,7}, {6,2}, {6,7}, {6,4} };
	ofPolyline line[12];
	ofNode node[8];

};

