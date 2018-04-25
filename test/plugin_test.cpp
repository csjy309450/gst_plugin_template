//
// Created by yz on 18-3-25.
//

#include "plugin_test.h"

#ifdef CAMERASRC_TEST

typedef struct _CBParam{
  GstElement * pipe;
  GMainLoop *loop;
}CBParam;

bool get_cmd(string & input){
  std::cout<<">> ";
  std::cin>>input;
  return true;
}

gpointer cb_controller (gpointer data){
  string input;
  CBParam * param = (CBParam *)data;
  GMainLoop * loop = param->loop;
  GstElement * pipe = param->pipe;
  GstBin * bin = (GstBin *)param->pipe;

  while(get_cmd(input)){
    if(input=="stop"){
      gst_element_set_state(pipe,GST_STATE_NULL);
      g_main_loop_quit(loop);
      break;
    }
    else if(input=="play"){
      gst_element_set_state(pipe,GST_STATE_PLAYING);
//      if(!g_main_loop_is_running(loop))
//        g_main_loop_run(loop);
    }
    else if(input=="pause"){
      gst_element_set_state(pipe,GST_STATE_PAUSED);
    }
    else if(input=="channal"){
      int channal;
      std::cout<<" = ";
      std::cin>>channal;
      GstElement * camerasrc = gst_bin_get_by_name(bin,"camerasrc");
      gst_element_set_state(pipe,GST_STATE_NULL);
      g_object_set(G_OBJECT(camerasrc),"frame-channal",channal,NULL);
      gst_element_set_state(pipe,GST_STATE_PLAYING);
    }
  }
  return NULL;
}

void camerasrc_test(int argc,char *argv[]){
  CBParam param;
  GstElement *pipeline,*camerasrc,*conv,*sink0,*sink1;
  GMainLoop *loop;
  GThread * controler;
  gst_init(&argc,&argv);
  loop=g_main_loop_new(NULL,FALSE);
  pipeline = gst_pipeline_new("pipeline");
  camerasrc = gst_element_factory_make("camerasrc","camerasrc");
  conv = gst_element_factory_make ("videoconvert", "conv");
  sink0 = gst_element_factory_make ("autovideosink", "sink0");
  sink1 = gst_element_factory_make ("autovideosink", "sink1");

//  g_object_set(G_OBJECT(camerasrc), "frame-channal",1,NULL);

  if(!pipeline||!camerasrc||!conv||!sink0||!sink1){
    return;
  }
  gst_bin_add_many(GST_BIN(pipeline),camerasrc,conv,sink0,sink1,NULL);
  //依次连接组件
  gst_element_link_many(camerasrc,conv,sink0,NULL);

  gst_element_set_state(pipeline,GST_STATE_PLAYING);

  param.loop = loop;
  param.pipe=pipeline;
  controler = g_thread_new("controller",(GThreadFunc)cb_controller, (gpointer)&param);

  g_main_loop_run(loop);
  g_thread_join(controler);
}

#endif // CAMERASRC_TEST

#ifdef APPSRC_TEST

int g_width=500,g_height=500;
GMainLoop * loop;

static void
cb_need_data (GstElement    *appsrc,
              guint         unused_size,
              gpointer      user_data)
{
//    std::cout<<*(int*)user_data<<std::endl;
  static gboolean white = FALSE;
  static GstClockTime timestamp = 0;
  static int n=0;
  GstBuffer *buffer;
//    GstMapInfo info;
  gsize size = (gsize)1*g_width*g_height;
  static shared_ptr<VideoCapture> pCap= shared_ptr<VideoCapture>(new VideoCapture(0));
  static Mat t_frame,frame;
  static gchar *data = g_new(gchar, size);

  for(gint i=0;i<1;i++){
    (*pCap) >> t_frame; // get a new frame from camera
    cvtColor(t_frame, t_frame, CV_RGB2GRAY);
    resize(t_frame, frame, cv::Size(g_width,g_height));
    g_strlcpy(data+i*g_width*g_height, (gchar*)frame.data, g_width*g_height);
  }

  GstFlowReturn ret;

//    size = g_width*g_height*3;
  // 为GstBuffer申请内存, 用于与Mat对象交互数据
  buffer = gst_buffer_new_allocate (NULL, size, NULL);
//    gst_buffer_map(buffer, &info, GST_MAP_WRITE);
  // 将一片Mat内存区域的数据写入GstBuffer
  gst_buffer_fill(buffer,0,(gpointer*)data,size);

  if(buffer==NULL)
  {
    cout <<"buffer allocate failed"<<endl;
  }
  GST_BUFFER_PTS (buffer) = timestamp;
  GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 2);

  timestamp += GST_BUFFER_DURATION (buffer);
  // 将buffer 缓冲的数据推入appsrc元件的src pad缓冲区中
  g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);
  gst_buffer_unref (buffer);

  if (ret != GST_FLOW_OK) {
    /* something wrong, stop pushing */
    g_main_loop_quit (loop);
  }
    std::cout<<n++<<std::endl;
}

void appasrc_test(int argc,char *argv[]){
  GstElement *pipeline,*appsrc,*conv,*sink;
  GMainLoop *loop;
  gst_init(&argc,&argv);
  loop=g_main_loop_new(NULL,FALSE);
  pipeline = gst_pipeline_new("_pipeline");
  appsrc = gst_element_factory_make("appsrc","appsrc");
  conv = gst_element_factory_make ("videoconvert", "conv");
  sink = gst_element_factory_make ("autovideosink", "sink");
//  sink = gst_element_factory_make ("filesrc", "sink");
  g_object_set (G_OBJECT (appsrc), "caps",
                gst_caps_new_simple ("video/x-raw",
                                     "format", G_TYPE_STRING, "GRAY8"/*"BGR"*/,
                                     "width", G_TYPE_INT, g_width,
                                     "height", G_TYPE_INT, g_height,
                                     "framerate", GST_TYPE_FRACTION, 0.5, 1,
                                     NULL), NULL);

  g_signal_connect(appsrc,"need-data", G_CALLBACK (cb_need_data), NULL);

//  g_object_set(G_OBJECT(sink), "location","./ret_camerasrc.txt",NULL);

  if(!pipeline||!appsrc||!sink||!conv){
    return;
  }
  gst_bin_add_many(GST_BIN(pipeline),appsrc,conv,sink,NULL);
  //依次连接组件
  gst_element_link_many(appsrc,conv,sink,NULL);
  gst_element_set_state(pipeline,GST_STATE_PLAYING);
  g_main_loop_run(loop);
}

#endif //APPSRC_TEST

#ifdef MYFILTER_TEST

void myfilter_test(int argc,char *argv[]){
  GstElement *pipeline,*filesink,*filesrc,*myfilter;
  GMainLoop *loop;
  gst_init(&argc,&argv);
  loop=g_main_loop_new(NULL,FALSE);
  pipeline = gst_pipeline_new("_pipeline");
  filesrc = gst_element_factory_make("filesrc","_filesrc");
  myfilter = gst_element_factory_make("myfilter","_myfilter");
  filesink = gst_element_factory_make("filesink","_filesink");
  g_object_set(G_OBJECT(filesrc),"location","./test.txt",NULL);
  g_object_set(G_OBJECT(myfilter),"silent",TRUE,NULL);
  g_object_set(G_OBJECT(filesink),"location","./ret_myfilter.txt",NULL);
//    g_object_set(playbin,"video-sink",mfw_v4lsink,NULL);
//    gst_element_set_state(playbin,GST_STATE_PAUSED);
//    sleep(2);
  if(!pipeline||!filesrc||!myfilter||!filesink){
    return;
  }
  gst_bin_add_many(GST_BIN(pipeline),filesrc,myfilter,filesink,NULL);
  //依次连接组件
  gst_element_link_many(filesrc,myfilter,filesink,NULL);
  gst_element_set_state(pipeline,GST_STATE_PLAYING);
  g_main_loop_run(loop);
}

#endif // MYFILTER_TEST