#include <uv.h>
#include "VideoFX.h"


int VideoFX::count = 0;

void VideoFX::init() {

  videoSource = NULL;    

  effects.push_back( &khronosEffect );
  effects.push_back( &colorMapEffect );
  effects.push_back( &badTVEffect );
  effects.push_back( &scanlinesEffect );
  effects.push_back( &rgbShiftEffect );
  effects.push_back( &flowLinesEffect );
  effects.push_back( &gridDistortEffect );
    
  ramp.loadImage( "textures/ramp1.png" );
  colorMapEffect.setRamp( ramp.getTextureReference().texData );
    
  num = ++count;
  opticalFlowEnabled = true;
    
  // SETUP GUI FOR ALL EFFECTS
  for ( int i=0; i<effects.size(); i++ ) {
    effects[i]->farneback = &farneback;
  }

}

bool VideoFX::setVideoSource( ofBaseImage *source ) {

  if(!source) {
    printf("error: invalid source in VideoFX.\n");
    return false;
  }

  static bool is_allocated = false;

  int w = source->getWidth();
  int h = source->getHeight();

  if(!w || !h || w < 0 || h < 0) {
    printf("error: invalid width / height of the base image: %d x %d.\n", w, h);
    return false;
  }
    
  videoSource = source;

  // allocate the circular texture
  circularTexture.allocate( w, h, 20 );

  // allocate a downsampled texture to run through optical flow

  //downsampledFrame.allocate( w/4, h/4, OF_IMAGE_COLOR );
  downsampledFrame.allocate(160, 120, OF_IMAGE_COLOR );
  //downsampledFrame.allocate(320, 240, OF_IMAGE_COLOR );
  //downsampledFrame.allocate(320, 140, OF_IMAGE_COLOR );
  //downsampledFrame.allocate(240, 140, OF_IMAGE_COLOR );
  //downsampledFrame.allocate(240, 140, OF_IMAGE_COLOR );
      
  // allocate the effects
  for ( int i=0; i<effects.size(); i++ ) {
    if(!effects[i]->compiled()) {
      effects[i]->allocate( w, h );
    }
  }

#if USE_FLOW
  flow.setup(downsampledFrame.getWidth(), downsampledFrame.getHeight(), 
             0.5, 3, 10, 1, 7, 1.5, false, false);
#else

  // @todo - roxlu added this as mega quickfix - test
  farneback.setNumLevels(3);
  farneback.setWindowSize(10);
  farneback.setNumIterations(1);
  farneback.setPolyN(7);
  farneback.setPolySigma(1.5);
  farneback.setUseGaussian(false);
  farneback.setPyramidScale(0.5f);
#endif

  return true;
}

void VideoFX::update( bool isFrameNew ) {
    
  if(!videoSource) {
    return;
  }

  // if the video source has a new frame, run it through optical flow and add it to the circularTexture

  if(isFrameNew) { 
    
#if !defined(NDEBUG)
    int w = videoSource->getWidth();
    int h = videoSource->getHeight();
    if(w < 0 || w > 1024 * 2 || h < 0 || h > 1024 * 2) {
      printf("error: video srouce has an invalid size.\n");
      ::exit(0);
    }
#endif  

    // resample for optical flow
    if(opticalFlowEnabled) {

      ofxCv::resize(*videoSource, downsampledFrame);
      downsampledFrame.update();
  
#if USE_FLOW
      flow.update(downsampledFrame);
      farneback.flow = flow.flow;
#else 
      farneback.calcOpticalFlow(downsampledFrame);
      farneback.updateVectorFieldTexture();
#endif

    }

    // pack into circular texture
    if(khronosEffect.enabled){
      unsigned char * pixels = videoSource->getPixels();
      circularTexture.addData( pixels, videoSource->getWidth(), videoSource->getHeight() );
    }

  }

  deque<BaseEffect*> enabledEffects;
  for(int i = 0; i < effects.size(); i++){
    if(effects[i]->enabled && effects[i] != &khronosEffect){
      enabledEffects.push_back(effects[i]);
    }
  }

  // roxlu: commented this sort
  //sort(enabledEffects.begin(), enabledEffects.end(), CompareBaseEffects);

  if (khronosEffect.enabled) {
    enabledEffects.push_front( &khronosEffect );
  }
    
  finalEffect = 0;
  ofTextureData *texData = 0;

  for (int i = 0; i < enabledEffects.size(); i++){
        
    BaseEffect *effect = enabledEffects[i];
    effect->setVectorField(farneback.vf.getTextureReference().texData);

    if(texData == 0){

      // special circumstances for khronos effect
      if(effect == &khronosEffect) {
        khronosEffect.setDepthOffset(circularTexture.offset / (float)circularTexture.depth);
        khronosEffect.setVideo3D(circularTexture.texData);
      }
      else {
        effect->setDiffuseTexture(videoSource->getTextureReference().texData);
      }
    }
    else {
      effect->setDiffuseTexture(*texData);
    }
            
    updateEffect(effect);
    texData = &effect->getTextureReference().texData;
  }
}

void VideoFX::updateEffect( BaseEffect *effect ) {
  effect->update();
  finalEffect = effect;
}

void VideoFX::draw( float posX, float posY, float width, float height ) {

#if !defined(NDEBUG)
  if(!videoSource) {
    printf("error: videoSource is null in VideoFX.\n");
    return;
  }
#endif  

  if ( finalEffect != 0 ) {
    finalEffect->getTextureReference().draw( posX, posY, width, height );
  }
  else {
    videoSource->draw( posX, posY, width, height );
  }

#if USE_FLOW
  //  flow.drawColored(ofGetWidth(), ofGetHeight(), 10, 3);
#endif
    
}

void VideoFX::reloadShaders() {
  for ( int i=0; i<effects.size(); i++ ) {
    if ( effects[i]->getPathToShader() != "" )
      effects[i]->reloadShader();
  }
}

void VideoFX::exit() {
}

void VideoFX::setOpticalFlowEnabled( bool enabled ) {
  opticalFlowEnabled = enabled;
}

bool CompareBaseEffects( BaseEffect * a, BaseEffect * b ) {
  return a->settings->getRect()->getX() < b->settings->getRect()->getX();
}
