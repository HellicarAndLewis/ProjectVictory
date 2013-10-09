#include <assert.h>
#include <streamer/core/VideoEncoder.h>
#include <streamer/core/Debug.h>
// @todo remove the USE_GRAPH and Graph include
#if defined(USE_GRAPH)
#  include <streamer/utils/Graph.h>
#endif

VideoEncoder::VideoEncoder() 
  :encoder(NULL)
  ,vflip(true)
  ,frame_num(0)
{
}

VideoEncoder::~VideoEncoder() {

  shutdown();

  printf("error: need to x264_encoder_close etc. \n");
}

// @todo check if width / height are valid (multiples of 16 I believe)
bool VideoEncoder::setup(VideoSettings s) {
  assert(s.width > 0);
  assert(s.height > 0);
  assert(s.fps > 0);

  settings = s;

  return true;
}

bool VideoEncoder::initialize() {
  assert(settings.width);
  assert(settings.height);

  frame_num = 0;

  if(!initializeX264()) {
    return false;
  }

  if(!initializePic()) {
    return false;
  }

  return true;
}

// See https://gist.github.com/roxlu/0f61a499df75e64b764d for an older version of this, with some rate control tests
bool VideoEncoder::initializeX264() {
  assert(settings.width > 0);
  assert(settings.height > 0);
  assert(settings.fps > 0);

  int r = 0;
  x264_param_t* p = &params;
  
  r = x264_param_default_preset(p, "faster", "zerolatency");
  if(r != 0) {
    printf("error: cannot set the default preset on x264.\n");
    return false;
  }

#if !defined(NDEBUG)
  p->i_log_level = X264_LOG_DEBUG;
#endif   

  p->i_threads = 1;
  p->i_width = settings.width;
  p->i_height = settings.height;
  p->i_fps_num = settings.fps;
  p->i_fps_den = 1;
  p->b_annexb = 0; // flv == no annexb, but strangely, when I disable it the generated flv cannot be played back, for raw h264 you'll need to set annexb to 1 when you want to play it in vlc (vlc isn't properly playing back flv)

  p->rc.i_rc_method = X264_RC_ABR;  // when you're limited to bandwidth you set the vbv_buffer_size and vbv_max_bitrate using the X264_RC_ABR rate control method. The vbv_buffer_size is a decoder option and tells the decoder how much data must be buffered before playback can start. When vbv_max_bitrate == vbv_buffer_size, then it will take one second before the playback might start. when vbv_buffer_size == vbv_max_bitrate * 0.5, it might start in 0.5 sec. 
  p->rc.i_bitrate = settings.bitrate;
  p->rc.i_vbv_buffer_size = p->rc.i_bitrate * 4; 
  p->rc.i_vbv_max_bitrate = p->rc.i_bitrate;

#if 0
  r = x264_param_apply_profile(p, "main");
  if(r != 0) {
    printf("error: cannot set the baseline profile on x264.\n");
    return false;
  }
#endif

  encoder = x264_encoder_open(p);
  if(!encoder) {
    printf("error: cannot create the encoder.\n");
    return false;
  }

  x264_encoder_parameters(encoder, &params);  

  print_x264_params(p);

  return true;
}

bool VideoEncoder::initializePic() {

  unsigned int csp = (vflip) ? X264_CSP_I420 | X264_CSP_VFLIP : X264_CSP_I420;
  x264_picture_init(&pic_in);
  pic_in.img.i_csp = csp;
  pic_in.img.i_plane = 3;

  return true;
}

bool VideoEncoder::encodePacket(AVPacket* p, FLVTag& tag) {
  assert(p);
  assert(encoder);

  size_t nbytes_y = settings.width * settings.height;
  size_t nbytes_uv = nbytes_y / 4;

  pic_in.img.i_stride[0] = p->strides[0];
  pic_in.img.i_stride[1] = p->strides[1];
  pic_in.img.i_stride[2] = p->strides[2];

  pic_in.img.plane[0] = p->planes[0];
  pic_in.img.plane[1] = p->planes[1];
  pic_in.img.plane[2] = p->planes[2];

  pic_in.i_pts = frame_num; 

  frame_num++;

  x264_nal_t* nal = NULL;
  int nals_count = 0;

#if defined(USE_GRAPH)
  uint64_t enc_start = uv_hrtime() / 1000000;
#endif

  int frame_size = x264_encoder_encode(encoder, &nal, &nals_count, &pic_in, &pic_out);

#if defined(USE_GRAPH)
  frames_graph["enc_video"] += ((uv_hrtime()/1000000) - enc_start);
  frames_graph["enc_audio_video"] += ((uv_hrtime()/1000000) - enc_start);
#endif

  if(frame_size < 0) {
    printf("error: x264_encoder_encode failed.\n");
    return false;
  }

  if(!nal) {
    //printf("error: x264_encoder_encode returned no valid nals. nals_count = %d\n", nals_count);
    return false;
  }

  tag.makeVideoTag();

#define ENCODE_PACKET_USE_PTR 0
#define ENCODE_PACKET_USE_COPY 0
#define ENCODE_PACKE_USE_COPY_ALL 1

#if ENCODE_PACKE_USE_COPY_ALL 
  tag.bs.clear();
  for(int i = 0; i < nals_count; ++i) {
    tag.bs.putBytes(nal[i].p_payload, nal[i].i_payload);
#if USE_GRAPH    
    network_graph["h264"] += nal[i].i_payload;
#endif
  }
  tag.setData(tag.bs.getPtr(), tag.bs.size());
#endif

#if ENCODE_PACKET_USE_COPY
  tag.bs.clear();
  tag.bs.putBytes(nal[0].p_payload, frame_size);
  tag.setData(tag.bs.getPtr(), tag.bs.size());
# if USE_GRAPH
  network_graph["h264"] += frame_size;
# endif  
#endif

#if ENCODE_PACKET_USE_PTR
  tag.setData(nal[0].p_payload, frame_size);
#endif

  float tb = (1.0f/settings.fps) * 1000;
  int64_t cts = (pic_out.i_pts < 0) ? 0 : pic_out.i_pts * tb;
  int64_t dts = (pic_out.i_dts < 0) ? 0 : pic_out.i_dts * tb;
  int64_t offset = cts - dts;

  tag.setTimeStamp(p->timestamp);
  tag.setFrameType(pic_out.b_keyframe ? FLV_VIDEOFRAME_KEY_FRAME : FLV_VIDEOFRAME_INTER_FRAME);
  tag.setCompositionTime(offset);

  return true;
}

bool VideoEncoder::createDecoderConfigurationRecord(AVCDecoderConfigurationRecord& rec) {
  assert(encoder);

  int num_nals = 0;
  x264_nal_t* nals = NULL;

  x264_encoder_headers(encoder, &nals, &num_nals);

  if(!nals) {
    printf("error: cannot get encoder headers from x264.\n");
    return false;
  }

  if(num_nals != 3) {
    printf("warning: we expect number of nals from x264_encoder_headers to be 3.\n");
    return false;
  }

  int sps_size = nals[0].i_payload;
  int pps_size = nals[1].i_payload;
  int sei_size = nals[2].i_payload;
  uint8_t* sps = nals[0].p_payload + 4;
  uint8_t* pps = nals[1].p_payload + 4;
  uint8_t* sei = nals[2].p_payload + 4;

  rec.configuration_version = 1;
  rec.avc_profile_indication = sps[1];
  rec.profile_compatibility = sps[2];
  rec.avc_level_indication = sps[3];

  std::copy(sps, sps+(sps_size-4), std::back_inserter(rec.sps));
  std::copy(pps, pps+(pps_size-4), std::back_inserter(rec.pps));
  std::copy(sei, sei+(sei_size-4), std::back_inserter(rec.sei));

  printf(">> nals[0].i_payload: %d, sps_size: %d, profile: %d\n", nals[0].i_payload, sps_size, sps[1]);
  printf(">> nals[1].i_payload: %d, pps_size: %d\n", nals[1].i_payload, pps_size);
  printf(">> nals[2].i_payload: %d, sei_size: %d / %ld\n", nals[2].i_payload, sei_size, rec.sei.size());

  return true;
} 

bool VideoEncoder::openFile(std::string filepath) {

  if(ofs.is_open()) {
    printf("error: cannot open the video encoder output file becuase it's already open.\n");
    return false;
  }
  
  ofs.open(filepath.c_str(), std::ios::binary | std::ios::out);
  if(!ofs.is_open()) {
    printf("error: cannot open the video encoder output file: %s\n", filepath.c_str());
    return false;
  }
  
  return true;
}

bool VideoEncoder::writeTagToFile(FLVTag& tag) {

  if(!ofs.is_open()) {
    printf("error: cannot write the video tag to the encoder output file because the file hasn't been opened yet.\n");
    return false;
  }
  
  ofs.write((char*)tag.data, tag.size);

  return true;
}

bool VideoEncoder::closeFile() {

  if(!ofs.is_open()) {
    printf("error: cannot close the encoder file because it hasn't been opened yet.\n");
    return false;
  }

  ofs.close();

  return true;
}

bool VideoEncoder::shutdown() {

  if(ofs.is_open()) {
    closeFile();
  }

  if(!encoder) {
    printf("error: encoder is not initialized.\n");
    return false;
  }

  printf("Shutting down the video encoder.\n");

  x264_encoder_close(encoder);
  encoder = NULL;

  frame_num = 0;

  encoder = NULL;

  return true;
}

