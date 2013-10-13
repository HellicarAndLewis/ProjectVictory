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

#define SGS_STATE_NONE 0 /* default state of ScreenGrabsaver */
#define SGS_STATE_RUNNING 1 /* thread is running */

// -----------------------------------------
struct GrabData {
  GrabData();
  ofImage img;
  bool is_free;
  int type;
  std::string name;
};

// -----------------------------------------

void screengrab_thread(void* user);

// -----------------------------------------

class ScreenGrabSaver {
 public:
  ScreenGrabSaver();
  ~ScreenGrabSaver();
  bool setup(int w, int h, int bufferSize); /* bufferSize is the number of preallocated frames/buffers, one for each image */
  GrabData* getFreeGrabData();              /* returns an entry from the "buffer" member which is free to be used (not being used in the thread) */
  bool grab(std::string filename);          /* will grab the data in the caller thread, and saves the data to file in a separate thread */
 public:
  int state;            /* state of the screen grab saver */
  size_t nbytes;        /* number of bytes per frame */
  int win_w;            /* width of the images */
  int win_h;            /* height of the images */
  uv_mutex_t mutex;     /* mutex to protect buffer */
  uv_thread_t thread;   /* thread handle that saves the files */
  uv_cond_t cv;         /* cond var. signals thread */
  std::vector<GrabData*> buffer; /* all available (preallocated) buffers */
  std::vector<GrabData*> work;   /* the work that must be handled by the thread */
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
