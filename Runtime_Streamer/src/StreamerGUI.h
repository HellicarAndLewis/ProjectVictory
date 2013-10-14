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

/*
void streamer_gui_thread(void* user);
struct GUITask {
};
*/

class StreamerGUI {
 public:
  StreamerGUI();
  ~StreamerGUI();
  bool setup();
  void update();
  void draw();
  bool onKeyPressed(int key); /* used to toggle the effect on or off (1-...), returns true when it's handled */
 public:
  void onClickedTopicA();
  void onClickedTopicB();
  void lock(); /* lock mutex */
  void unlock(); /* unlock mutex */
                  

 public:

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

  /* voting */
  ofxPanel vote_panel;
  ofxLabel vote_topic_a;
  ofxLabel vote_topic_b;
  ofxButton vote_button_a;
  ofxButton vote_button_b;

  /* text is received from another app because it stops the event loop */
  ofxOscReceiver receiver;

  /* we dispatch a thread in which we show the input popup, else it blocks .. */
  /*
  std::vector<GUITask*> work;
  uv_thread_t thread;
  uv_mutex_t mutex;
  uv_cond_t cv;
  */
};

/*
inline void StreamerGUI::lock() {
  uv_mutex_lock(&mutex);
}

inline void StreamerGUI::unlock() {
  uv_mutex_unlock(&mutex);
}
*/
#endif
