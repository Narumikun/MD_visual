#include "atom_model.h"

AtomModel::AtomModel(Settings* s)
{
	/* initial all paramters from settings
	int frame_num, int center_id, int neighbor_num, int neighbor_radius,
	int frame_rate, int color_mixing, ofColor center_color, neighbor_color,
	bool forcefield_toggle,
	*/
	settings = s;
	if (s != NULL) {
		frame_num = s->modelFrameNumSlider->getValue();
		center_id = s->modelCenterIdSlider->getValue();
		neighbor_num = s->modelNeighborNumSlider->getValue();
		neighbor_radius = s->modelNeighborRadiusSlider->getValue();
		frame_rate = s->modelFrameRateSlider->getValue();
		color_mixing = s->modelColorMixingSlider->getValue();
		center_color = s->modelCenterColorPicker->getColor();
		neighbor_color = s->modelNeighborColorPicker->getColor();
		forcefield_toggle = s->modelForceFieldToggle->getChecked();
	}
	else {
		frame_num = 1;
		center_id = 137;
		neighbor_num = 10;
		neighbor_radius = 20;
		frame_rate = 1;
		color_mixing = 20;
		center_color = ofColor(148, 0, 211);
		neighbor_color = ofColor(3, 200, 158);
		forcefield_toggle = false;
	}
}

void AtomModel::loadData(int frames, string prefix)
{
	// all filepaths must be absolute or relative to bin/MD_visual_debug.exe
	// please save data files to "bin/data/", not project root. And don't include them in project.
	ofLogNotice() << "Ready to read " << frames << " frames with prefix " << prefix;
	
	int max_frame_num = atom3d.loadData(prefix, frames);
	frame_num = atom3d.frame_num;
	updateNeighbors(center_id, neighbor_radius);
	path_prefix = prefix;
	if (atom3d.group_map.count(center_id) == 0) {
		// ensure the validation of center_id
		center_id = atom3d.group_map.begin()->first;
	}
	settings->modelFrameNumSlider->setMax(max_frame_num);
	settings->modelFrameNumSlider->setValue(frame_num,false);
	settings->modelCurFrameSlider->setMax(frame_num);
	cur_frame = min(frame_num - 1, cur_frame);// ensure the validation, otherwise error when draw before update?
	settings->modelCurFrameSlider->setValue(cur_frame + 1);

	ofLogNotice() << "Load " << frame_num << " frames";
	ofLogNotice() << "Total " << max_frame_num << " frames exist.";
}

void AtomModel::setup(int frames, string prefix)
{
	loadData(frames, prefix);
	cur_frame = 0;
	axis.update(atom3d.axis_length[cur_frame]);
	atom3d.group_map[center_id]->update(cur_frame);
	int max_neighbors = min(int(frames_neighbor_id[cur_frame].size()), neighbor_num);
	for (int i = 0; i < max_neighbors; i++) {
		atom3d.group_map[frames_neighbor_id[cur_frame][i]]->update(cur_frame);
	}
	atom3d.setupParticle(cur_frame, center_id, frames_neighbor_id[cur_frame], neighbor_num);
}

void AtomModel::update()
{
	if (playing) {
		cur_frame = (ofGetElapsedTimeMicros() / (1000000 / frame_rate) + init_frame) % frame_num;
	}
	else {
		cur_frame = init_frame % frame_num;
	}
	// only update when current frame changed
	if (cur_frame != last_frame || draw_neighbor_changed) {
		axis.update(atom3d.axis_length[cur_frame]);
		atom3d.group_map[center_id]->update(cur_frame);
		int max_neighbors = min(int(frames_neighbor_id[cur_frame].size()), neighbor_num);
		for (int i = 0; i < max_neighbors; i++) {
			atom3d.group_map[frames_neighbor_id[cur_frame][i]]->update(cur_frame);
		}
		last_frame = cur_frame;
		draw_neighbor_changed = false;

		//update cur_frame when running
		if (!settings->modelCurFrameSlider->getMouseDown()) {
			settings->modelCurFrameSlider->setValue(cur_frame, false);
		}
	}
	//when show forcefield, it should always be drawn even if paused,
	if (forcefield_toggle) {
		if (forcefield_changed) {
			atom3d.setupParticle(cur_frame, center_id, frames_neighbor_id[cur_frame], neighbor_num);
			forcefield_changed = false;
		}
		atom3d.updateParticle();
	}
}

void AtomModel::draw() {
	axis.draw();
	// here you will draw your object
	atom3d.group_map[center_id]->draw(cur_frame, ofColor(center_color, color_mixing));
	int max_neighbors = min(int(frames_neighbor_id[cur_frame].size()), neighbor_num);
	for (int i = 0; i < max_neighbors; i++) {
		atom3d.group_map[frames_neighbor_id[cur_frame][i]]->draw(cur_frame, ofColor(neighbor_color, color_mixing));
	}

	//draw particle system
	if (forcefield_toggle && !playing) {
		atom3d.drawParticle();
	}
}

void AtomModel::updateNeighbors(int _center_id, float _radius)
{
	if (_radius < 0) {
		_radius = neighbor_radius;
	}
	frames_neighbor_id.clear();
	for (int i = 0; i < frame_num; i++) {
		frames_neighbor_id.push_back(atom3d.getNeighborGroupId(_center_id, _radius));
	}
	forcefield_changed = true;
	draw_neighbor_changed = true;
}

float AtomModel::getAxisLength()
{
	if (atom3d.frame_num > 0) {
		return atom3d.axis_length[0];
	}
	else {
		return 0.f;
	}
}

void AtomModel::onPlayButton(ofxDatGuiButtonEvent e)
{
	if (!playing) {
		playing = true;
		ofResetElapsedTimeCounter();
		settings->modelForceFieldToggle->setChecked(false);
		forcefield_toggle = false;
		ofLogNotice() << "hit Play button.";
	}
}

void AtomModel::onPauseButton(ofxDatGuiButtonEvent e)
{
	playing = false;
	init_frame = cur_frame;
	ofLogNotice() << "hit Pause button. init_frame=" << init_frame;
}

void AtomModel::onStopButton(ofxDatGuiButtonEvent e)
{
	playing = false;
	cur_frame = 0;
	init_frame = 0;
	forcefield_toggle = false;
	ofLogNotice() << "hit Stop button.";
}

void AtomModel::onCurFrameSlider(ofxDatGuiSliderEvent e)
{
	init_frame = int(e.target->getValue() - 1) % frame_num;
	ofResetElapsedTimeCounter();
	forcefield_changed = true;
	draw_neighbor_changed = true;
	ofLogNotice() << "onCurFrameSlider called: " << e.target->getValue();
}

void AtomModel::onFrameNumSlider(ofxDatGuiSliderEvent e)
{
	if (playing) {
		// load data need more time, so pause first if playing
		onPauseButton();
		loadData(int(e.target->getValue()), path_prefix);
		onPlayButton();
	}
	else {
		loadData(int(e.target->getValue()), path_prefix);
	}
	//ofLogNotice() << "Frame num slider: set cur frame max to " << frame_num;
}

void AtomModel::onCenterIdSlider(ofxDatGuiSliderEvent e)
{
	int id = e.target->getValue();
	if (cur_frame >= 0) {
		if (atom3d.group_map.count(id) == 1) {
			center_id = id;
			last_frame = -1; 
			updateNeighbors(center_id, neighbor_radius);
		}
		else if (center_id >= 0) {
			// has been set before
			e.target->setValue(center_id);
			ofLogNotice() << " center_id invalid, reset from " << id << " to " << center_id << endl;
		}
	}
}

void AtomModel::onNeighborNumSlider(ofxDatGuiSliderEvent e)
{
	if (e.target->getValue() >= 0) {
		neighbor_num = e.target->getValue();
	}
	updateNeighbors(center_id, neighbor_radius);
}

void AtomModel::onNeighborRadiusSlider(ofxDatGuiSliderEvent e)
{
	if (e.target->getValue() >= 0) {
		neighbor_radius = e.target->getValue();
	}
	updateNeighbors(center_id, neighbor_radius);
}

void AtomModel::onFrameRateSlider(ofxDatGuiSliderEvent e)
{
	frame_rate = e.target->getValue();
	init_frame = cur_frame;
	ofResetElapsedTimeCounter();
	ofLogNotice() << "Model frame rate: " << frame_rate;
}

void AtomModel::onOpacitySlider(ofxDatGuiSliderEvent e)
{
	color_mixing = e.target->getValue();
	ofLogNotice() << "Color Mixing: " << color_mixing;
}

void AtomModel::onCenterColorPicker(ofxDatGuiColorPickerEvent e)
{
	center_color = e.target->getColor();
	ofLogNotice() << "Model center_color: " << center_color;
}

void AtomModel::onNeighborColorPicker(ofxDatGuiColorPickerEvent e)
{
	neighbor_color = e.target->getColor();
	ofLogNotice() << "Model neighbor_color: " << neighbor_color;
}

void AtomModel::onForceFieldToggle(ofxDatGuiToggleEvent e)
{
	forcefield_toggle = e.target->getChecked();
	onPauseButton();
}
