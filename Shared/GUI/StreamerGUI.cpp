#include "StreamerGUI.h"

// ----------------------------------------------------------

void streamer_gui_cleanup_name(std::string& s) {
  std::replace(s.begin(), s.end(), '.', '_'); 
}

// ----------------------------------------------------------

OverlayImage::OverlayImage(StreamerGUI* gui)
  :gui(gui) 
{
}

void OverlayImage::onPressed(bool& v) {

  if(!gui) {
    printf("error: overlay image event handled called; cannot do anything because gui member is not set. Stopping now.\n");
    ::exit(EXIT_FAILURE);
  }

  for(size_t i = 0; i < gui->overlay_images.size(); ++i) {
    OverlayImage* o = gui->overlay_images[i];
    if(o == this && toggle) {
      gui->overlay_dx = i;
      gui->overlay_changed = true;
    }
  }

}

// ----------------------------------------------------------

CommandOverlayImage::CommandOverlayImage(StreamerGUI* gui)
  :OverlayImage(gui),lastDecayTime(0.0f)
{
}

void CommandOverlayImage::onPressed(bool& v) {
  printf("warning: CommandOverlayImage::onPressed not implemented");
}

void CommandOverlayImage::decay(float rate) {
  if (opacity == 0) { return; }
  float currentTime = ofGetElapsedTimef();
  float timeDelta = currentTime - lastDecayTime;
  float decayAmount = opacity * rate * timeDelta;
  opacity -= decayAmount;
  if ( opacity < 0.001f) {
    opacity = 0.0f;
  } 
  lastDecayTime = currentTime;
}

// ----------------------------------------------------------

StreamerGUI::StreamerGUI()
  :overlay_dx(0)
  ,overlay_changed(false)
  ,is_visible(false)
  ,is_sync_sender(false)
{

}

StreamerGUI::~StreamerGUI() {
  is_visible = false;
  is_sync_sender = false;

  for(size_t i = 0; i < sync.size(); ++i) {
    delete sync[i];
    sync[i] = NULL;
  }
  sync.clear();

  for(size_t i = 0; i < overlay_images.size(); ++i) {
    delete overlay_images[i];
    overlay_images[i] = NULL;
  }
  overlay_images.clear();

  for(size_t i = 0; i < command_overlay_images.size(); ++i) {
    delete command_overlay_images[i];
    command_overlay_images[i] = NULL;
  }
  command_overlay_images.clear();
}

bool StreamerGUI::setup(std::string settingsFile, bool isSender) {
  
  // effects panel
  fx_panel.setup("Effects", "ofxgui/effects.xml");
  fx_panel.add(fx_khronos_enabled.set("HYPER Khronos", false));
  fx_panel.add(fx_color_map_enabled.setup("SPECTRUM Color Map", false));
  fx_panel.add(fx_bad_tv_enabled.setup("WAVY Bad TV", false));
  fx_panel.add(fx_scanlines_enabled.setup("SCAN Scan Lines", false));
  fx_panel.add(fx_rgb_shift_enabled.setup("GLITCH RGB Shift", false));
  fx_panel.add(fx_flow_lines_enabled.setup("TRAIL Flow Lines", false));
  fx_panel.add(fx_grid_distort_enabled.setup("DISTORT Grid Distort", false));
  fx_panel.loadFromFile("ofxgui/effects.xml");

  // effect: khronos
  khronos_panel.setup("HYPER Khronos", "ofxgui/effect_khronos.xml");
  khronos_panel.add(khronos_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  khronos_panel.add(khronos_mag_scaler.setup("Mag Scaler", 1.0f, 1.0f, 10.0f));
  khronos_panel.loadFromFile("ofxgui/effect_khronos.xml");

  // effect: color map
  colmap_panel.setup("SPECTRUM Color Map", "ofxgui/effect_color_map.xml");
  colmap_panel.add(colmap_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  colmap_panel.loadFromFile("ofxgui/effect_color_map.xml");

  // effect: bad tv
  btv_panel.setup("WAVY Bad TV", "ofxgui/effect_bad_tv.xml");
  btv_panel.add(btv_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  btv_panel.add(btv_thick_distort.setup("Thick Distort", 0.0f, 0.1f, 20.0f));
  btv_panel.add(btv_fine_distort.setup("Fine Distort", 0.0f, 0.1f, 20.0f));
  btv_panel.add(btv_distort_speed.setup("Distort Speed", 0.0f, 0.0f, 1.0f));
  btv_panel.add(btv_roll_speed.setup("Roll Speed", 0.0f, 0.0f, 1.0f));
  btv_panel.loadFromFile("ofxgui/effect_bad_tv.xml");

  // effect: scan lines
  sl_panel.setup("SCAN Scan Lines", "ofxgui/effect_scanlines.xml");
  sl_panel.add(sl_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  sl_panel.add(sl_count.setup("Count", 50.0f, 50.0f, 1000.0f));
  sl_panel.add(sl_s_intensity.setup("Intensity: S", 0.0f, 0.0f, 2.0f));
  sl_panel.add(sl_n_intensity.setup("Intensity: N", 0.0f, 0.0f, 2.0f));
  sl_panel.loadFromFile("ofxgui/effect_scanlines.xml");

  // effect: rgb shift
  rgb_panel.setup("GLITCH RGB Shift", "ofxgui/effect_rgb_shift.xml");
  rgb_panel.add(rgb_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  rgb_panel.add(rgb_rgb_amount.setup("RGB Amount", 0.0f, 0.0f, 0.1f));
  rgb_panel.add(rgb_angle.setup("Angle", 0.0f, 0.0f, 2.0f));
  rgb_panel.loadFromFile("ofxgui/effect_rgb_shift.xml");

  // effect: flow lines
  flow_panel.setup("TRAIL Flow Lines", "ofxgui/effect_flow_lines.xml");
  flow_panel.add(flow_amount.setup("Amount", 0.0f, 0.0f, 1.0f));
  flow_panel.add(flow_step_size.setup("Step Size", 2.0f, 2.0f, 20.0f));
  flow_panel.add(flow_line_scale.setup("Line Scale", 1.0f, 1.0f, 20.0f));
  flow_panel.add(flow_threshold.setup("Threshold", 0.1f, 0.1f, 2.0f));
  flow_panel.loadFromFile("ofxgui/effect_flow_lines.xml");

  // effect: grid
  grid_panel.setup("DISTORT Grid Distort", "ofxgui/effect_grid.xml");
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
  web_panel.add(web_count_hash_tags.setup("Count Hash Tags", false));
  web_panel.add(web_decay_speed.setup("Decay Speed", 0.3f, 0.0001f, 1.0f));
  web_panel.add(web_command_impact.setup("Command Impact", 1.0f, 0.0001f, 1.0f));
  web_panel.loadFromFile("ofxgui/websystem.xml");

  /* overlay panel - images */
  setupOverlay();
    
  /* commnad overlay panel - images */
  setupCommadOverlay();

  /* crawl panel */
  crawl_panel.setup("Crawl", "ofxgui/crawl.xml");
  crawl_panel.add(crawl_enabled.setup("Enabled", false));
  crawl_panel.add(crawl_speed.setup("Speed", 1.0f, 1.0f, 20.0f));
  crawl_panel.loadFromFile("ofxgui/crawl.xml");

  /* text panel */
  text_panel.setup("Text overlay", "ofxgui/text.xml");
  text_panel.add(text_enabled.setup("Enabled", false));
  text_panel.add(text_add_text.setup("Set the text"));
  text_panel.add(text_scale.setup("Scale", 0.5f, 0.5f, 5.0f));
  text_panel.add(text_spacing.setup("Spacing", 0.0f, 0.0f, 100.0f));
  text_panel.add(text_opacity.setup("Opacity", 1.0f, 0.0f, 1.0f));
  text_panel.loadFromFile("ofxgui/text.xml");

  text_add_text.addListener(this, &StreamerGUI::onSetOverlayText);

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
  int over_h = overlay_panel.getHeight();
  int crawl_h = crawl_panel.getHeight();

  web_panel.setPosition(10, 10);
  overlay_panel.setPosition(10, web_h + 20);
  crawl_panel.setPosition(10, web_h + over_h + 30);
  text_panel.setPosition(10, web_h + over_h + crawl_h + 40);

  fx_panel.setPosition(web_w + 20, 10);
  khronos_panel.setPosition(web_w + 20, fx_h + 20);
  colmap_panel.setPosition(web_w + 20, fx_h + kr_h + 30);
  btv_panel.setPosition(web_w + 20, fx_h + kr_h + col_h + 40);
  sl_panel.setPosition(web_w + 20,  fx_h + kr_h + col_h + btv_h + 50);
  rgb_panel.setPosition(web_w + 20, fx_h + kr_h + col_h + btv_h + sl_h + 60);
  flow_panel.setPosition(web_w + 20,  fx_h + rgb_h + btv_h + kr_h + col_h + sl_h + 70);
  grid_panel.setPosition(web_w + fx_panel.getWidth() + 30, fx_panel.getHeight() + 20);

  // sync
  is_sync_sender = isSender;
  setupSync(settingsFile, is_sync_sender);
  return true;
}

void StreamerGUI::setupSync(std::string settingsFile, bool isSender) {

  ofxXmlSettings xml;
  if(!xml.loadFile(settingsFile)) {
    printf("error: cannot load the settings file for the streamer gui. Stopping now.\n");
    ::exit(EXIT_FAILURE);
  }

  std::string receiver_ip = xml.getValue("settings:receiver_ip", "");
  if(!receiver_ip.size()) {
    printf("error: cannot find the <receiver_ip> in the settings xml. Stopping now.\n");
    ::exit(EXIT_FAILURE);
  }

  // setup the panel sync
  std::vector<ofxPanel*> to_sync;
  to_sync.push_back(&fx_panel);
  to_sync.push_back(&khronos_panel);
  to_sync.push_back(&colmap_panel);
  to_sync.push_back(&btv_panel);
  to_sync.push_back(&sl_panel);
  to_sync.push_back(&flow_panel);
  to_sync.push_back(&rgb_panel);
  to_sync.push_back(&grid_panel);
  to_sync.push_back(&web_panel);
  to_sync.push_back(&overlay_panel);
  to_sync.push_back(&crawl_panel);
  to_sync.push_back(&text_panel);
  
  int from_port = 6667;
  int to_port = 6666;

  if(!isSender) {
    from_port = 6666;
    to_port = 6667;
  }

  for(size_t i = 0; i < to_sync.size(); ++i) {
    ofxOscParameterSync* s = new ofxOscParameterSync();
    s->setup((ofParameterGroup&)to_sync[i]->getParameter(), to_port, receiver_ip, from_port);
    to_port += 2;
    from_port += 2;
    sync.push_back(s);
  }

  // setup text sync
  if(isSender) {
    osc_sender.setup(receiver_ip, 4455);
  }
  else {
    osc_receiver.setup(4455);
  }
}

void StreamerGUI::setupOverlay() {

  overlay_panel.setup("Overlay images", "ofxgui/overay_images.xml");
  overlay_panel.add(overlay_image_enabled.setup("Image Enabled", false));
  overlay_panel.add(overlay_image_opacity.setup("Image Opacity", 0.0f, 0.0f, 1.0f));

  ofDirectory dir( ofToDataPath("./overlays/") );
  dir.allowExt("png");
  dir.listDir();

  vector<ofFile> files = dir.getFiles();
  if(!files.size()) {
    printf("error: not overlay images found. create a `overlays` directory in your data dir and put the overlay images in there. Stopping now.\n");
    ::exit(EXIT_FAILURE);
  }
  
  ofColor file_color(255,33,33);

  for(vector<ofFile>::iterator it = files.begin(); it != files.end(); ++it) {
    ofFile& f = *it;
    OverlayImage* oi = new OverlayImage(this);
    std::string name = f.getFileName();
    overlay_panel.add(oi->toggle.setup(name, false));
    oi->file = f;
    oi->toggle.addListener(oi, &OverlayImage::onPressed);
    oi->toggle.setFillColor(file_color);
    overlay_images.push_back(oi);
  }

  overlay_panel.loadFromFile("ofxgui/overlay_images.xml");

}

void StreamerGUI::setupCommadOverlay() {

  command_overlay_panel.setup("Command overlay images", "ofxgui/command_overlay_images.xml");

  ofDirectory dir( ofToDataPath("./command_overlays/") );
  dir.allowExt("png");
  dir.listDir();

  vector<ofFile> files = dir.getFiles();
  if(!files.size()) {
    printf("error: not overlay images found. create a `command_overlays` directory in your data dir and put the overlay images in there. Stopping now.\n");
    ::exit(EXIT_FAILURE);
  }
  
  ofColor file_color(255,33,33);

  for(vector<ofFile>::iterator it = files.begin(); it != files.end(); ++it) {
    ofFile& f = *it;
    CommandOverlayImage* oi = new CommandOverlayImage(this);
    std::string name = f.getFileName();
    command_overlay_panel.add(oi->toggle.setup(name, false));
    oi->file = f;
    oi->toggle.addListener(oi, &CommandOverlayImage::onPressed);
    oi->toggle.setFillColor(file_color);
    oi->commandName = f.getBaseName();
    command_overlay_images.push_back(oi); 
  }

  command_overlay_panel.loadFromFile("ofxgui/overlay_images.xml");

}

void StreamerGUI::update() {

  for(size_t i = 0; i < sync.size(); ++i) {
    sync[i]->update();
  }

  // get the osc messages from the remote gui
  if(!is_sync_sender) {
    while(osc_receiver.hasWaitingMessages()){
      ofxOscMessage m;
      osc_receiver.getNextMessage(&m);
      std::string address = m.getAddress();

      if(address == "/overlay/text") {
        overlay_text = m.getArgAsString(0);
      }
    }
  }

  // when overlay has changed we unset the other toggles
  if(overlay_changed) {
    for(size_t i = 0; i < overlay_images.size(); ++i) {
      if(i != overlay_dx) {
        overlay_images[i]->toggle = false;
      }
    }
  }
}

// @todo - pass in a controller key (if available) else text inputs fail
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

  if(!is_visible) {
    return  ;
  }

  fx_panel.draw();
  crawl_panel.draw();
  overlay_panel.draw();
  web_panel.draw();
  text_panel.draw();

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

void StreamerGUI::save() {
  fx_panel.saveToFile("ofxgui/effects.xml");
  khronos_panel.saveToFile("ofxgui/effect_khronos.xml");
  colmap_panel.saveToFile("ofxgui/effect_color_map.xml");
  btv_panel.saveToFile("ofxgui/effect_bad_tv.xml");
  sl_panel.saveToFile("ofxgui/effect_scanlines.xml");
  rgb_panel.saveToFile("ofxgui/effect_rgb_shift.xml");
  flow_panel.saveToFile("ofxgui/effect_flow_lines.xml");
  grid_panel.saveToFile("ofxgui/effect_grid.xml");
  web_panel.saveToFile("ofxgui/websystem.xml");
  crawl_panel.saveToFile("ofxgui/crawl.xml");
  text_panel.saveToFile("ofxgui/text.xml");
}

void StreamerGUI::load() {
  fx_panel.loadFromFile("ofxgui/effects.xml");
  khronos_panel.loadFromFile("ofxgui/effect_khronos.xml");
  colmap_panel.loadFromFile("ofxgui/effect_color_map.xml");
  btv_panel.loadFromFile("ofxgui/effect_bad_tv.xml");
  sl_panel.loadFromFile("ofxgui/effect_scanlines.xml");
  rgb_panel.loadFromFile("ofxgui/effect_rgb_shift.xml");
  flow_panel.loadFromFile("ofxgui/effect_flow_lines.xml");
  grid_panel.loadFromFile("ofxgui/effect_grid.xml");
  web_panel.loadFromFile("ofxgui/websystem.xml");
  crawl_panel.loadFromFile("ofxgui/crawl.xml");
  text_panel.loadFromFile("ofxgui/text.xml");
}

void StreamerGUI::onSetOverlayText() {
  if(is_sync_sender) { 
    overlay_text = ofSystemTextBoxDialog("Please enter the text for overlay 1 (comma creates new lines)");

    ofxOscMessage m;
    m.setAddress("/overlay/text");
    m.addStringArg(overlay_text);
    osc_sender.sendMessage(m);

  }
}
