/* GStreamer
 * Copyright (C) 2007 David Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _GST_CAMERA_SRC_H_
#define _GST_CAMERA_SRC_H_

#include <gst/gst.h>
#include <gst/base/gstpushsrc.h>
#include <unistd.h>

G_BEGIN_DECLS

#define GST_TYPE_CAMERA_SRC \
  (gst_camera_src_get_type())
#define GST_CAMERA_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_CAMERA_SRC,GstCameraSrc))
#define GST_CAMERA_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_CAMERA_SRC,GstCameraSrcClass))
#define GST_IS_CAMERA_SRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_CAMERA_SRC))
#define GST_IS_CAMERA_SRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_CAMERA_SRC))
#define GST_CAMERA_SRC_CAST(obj) \
  ((GstCameraSrc*)(obj))

typedef struct _GstCameraSrc GstCameraSrc;
typedef struct _GstCameraSrcClass GstCameraSrcClass;
typedef struct _GstCameraSrcPrivate GstCameraSrcPrivate;

/**
 * GstCameraSrcCallbacks: (skip)
 * @need_data: Called when the camerasrc needs more data. A buffer or EOS should be
 *    pushed to camerasrc from this thread or another thread. @length is just a hint
 *    and when it is set to -1, any number of bytes can be pushed into @camerasrc.
 * @enough_data: Called when camerasrc has enough data. It is recommended that the
 *    application stops calling push-buffer until the need_data callback is
 *    emitted again to avoid excessive buffer queueing.
 * @seek_data: Called when a seek should be performed to the offset.
 *    The next push-buffer should produce buffers from the new @offset.
 *    This callback is only called for seekable stream types.
 *
 * A set of callbacks that can be installed on the camerasrc with
 * gst_camera_src_set_callbacks().
 */
typedef struct {
  void      (*need_data)    (GstCameraSrc *src, guint length, gpointer user_data);
  void      (*enough_data)  (GstCameraSrc *src, gpointer user_data);
  gboolean  (*seek_data)    (GstCameraSrc *src, guint64 offset, gpointer user_data);

  /*< private >*/
  gpointer     _gst_reserved[GST_PADDING];
} GstCameraSrcCallbacks;

/**
 * GstAppStreamType:
 * @GST_APP_STREAM_TYPE_STREAM: No seeking is supported in the stream, such as a
 * live stream.
 * @GST_APP_STREAM_TYPE_SEEKABLE: The stream is seekable but seeking might not
 * be very fast, such as data from a webserver.
 * @GST_APP_STREAM_TYPE_RANDOM_ACCESS: The stream is seekable and seeking is fast,
 * such as in a local file.
 *
 * The stream type.
 */
typedef enum
{
  GST_APP_STREAM_TYPE_STREAM,
  GST_APP_STREAM_TYPE_SEEKABLE,
  GST_APP_STREAM_TYPE_RANDOM_ACCESS
} GstAppStreamType;

struct _GstCameraSrc
{
  GstBaseSrc basesrc;

  /*< private >*/
  GstCameraSrcPrivate *priv;

  /*< private >*/
  gpointer     _gst_reserved[GST_PADDING];
};

struct _GstCameraSrcClass
{
  GstBaseSrcClass basesrc_class;

  /* signals */
  void          (*need_data)       (GstCameraSrc *camerasrc, guint length);
  void          (*enough_data)     (GstCameraSrc *camerasrc);
  gboolean      (*seek_data)       (GstCameraSrc *camerasrc, guint64 offset);

  /* actions */
  GstFlowReturn (*push_buffer)     (GstCameraSrc *camerasrc, GstBuffer *buffer);
  GstFlowReturn (*end_of_stream)   (GstCameraSrc *camerasrc);

  /*< private >*/
  gpointer     _gst_reserved[GST_PADDING];
};

GType gst_camera_src_get_type(void);

/* GType getter for GstAppStreamType */
#define GST_TYPE_APP_STREAM_TYPE (gst_app_stream_type_get_type ())
GType gst_app_stream_type_get_type (void);

void             gst_camera_src_set_caps         (GstCameraSrc *camerasrc, const GstCaps *caps);
GstCaps*         gst_camera_src_get_caps         (GstCameraSrc *camerasrc);

void             gst_camera_src_set_size         (GstCameraSrc *camerasrc, gint64 size);
gint64           gst_camera_src_get_size         (GstCameraSrc *camerasrc);

void             gst_camera_src_set_stream_type  (GstCameraSrc *camerasrc, GstAppStreamType type);
GstAppStreamType gst_camera_src_get_stream_type  (GstCameraSrc *camerasrc);

void             gst_camera_src_set_max_bytes    (GstCameraSrc *camerasrc, guint64 max);
guint64          gst_camera_src_get_max_bytes    (GstCameraSrc *camerasrc);

void             gst_camera_src_set_latency      (GstCameraSrc *camerasrc, guint64 min, guint64 max);
void             gst_camera_src_get_latency      (GstCameraSrc *camerasrc, guint64 *min, guint64 *max);

void             gst_camera_src_set_emit_signals (GstCameraSrc *camerasrc, gboolean emit);
gboolean         gst_camera_src_get_emit_signals (GstCameraSrc *camerasrc);

GstFlowReturn    gst_camera_src_push_buffer      (GstCameraSrc *camerasrc, GstBuffer *buffer);
GstFlowReturn    gst_camera_src_end_of_stream    (GstCameraSrc *camerasrc);

void             gst_camera_src_set_callbacks    (GstCameraSrc * camerasrc,
                                               GstCameraSrcCallbacks *callbacks,
                                               gpointer user_data,
                                               GDestroyNotify notify);

G_END_DECLS

#endif

