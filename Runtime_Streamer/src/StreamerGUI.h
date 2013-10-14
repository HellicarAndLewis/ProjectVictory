#ifndef ROXLU_STREAMER_GUI_H
#define ROXLU_STREAMER_GUI_H

/*

  # StreamerGIU

  One class to rule them all....

  It took me quite some time to change all the gui settings
  with the previous ofxUI system because the gui was all scattered.
  Therefore I'm using one class that controls all the parameters
  of the complete system. 

 */

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOSC.h" /* we're using OSC to receive text input because it was stopping the main event loop .. arg... */

extern "C" {
#  include <uv.h>
}

// -----------------------------------------------------

void streamer_gui_cleanup_name(std::string& fix); /* ofxGui has a bug with names that have certain characters ...*/

class StreamerGUI;

class OverlayImage {
 public:
  OverlayImage(StreamerGUI* gui);
  void onPressed(bool& v);
  
  StreamerGUI* gui;
  ofFile file;
  ofxToggle toggle;
};

// -----------------------------------------------------

class StreamerGUI {
 public:
  StreamerGUI();
  ~StreamerGUI();
  bool setup();
  void update();
  void draw();
  void save(); /* saves the current state */
  void load(); /* loads the last saved state */
  void show(); /* show the gui */
  void hide(); /* hide the gui */
  void toggle(); /* toggle visibility */
  bool onKeyPressed(int key); /* used to toggle the effect on or off (1-...), returns true when it's handled */
  bool didOverlayImageChange(); /* returns true when the overlay image should be drawn/updated - call resetOverlayImageChanged() after handling this */
  void resetOverlayImageChanged(); /* after  you've checked which overlay image should be should you need to call this */
 public:
  void setupOverlay(); /* sets up the overlay gui */

 public:
  /* general stuff */
  bool is_visible;

  /* fx panel */
  ofxPanel fx_panel;
  ofxToggle fx_khronos_enabled;
  ofxToggle fx_color_map_enabled;
  ofxToggle fx_bad_tv_enabled;
  ofxToggle fx_scanlines_enabled;
  ofxToggle fx_rgb_shift_enabled;
  ofxToggle fx_flow_lines_enabled;
  ofxToggle fx_grid_distort_enabled;

  /* fx: khronos */
  ofxPanel khronos_panel;
  ofxFloatSlider khronos_amount;
  ofxFloatSlider khronos_mag_scaler;

  /* fx: color map */
  ofxPanel colmap_panel;
  ofxFloatSlider colmap_amount;

  /* fx: bad tv */
  ofxPanel btv_panel;
  ofxFloatSlider btv_amount;
  ofxFloatSlider btv_thick_distort;
  ofxFloatSlider btv_fine_distort;
  ofxFloatSlider btv_distort_speed;
  ofxFloatSlider btv_roll_speed;

  /* fx: scan lines */
  ofxPanel sl_panel;
  ofxFloatSlider sl_amount;
  ofxFloatSlider sl_count;
  ofxFloatSlider sl_s_intensity;
  ofxFloatSlider sl_n_intensity;

  /* fx: flow lines */
  ofxPanel flow_panel;
  ofxFloatSlider flow_amount;
  ofxFloatSlider flow_step_size;
  ofxFloatSlider flow_line_scale;
  ofxFloatSlider flow_threshold;

  /* fx: rgb shift fx */
  ofxPanel rgb_panel;
  ofxFloatSlider rgb_amount;
  ofxFloatSlider rgb_rgb_amount;
  ofxFloatSlider rgb_angle;

  /* fx: grid */
  ofxPanel grid_panel;
  ofxFloatSlider grid_amount;
  ofxFloatSlider grid_step_size;
  ofxFloatSlider grid_particle_radius;
  ofxFloatSlider grid_force_scale;
  ofxFloatSlider grid_spring_strength;
  ofxFloatSlider grid_drag;
  ofxToggle grid_show_grid;

  /* web panel */
  ofxPanel web_panel;
  ofxToggle web_is_enabled;
  ofxToggle web_commands_enabled;
  ofxToggle web_shoutouts_enabled;
  ofxToggle web_decay_effects;
  ofxToggle web_count_hash_tags;

  /* overlay panel */
  ofxPanel overlay_panel;
  ofxToggle overlay_image_enabled;
  ofxFloatSlider overlay_image_opacity;
  std::vector<OverlayImage*> overlay_images;
  size_t overlay_dx; 
  bool overlay_changed;

  /* crawl panel */
  ofxPanel crawl_panel;
  ofxToggle crawl_enabled;
  ofxFloatSlider crawl_speed;

  /* text overlay panel */
  ofxPanel text_panel;
  ofxToggle text_enabled;
  ofxFloatSlider text_scale;
  ofxFloatSlider text_spacing;
  ofxOscReceiver receiver;   /* text is received from another app because it stops the event loop */
  std::string overlay_text;
};

inline bool StreamerGUI::didOverlayImageChange() {
  return overlay_changed;
}

inline void StreamerGUI::resetOverlayImageChanged() {
  overlay_changed = false;
}

inline void StreamerGUI::show() {
  is_visible = true;
}

inline void StreamerGUI::hide() {
  is_visible = false;
}

inline void StreamerGUI::toggle() {
  is_visible = !is_visible;
}

#endif
