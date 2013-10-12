#ifndef ROXLU_WEBSYSTEM_SCREENGRAB_SAVER_H
#define ROXLU_WEBSYSTEM_SCREENGRAB_SAVER_H

extern "C" {
#  include <uv.h>  
}

#include <stdint.h>
#include <string>
#include <vector>tes

#define GD_TYPE_NONE 0 /* default state */
#define GD_TYPE_STOP 1 /* stop working! quits the thread */
#define GD_TYPE_SAVE 2 /* save grab data to file */

struct GrabData {
  GrabData();

  unsigned char* pixels;
  bool is_free;
  int type;
};

void screengrab_thread(void* user);

class ScreenGrabSaver {
 public:
  ScreenGrabSaver();
  ~ScreenGrabSaver();
  bool setup(int w, int h);
  void update();
  GrabData* getFreeGrabData(); /* returns an entry from the "buffer" member which is free to be used (not being used in the thread) */
  bool grab(std::string filename);
 public:
  int win_w;
  int win_h;
  uv_mutex_t mutex;
  uv_thread_t thread;
  uv_cond_t cv;
  std::vector<GrabData*> buffer;
};


inline GrabData* ScreenGrabSaver::getFreeGrabData() {

  GrabData* found = NULL;

  uv_mutex_lock(&mutex);
  {
    for(std::vector<GrabData*>::iterator it = buffer.begin(); it != buffer.end(); ++it) {
       GrabData* gd = *it;
       if(gd->is_free) {
         found = gd;
         break;
       }
    }
  }
  uv_mutex_unlock(&mutex);

  return found;
}
#endif
