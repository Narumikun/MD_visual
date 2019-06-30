#pragma once

#include "atom_structure_res.h"
#include <cmath>

// basic element
Atom::Atom(ofJson atm_js, float axis_length) {
	int id = atm_js["id"], group_id = atm_js["group_id"], mole_id = atm_js["mole_id"];
	string group_type = atm_js["group_type"], element = atm_js["element"];
	vector<float> co = atm_js["coordinate"];
	double f_e = atm_js["f_e"], f_r = atm_js["f_r"], charge = atm_js["charge"];
	this->id = id;
	this->mole_id = mole_id;
	this->group_id = group_id;
	this->group_type = group_type;
	this->element = element;
	this->coordinate.push_back(ofVec3f(co[0] - axis_length / 2., co[1] - axis_length / 2., co[2] - axis_length / 2.));
	this->f_e = f_e;
	this->f_r = f_r;
	this->charge = charge;
}

void Atom::update(ofJson atm_js, float axis_length) {
	vector<float> co = atm_js["coordinate"];
	coordinate.push_back(ofVec3f(co[0] - axis_length / 2., co[1] - axis_length / 2., co[2] - axis_length / 2.));
};

// AtomGroup contains several atoms
AtomGroup::AtomGroup(int _group_id, int _mole_id, string _group_type) {
	this->group_id = _group_id;
	this->mole_id = _mole_id;
	this->group_type = _group_type;
}

AtomGroup::~AtomGroup() {
	for (auto it = atom_map.begin(); it != atom_map.end(); it++) {
		delete it->second;
	}
};


void AtomGroup::append_atom(Atom* _atom) {
	if (this->atom_map.count(_atom->id)) {
		throw "Atom already exist!";
	}
	else {
		atom_map.insert(make_pair(_atom->id, _atom));
	}
}

void AtomGroup::update(int frame_no) {
	if (!set_iso) {
		iso.setup(64);
		set_iso = true;
	}
	ofVec3f max_co = { 0.,0.,0. };
	ofVec3f dif = { 0.,0.,0. };
	for (auto map_it = this->atom_map.begin(); map_it != this->atom_map.end(); map_it++) {
		dif = map_it->second->coordinate[frame_no] - get_center(frame_no);
		for (int i = 0; i < 3; i++) {
			if (fabs(dif[i]) > max_co[i])
				max_co = ofVec3f(fabs(dif[i]));
		}
	}
	max_co += 3;
	iso_scale = max_co * 2;
	//cout << iso_scale << endl;
	vector<ofPoint> centers;
	for (auto map_it = this->atom_map.begin(); map_it != this->atom_map.end(); map_it++) {
		if (map_it->second->element != "H") {
			centers.push_back((map_it->second->coordinate[frame_no] - get_center(frame_no) + max_co) / iso_scale);
			//cout << (map_it->second.coordinate - min_co) / iso_scale << endl;
		}
	}
	this->iso.setCenters(centers);
	this->iso.setRadius(0.8 / 16. / iso_scale[0] * 10., 1.6 / 16. / iso_scale[0] * 10.);
	this->iso.update();
};

void AtomGroup::draw(int frame_no, ofColor color) {
	ofColor self_color;
	if (group_type == "TO") {
		self_color = ofColor::yellow;
	}
	else if (group_type == "U") {
		self_color = ofColor::purple;
	}
	else if (group_type == "Ph" || group_type == "B") {
		self_color = ofColor::blue;
	}
	else if (group_type == "Es") {
		self_color = ofColor::turquoise;
	}
	else {
		self_color = color;
		ofLogNotice() << group_type << endl;
	}

	float rand_max = 100;
	ofSeedRandom(group_id);

	double op = color.a;
	//rand_max should limit to: rand_max<255
	color.r = (color.r * op + ofRandom(rand_max) * ((color.r - 128 < 0) - 0.5) + self_color.r * 64.) / (64. + op);
	color.g = (color.g * op + ofRandom(rand_max) * ((color.g - 128 < 0) - 0.5) + self_color.g * 64.) / (64. + op);
	color.b = (color.b * op + ofRandom(rand_max) * ((color.b - 128 < 0) - 0.5) + self_color.b * 64.) / (64. + op);
	color.a = 255;

	// shininess is a value between 0 - 128, 128 being the most shiny //
	ofMaterial material;
	material.setShininess(120);
	// the light highlight of the material //
	material.setDiffuseColor(color);
	material.setSpecularColor(color);
	material.setAmbientColor(ofColor(255, 255, 255));
	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess(64);
	material.begin();


	ofPushMatrix();
	ofTranslate(get_center(frame_no));
	ofScale(iso_scale);
	iso.draw(color);
	ofPopMatrix();
}

ofVec3f AtomGroup::get_center(int frame_no) {
	if (this->cal_center[frame_no] == false) {
		ofVec3f _center = ofVec3f(0., 0., 0.);
		for (auto map_it = this->atom_map.begin(); map_it != this->atom_map.end(); map_it++) {
			_center += map_it->second->coordinate[frame_no];
		}
		_center /= this->atom_map.size();
		this->center[frame_no] = _center;
		this->cal_center[frame_no] = true;
	}
	return this->center[frame_no];
}

Atom3D::~Atom3D() {
	for (auto it = group_map.begin(); it != group_map.end(); it++) {
		delete it->second;
	}
};

void Atom3D::setup(string prefix) {
	int frame_no = 0;
	char buf[100];
	sprintf_s(buf, "%03d.json", frame_no);
	string filepath = prefix + string(buf);
	ofJson atom_info;
	if (!std::filesystem::exists(filepath)) {
		ofLogNotice() << "file not exist: " << filepath;
		throw filepath;
	}
	else {
		ifstream in_file(filepath);
		in_file >> atom_info;
		cout << " *loading frame 0...";
		//ofLogNotice() << "file loaded: " << fp;
	}
	axis_length.push_back(atom_info["length"]);
	for (ofJson::iterator it = atom_info.begin(); it != atom_info.end(); ++it) {
		if (it.key() != "length") {
			ofJson atm_json = it.value();
			Atom* p_atm = NULL;
			p_atm = new Atom(atm_json, axis_length[0]);
			append_atom(p_atm);
			//Atom new_atom(atm_json, axis_length[0]);
			//append_atom(new_atom);
		}
	}
	this->frames = 1;
};

void Atom3D::update(string prefix, int frames) {
	if (frames > this->frames) {
		int frame_no = this->frames;
		for (frame_no; frame_no < frames; frame_no++) {
			char buf[100];
			sprintf_s(buf, "%03d.json", frame_no);
			string filepath = prefix + string(buf);
			ofJson atom_info;
			if (std::filesystem::exists(filepath)) {
				ifstream in_file(filepath);
				in_file >> atom_info;
				cout << "\r *loading frame " << frame_no << "...";
				//ofLogNotice() << "file loaded: " << fp;
				axis_length.push_back(atom_info["length"]);
				for (ofJson::iterator it = atom_info.begin(); it != atom_info.end(); ++it) {
					if (it.key() != "length") {
						ofJson atm_json = it.value();
						int id = atm_json["id"], group_id = atm_json["group_id"];
						group_map[group_id]->atom_map[id]->update(atm_json, axis_length[frame_no]);
					}
				}
			}
			else {
				ofLogNotice() << filepath << " File not exist! stop reading!";
				break;
			}
		}
		this->frames = frame_no;
	}
};

void Atom3D::load_data(string prefix, int frames) {
	if (this->frames == 0)
		setup(prefix);
	update(prefix, frames);
};

// Atom3D contains all the atom groups for one frame
void Atom3D::append_atom(Atom* _atom) {
	if (this->group_map.count(_atom->group_id)) {
		// test
		//group_map[_atom.group_id].append_atom(_atom);
		group_map[_atom->group_id]->append_atom(_atom);
	}
	else {
		//ofLogNotice() << "new atom group id=" << _atom.group_id;

		// test
		//AtomGroup new_group(_atom.group_id, _atom.mole_id, _atom.group_type);
		//new_group.append_atom(_atom);
		//group_map.insert(make_pair(_atom.group_id, new_group));

		AtomGroup* agp = NULL;
		agp = new AtomGroup(_atom->group_id, _atom->mole_id, _atom->group_type);
		agp->append_atom(_atom);
		group_map.insert(make_pair(_atom->group_id, agp));


		max_group_id = max(max_group_id, _atom->group_id);
	}
}

vector<int> Atom3D::get_neighbor_group_id(const int center_group_id, float r, int cur_frame) {
	
	// test
	//AtomGroup& c_grp = this->group_map[center_group_id];
	AtomGroup* c_grp = this->group_map[center_group_id];

	vector<float> distance;
	vector<int> arg_vec;
	for (auto it = this->group_map.begin(); it != this->group_map.end(); it++) {
		//if ((it->first != center_group_id) && (it->second.mole_id != c_grp.mole_id)) {
		if (it->first != center_group_id) {
			// test
			//float _d = c_grp.get_center(cur_frame).distance(it->second.get_center(cur_frame));
			float _d = c_grp->get_center(cur_frame).distance(it->second->get_center(cur_frame));
			
			if (_d < r) {
				distance.push_back(_d);
				arg_vec.push_back(it->first);
			}
		}
	}
	return _arg_sort(distance, arg_vec);
}

vector<int> Atom3D::_arg_sort(vector<float> ivec, vector<int> arg_vec) {
	const int COUNT = ivec.size();
	for (int i = 1; i < COUNT; i++) {
		for (int j = 0; j < COUNT - i; j++) {
			if (ivec[j] > ivec[j + 1]) {
				swap(ivec[j], ivec[j + 1]);
				swap(arg_vec[j], arg_vec[j + 1]);
			}
		}
	}
	return arg_vec;
}

void Atom3D::setup_particle(int cur_frame, int cent_id, vector<int> neighbor_id, int neighbor_num) {
	if (_draw_particle) {
		/*cout << neighbor_id.size() << endl;*/
		// update atom coordinate
		ps.clear();
		// test
		//AtomGroup& ct_grp = group_map[cent_id];
		AtomGroup* ct_grp = group_map[cent_id];
		int atom_num = 0;
		for (auto c_it = ct_grp->atom_map.begin(); c_it != ct_grp->atom_map.end(); c_it++) {
			Atom* c_atm = c_it->second;
			if (c_atm->element != "H") {
				ofVec3f force = { 0.,0.,0. };
				int max_neighbors = min(int(neighbor_id.size()), neighbor_num);
				for (int _n = 0; _n < max_neighbors; _n++) {
					int n_id = neighbor_id[_n];
					AtomGroup* n_grp = group_map[n_id];
					for (auto n_it = n_grp->atom_map.begin(); n_it != n_grp->atom_map.end(); n_it++) {
						Atom* n_atm = n_it->second;
						float distance = c_atm->coordinate[cur_frame].distance(n_atm->coordinate[cur_frame]);
						if (distance > 6.) {
							ofVec3f direction = (c_atm->coordinate[cur_frame] - n_atm->coordinate[cur_frame]).normalize();
							force += direction * cal_frc(c_atm, n_atm, cur_frame);
						}
					}
				}
				ps.push_back(new particleSystem(c_atm->coordinate[cur_frame], force * 2));
				//cout << force*10 << endl;
				atom_num++;
			}
		}
	}
};

void Atom3D::update_particle() {
	if (_draw_particle) {
		for (int i = 0; i < ps.size(); i++) {
			ps.at(i)->update();
			ps.at(i)->addParticle();
		}
	}
};

void Atom3D::draw_particle() {
	if (_draw_particle) {
		for (int i = 0; i < ps.size(); i++) {
			ps.at(i)->display();
		}
	}
};

float Atom3D::cal_vdw(const Atom* atom1, const Atom* atom2, float r) {
	float r_i = atom1->f_r, r_j = atom2->f_r;
	float f_v = 0.;
	if (r < 9.5) {
		float r_ij = pow((pow(r_i, 6) + pow(r_j, 6) / 2), 1 / 6);
		float f_e = 2 * sqrt(atom1->f_e * atom2->f_e) / pow(r_i, 3) / pow(r_j, 3);
		f_v = 18 * f_e / r_ij * (pow((r_ij / r), 10) - pow((r_ij / r), 7));
	}
	return f_v;
}

float Atom3D::cal_elec(const Atom* atom1, const Atom* atom2, float r) {
	float f_e = 0.;
	if (r < 9.5) {
		f_e = atom1->charge * atom2->charge / r / r;
	}
	return f_e;
}

float Atom3D::cal_frc(const Atom* atom1, const Atom* atom2, int frame_no) {
	float r = atom1->coordinate[frame_no].distance(atom2->coordinate[frame_no]);
	return cal_vdw(atom1, atom2, r) + cal_elec(atom1, atom2, r);
}

// Axis implement, can use ofbox instead
Axis::Axis() {
	length = 300.0;
}

Axis::Axis(float _l) {
	length = _l;
}

void Axis::update(float _l) {
	this->length = _l;
	int i_node = 0;
	for (float x = -0.5; x < 1; x += 1) {
		for (float y = -0.5; y < 1; y += 1) {
			for (float z = -0.5; z < 1; z += 1) {
				node[i_node].setPosition(x * length, y * length, z * length);
				i_node++;
			}
		}
	}
	for (int i_line = 0; i_line < 12; i_line++) {
		line[i_line].addVertex(node[connect[i_line][0]].getGlobalPosition());
		line[i_line].addVertex(node[connect[i_line][1]].getGlobalPosition());
	}
}

void Axis::draw() {
	ofMaterial material;
	material.setShininess(120);
	// the light highlight of the material //
	material.setDiffuseColor(ofColor(0));
	material.setSpecularColor(ofColor(0));
	material.setAmbientColor(ofColor(0));
	// shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess(64);
	material.begin();
	for (int i_line = 0; i_line < 12; i_line++) {
		line[i_line].draw();
	}
}

void Axis::draw(ofColor color) {
	ofSetColor(color);
	for (int i_line = 0; i_line < 12; i_line++) {
		line[i_line].draw();
	}
}
