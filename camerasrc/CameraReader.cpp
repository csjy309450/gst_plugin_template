//
// Created by yz on 18-3-25.
//

#include <strstream>
#include <sstream>
#include <string>
#include "CameraReader.h"

string IntToString(int n)
{
  std::string result;
  std::strstream ss;
  ss <<  n;
  ss >> result;
  return result;
}

CameraReader::CameraReader(int device){
  pCap = shared_ptr<VideoCapture>(new VideoCapture(device));
  if(!pCap->isOpened())  // check if we succeeded
    state = false;
  else{
    pmutex_frame = g_mutex_new();
    if(!pmutex_frame)
      state = false;
    else{
      state = true;
    }
  }

}

CameraReader::CameraReader(const string & filename){
  pCap = shared_ptr<VideoCapture>(new VideoCapture(filename));
  if(!pCap->isOpened())  // check if we succeeded
    state = false;
}

CameraReader::~CameraReader(){

}

Mat & CameraReader::read_a_frame_unlock(int width, int height, int channal){
  Mat t_frame/* = Mat(width,height,CV_8UC(channal))*/;
  (*pCap) >> t_frame; // get a new frame from camera
  switch(channal){
    case 3:
      break;
    case 1:default:
      cvtColor(t_frame, t_frame, CV_RGB2GRAY);
      break;
  }

  resize(t_frame, frame, cv::Size(width,height));
  g_mutex_unlock(pmutex_frame);

#ifdef PLUGIN_CAMERASRC_DEBUG
  string frame_name;
  static int n=0;
  frame_name = "./save/" + IntToString(n++) + ".jpg";
  imwrite(frame_name,t_frame);
#endif // PLUGIN_CAMERASRC_DEBUG

  return frame;
}

void CameraReader::lock(){
  g_mutex_lock(pmutex_frame);
}

void CameraReader::unlock(){
  g_mutex_unlock(pmutex_frame);
}

int CameraReader::get_camera_frame_width_unlock(){
  if(isAvailable()) {
    unlock();
    return (int)pCap->get(CV_CAP_PROP_FRAME_WIDTH);
  }
}
int CameraReader::get_camera_frame_height_unlock(){
  if(isAvailable()) {
    unlock();
    return (int)pCap->get(CV_CAP_PROP_FRAME_HEIGHT);
  }
}

static shared_ptr<CameraReader> CReader;

gpointer read_a_frame_C(int width, int height, int channal){
  if(CReader->isAvailable()) {
    CReader->lock();
    return gpointer(CReader->read_a_frame_unlock(width, height, channal).data);
  }
  else
    return NULL;
}

gint get_camera_frame_width_C(){
  if(CReader->isAvailable()) {
    CReader->lock();
    return (gint)(CReader->get_camera_frame_width_unlock());
  }
}

gint get_camera_frame_height_C(){
  if(CReader->isAvailable()) {
    CReader->lock();
    return (gint)(CReader->get_camera_frame_height_unlock());
  }
}

int create_video_capture_C(int device){
  CReader = shared_ptr<CameraReader>(new CameraReader(device));
}