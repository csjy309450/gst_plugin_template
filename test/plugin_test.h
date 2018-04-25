//
// Created by yz on 18-3-25.
//

#ifndef PROJECT_PLUGIN_TEST_H
#define PROJECT_PLUGIN_TEST_H

#include <gst/gst.h>
#include <unistd.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/shared_ptr.hpp>

using namespace cv;
using namespace boost;
using namespace std;

void camerasrc_test(int argc,char *argv[]);
void myfilter_test(int argc,char *argv[]);
void appasrc_test(int argc,char *argv[]);


#endif //PROJECT_PLUGIN_TEST_H
