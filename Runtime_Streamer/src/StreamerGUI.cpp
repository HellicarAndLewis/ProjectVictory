#include "StreamerGUI.h"

// ----------------------------------------------------------
/*
void streamer_gui_thread(void* user) {
  StreamerGUI* gui = static_cast<StreamerGUI*>(user);
  while(true) {
    std::string text = ofSystemTextBoxDialog("What");
    sleep(5);
  }
}
*/

// ----------------------------------------------------------

StreamerGUI::StreamerGUI() {
  /*
  uv_mutex_init(&mutex);
  uv_cond_init(&cv);
  */
}

StreamerGUI::~StreamerGUI() {
  printf("_______________ ERROR:::::::::::::: CLEANUP GUI THREAD ::::::::::::::::\n");
}

bool StreamerGUI::setup() {

  // effects panel
  fx_panel.setup("Effects", "ofxgui/effects.xml");
  fx_panel.add(fx_khronos_enabled.setup("1. Khronos", false));
  fx_panel.add(fx_color_map_enabled.setup("2. Color Map", false));
  fx_panel.add(fx_bad_tv_enabled.setup("3. Bad TV", false));
  fx_panel.add(fx_scanlines_enabled.setup("4. Scanlines", false));
  fx_panel.add(fx_rgb_shift_enabled.setup("5. RGB Shift", false));
  fx_panel.add(fx_flow_lines_enabled.setup("6. Flow Lines", false));
  fx_panel.add(fx_grid_distort_enabled.setup("7. Grid Distort", false));
  fx_panel.loadFromFile("ofxgui/effects.xml");

  // effect: khronos
  khronos_panel.setup("1. Effect Khronos", "ofxgui/effect_khronos.xml");
  khronos_panel.add(khronos_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  khronos_panel.add(khronos_mag_scaler.setup("Mag Scaler", 1.0f, 1.0f, 10.0f));
  khronos_panel.loadFromFile("ofxgui/effect_khronos.xml");

  // effect: color map
  colmap_panel.setup("2. Effect Color Map", "ofxgui/effect_color_map.xml");
  colmap_panel.add(colmap_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  colmap_panel.loadFromFile("ofxgui/effect_color_map.xml");

  // effect: bad tv
  btv_panel.setup("3. Effect Bad TV", "ofxgui/effect_bad_tv.xml");
  btv_panel.add(btv_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  btv_panel.add(btv_thick_distort.setup("Thick Distort", 0.0f, 0.1f, 20.0f));
  btv_panel.add(btv_fine_distort.setup("Fine Distort", 0.0f, 0.1f, 20.0f));
  btv_panel.add(btv_distort_speed.setup("Distort Speed", 0.0f, 0.0f, 1.0f));
  btv_panel.add(btv_roll_speed.setup("Roll Speed", 0.0f, 0.0f, 1.0f));
  btv_panel.loadFromFile("ofxgui/effect_bad_tv.xml");

  // effect: scan lines
  sl_panel.setup("4. Effect Scan Lines", "ofxgui/effect_scanlines.xml");
  sl_panel.add(sl_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  sl_panel.add(sl_count.setup("Count", 50.0f, 50.0f, 1000.0f));
  sl_panel.add(sl_s_intensity.setup("Intensity: S", 0.0f, 0.0f, 2.0f));
  sl_panel.add(sl_n_intensity.setup("Intensity: N", 0.0f, 0.0f, 2.0f));
  sl_panel.loadFromFile("ofxgui/effect_scanlines.xml");

  // effect: rgb shift
  rgb_panel.setup("5. Effect RGB Shift", "ofxgui/effect_rgb_shift.xml");
  rgb_panel.add(rgb_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  rgb_panel.add(rgb_rgb_amount.setup("RGB Amount", 0.0f, 0.0f, 0.1f));
  rgb_panel.add(rgb_angle.setup("Angle", 0.0f, 0.0f, 2.0f));
  rgb_panel.loadFromFile("ofxgui/effect_rgb_shift.xml");

  // effect: flow lines
  flow_panel.setup("6. Effect Flow Lines", "ofxgui/effect_flow_lines.xml");
  flow_panel.add(flow_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  flow_panel.add(flow_step_size.setup("Step Size", 2.0f, 2.0f, 20.0f));
  flow_panel.add(flow_line_scale.setup("Line Scale", 1.0f, 1.0f, 20.0f));
  flow_panel.add(flow_threshold.setup("Threshold", 0.1f, 0.1f, 2.0f));

  // effect: grid
  grid_panel.setup("7. Effect grid", "ofxgui/effect_grid.xml");
  grid_panel.add(grid_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  grid_panel.add(grid_step_size.setup("Step Size", 8.0f, 8.0f, 32.0f));
  grid_panel.add(grid_particle_radius.setup("Particle Radius", 2.0f, 2.0f, 16.0f));
  grid_panel.add(grid_force_scale.setup("Force Scale", 1.0f, 1.0f, 100.0f));
  grid_panel.add(grid_spring_strength.setup("Spring Strength", 0.005f, 0.005f, 0.2f));
  grid_panel.add(grid_drag.setup("Drag", 0.0f, 0.0f, 1.0f));
  grid_panel.add(grid_show_grid.setup("Show Grid", false));
  grid_panel.loadFromFile("ofxgui/effect_grid.xml");

  // web panel
  web_panel.setup("Web System", "ofxgui/websystem.xml");
  web_panel.add(web_is_enabled.setup("Enable", true));
  web_panel.add(web_shoutouts_enabled.setup("Enable Shoutouts", true));
  web_panel.add(web_commands_enabled.setup("Enable Commands", true));
  web_panel.add(web_decay_effects.setup("Decay Effects", true));
  web_panel.add(web_count_hash_tags.setup("Count Hash Tags", true));
  web_panel.loadFromFile("ofxgui/websystem.xml");

  // vote gui
  vote_panel.setup("Voting", "ofxgui/voting.xml");
  vote_panel.add(vote_topic_a.setup("Topic 1", ""));
  vote_panel.add(vote_topic_b.setup("Topic 2", ""));

  // positioning
  int rgb_h = rgb_panel.getHeight();
  int web_h = web_panel.getHeight();
  int web_w = web_panel.getWidth();
  int fx_h = fx_panel.getHeight();
  int btv_h = btv_panel.getHeight();
  int kr_h = khronos_panel.getHeight();
  int col_h = colmap_panel.getHeight();
  int sl_h = sl_panel.getHeight();
  int fl_h = flow_panel.getHeight();

  web_panel.setPosition(10, 10);
  vote_panel.setPosition(10, web_h + 10);

  fx_panel.setPosition(web_w + 20, 10);
  khronos_panel.setPosition(web_w + 20, fx_h + 20);
  colmap_panel.setPosition(web_w + 20, fx_h + kr_h + 30);
  btv_panel.setPosition(web_w + 20, fx_h + kr_h + col_h + 40);
  sl_panel.setPosition(web_w + 20,  fx_h + kr_h + col_h + btv_h + 50);
  rgb_panel.setPosition(web_w + 20, fx_h + kr_h + col_h + btv_h + sl_h + 60);
  flow_panel.setPosition(web_w + 20,  fx_h + rgb_h + btv_h + kr_h + col_h + sl_h + 70);
  grid_panel.setPosition(web_w + fx_panel.getWidth() + 30, fx_panel.getHeight() + 20);

  receiver.setup(4455);

  return true;
}


void StreamerGUI::update() {
  while(receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);

    std::string address = m.getAddress();
    if(address == "/overlay/text_a") {
      std::string text_a = m.getArgAsString(0);
      printf(">>>>>>>>>>>> %s <<<<<<<<<<\n", text_a.c_str());
    }
  }
  
}

bool StreamerGUI::onKeyPressed(int key) {

  switch(key) {
    case '1': { fx_khronos_enabled = !fx_khronos_enabled; return true;  }
    case '2': { fx_color_map_enabled = !fx_color_map_enabled; return true;  }
    case '3': { fx_bad_tv_enabled = !fx_bad_tv_enabled; return true;  }
    case '4': { fx_scanlines_enabled = !fx_scanlines_enabled; return true; } 
    case '5': { fx_rgb_shift_enabled = !fx_rgb_shift_enabled; return true; }
    case '6': { fx_flow_lines_enabled = !fx_flow_lines_enabled; return true; } 
    case '7': { fx_grid_distort_enabled = !fx_grid_distort_enabled; return true; }
  }

  return false;
}

void StreamerGUI::draw() {
  fx_panel.draw();
  web_panel.draw();
  vote_panel.draw();

  if(fx_khronos_enabled) {
    khronos_panel.draw();
  }

  if(fx_color_map_enabled) {
    colmap_panel.draw();
  }
  
  if(fx_scanlines_enabled) {
    sl_panel.draw();
  }

  if(fx_flow_lines_enabled) {
    flow_panel.draw();
  }

  if(fx_bad_tv_enabled) {
    btv_panel.draw();
  }

  if(fx_rgb_shift_enabled) {
    rgb_panel.draw();
  }
  
  if(fx_grid_distort_enabled) {
    grid_panel.draw();
  }
}

/*
void StreamerGUI::onClickedTopicA() {
}

void StreamerGUI::onClickedTopicB() {
}
*/

