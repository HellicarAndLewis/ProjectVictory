#include "ofMain.h"
#include "ScreenGrabSaver.h"

// -----------------------------------------

void screengrab_thread(void* user) {
  
  ScreenGrabSaver* gsp = static_cast<ScreenGrabSaver*>(user);
  ScreenGrabSaver& gs = *gsp;
  std::vector<GrabData*> todo;

  while(true) {

    // wait for new work.
    uv_mutex_lock(&gs.mutex);
    {
      while(gs.work.size() == 0) {
        uv_cond_wait(&gs.cv, &gs.mutex);
      }
      std::copy(gs.work.begin(), gs.work.end(), std::back_inserter(todo));
      gs.work.clear();
    }
    uv_mutex_unlock(&gs.mutex);
    
    // process the work.
    bool must_stop = false;
    for(std::vector<GrabData*>::iterator it = todo.begin(); it != todo.end(); ++it) {
      GrabData* gd = *it;
      if(gd->type == GD_TYPE_SAVE) {
        gd->img.mirror(true, false);
        gd->img.saveImage("screenshots/" +gd->name +".jpg");

        uv_mutex_lock(&gs.mutex);
        gd->is_free = true;
        uv_mutex_unlock(&gs.mutex);
      }
      else if(gd->type == GD_TYPE_STOP) {
        must_stop = true;
        break;
      }
    }

    todo.clear();

    if(must_stop) {
      break;
    }
  }

  gsp = NULL;

}

// -----------------------------------------

GrabData::GrabData() 
  :is_free(true)
  ,type(GD_TYPE_NONE)
{
  img.setUseTexture(false);
}

// -----------------------------------------

ScreenGrabSaver::ScreenGrabSaver() 
  :win_w(0)
  ,win_h(0)
  ,state(SGS_STATE_NONE)
{
  uv_mutex_init(&mutex);
  uv_cond_init(&cv);
}

ScreenGrabSaver::~ScreenGrabSaver() {

  // send a stop signal.
  {
    GrabData* gd = new GrabData();
    gd->type = GD_TYPE_STOP;
    
    uv_mutex_lock(&mutex);
      work.push_back(gd);
      uv_cond_signal(&cv);
    uv_mutex_unlock(&mutex);
  }

  // wait for it ...
  uv_thread_join(&thread);

  // clean up
  uv_mutex_destroy(&mutex);
  uv_cond_destroy(&cv);

  for(std::vector<GrabData*>::iterator it = buffer.begin(); it != buffer.end(); ++it) {

    GrabData* gd = *it;
    gd->type = GD_TYPE_NONE;
    gd->is_free = false;

    delete gd;
    gd = NULL;
  }

  buffer.clear();
  win_w = 0;
  win_h = 0;
  state = SGS_STATE_NONE;
}

bool ScreenGrabSaver::setup(int winW, int winH, int bufferSize) {

  if(state != SGS_STATE_NONE) {
    printf("error: you can only setup the grab saver once ...\n");
    return false;
  }

  if(!winW || !winH) {
    printf("error: invalid width and/or height: %d x %d\n", winW, winH);
    return false;
  }

  win_w = winW;
  win_h = winH;

  nbytes = win_w * win_h * 3; 

  // preallocate the grab buffers
  uv_mutex_lock(&mutex);
  {
    for(int i = 0; i < bufferSize; ++i) {

      GrabData* gd = new GrabData();
      gd->img.allocate(win_w, win_h, OF_IMAGE_COLOR);
      gd->type = GD_TYPE_SAVE;
      gd->is_free = true;
      buffer.push_back(gd);
    }
  }
  uv_mutex_unlock(&mutex);
  
  uv_thread_create(&thread, screengrab_thread, this);

  state = SGS_STATE_RUNNING;

  return true;
}

bool ScreenGrabSaver::grab(std::string filename) {

#if !defined(NDEBUG)

  if(!win_h || !win_w) {
    printf("error: cannot grab because the win_w and/or win_h members are invalid: %d x %d\n", win_w, win_h);
    return false;
  }

  if(!filename.size()) {
    printf("error: inavlid filename; cannot grab.\n");
    return false;
  }

  if(state != SGS_STATE_RUNNING) {
    printf("error: cannot grab because we are not running atm...\n");
    return false;
  }

#endif

  GrabData* gd = getFreeGrabData();
  if(!gd) {
    printf("error: cannot get a free grab data item. we must allocate more frames at the start!\n");
    return false;
  }

  printf("found some data we can use!\n");

  // download from gpu takes about 5.7ms
  uv_mutex_lock(&mutex);
  {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glReadBuffer(GL_BACK);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadPixels(0, 0, win_w, win_h, GL_RGB, GL_UNSIGNED_BYTE, gd->img.getPixels());

    gd->name = filename;
    gd->is_free = false;

    work.push_back(gd);
    uv_cond_signal(&cv);
  }
  uv_mutex_unlock(&mutex);

  return true;
}
