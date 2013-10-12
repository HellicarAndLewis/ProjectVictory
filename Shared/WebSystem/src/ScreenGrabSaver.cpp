#include "ScreenGrabSaver.h"


void screengrab_thread(void* user) {
  
  while(true) {
    printf("grab thread ------------------------------------------\n");
    sleep(3);
  }
}

// -----------------------------------------

GrabData::GrabData() 
  :pixels(NULL)
  ,is_free(true)
  ,type(GD_TYPE_NONE)
{
}

// -----------------------------------------

ScreenGrabSaver::ScreenGrabSaver() 
  :win_w(0)
  ,win_h(0)
{
  uv_mutex_init(&mutex);
  uv_cond_init(&cv);
}

ScreenGrabSaver::~ScreenGrabSaver() {
}

bool ScreenGrabSaver::setup(int winW, int winH) {

  if(!winW || !winH) {
    printf("error: invalid width and/or height: %d x %d\n", winW, winH);
    return false;
  }

  win_w = winW;
  win_h = winH;
  
  uv_thread_create(&thread, screengrab_thread, this);

  return true;
}

bool ScreenGrabSaver::grab(std::string filename) {

#if !defined(NDEBUG)
  if(!filename.size()) {
    printf("error: inavlid filename; cannot grab.\n");
    return false;
  }
#endif

  GrabData* gd = getFreeGrabData();
  if(!gd) {
    printf("error: cannot get a free grab data item. we must allocate more frames at the start!\n");
    return false;
  }

  

}
