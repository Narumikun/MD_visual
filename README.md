# MD_visual
Molecular Dynamics visualization with openFrameworks.

**Develop environment**:
 - Environment is established using openframeworks.
 - need external addon `ofxDatGui`, clone to `of/addons/` from [braitsch/ofxDatGui@github](https://github.com/braitsch/ofxDatGui.git)
 - You'd better replace the old `of/libs/json/include/json.hpp` (ver 2.1.1) with the newest version(>= ver **3.6.1**) from [nlohmann/json@github](https://github.com/nlohmann/json).
 - If you want to use the visual studio(2019) solution (*.sln, *.vcxproj, etc.), please clone it to `oF_root_path/apps/MD_visual`.
 - data files are and should be located in `bin/data/`, sample data is located in `bin/data/atom_data`.