//
// Created by yz on 18-3-25.
//

#ifndef PROJECT_CAMERAREADER_H
#define PROJECT_CAMERAREADER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <boost/shared_ptr.hpp>
#include <gst/gst.h>

using namespace cv;
using namespace std;
using namespace boost;

class CameraReader {
private:
  shared_ptr<VideoCapture> pCap;
  Mat frame;
  bool state;
  GMutex * pmutex_frame;

public:
  CameraReader(int device);
  CameraReader(const string & filename);
  ~CameraReader();
  int get_camera_frame_width_unlock();
  int get_camera_frame_height_unlock();
  void lock();
  void unlock();
  Mat & read_a_frame_unlock(int width, int height, int channal);
  inline bool isAvailable(){
    return state;
  };
};

extern "C"{
gpointer read_a_frame_C(int width, int height, int channal);
gint get_camera_frame_width_C();
gint get_camera_frame_height_C();
int create_video_capture_C(int device);
};

#endif //PROJECT_CAMERAREADER_H
