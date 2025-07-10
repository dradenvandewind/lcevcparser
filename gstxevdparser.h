#ifndef __GST_XEVE_PARSER_H__
#define __GST_XEVE_PARSER_H__

#include <gst/base/gstbaseparse.h>
#include <gst/gst.h>
#include <gst/video/video.h>

G_BEGIN_DECLS

#define GST_TYPE_XEVE_PARSE (gst_xeve_parse_get_type())
#define GST_XEVE_PARSE(obj)                                                    \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_XEVE_PARSE, GstXeveParse))

// Forward declarations
typedef struct _GstXeveParse GstXeveParse;
typedef struct _GstXeveParseClass GstXeveParseClass;

struct _GstXeveParse {

  /*< private >*/
  guint bs_read_pos;
  guint state; // Current state of the parser

  /*< private >*/
  gpointer priv;
};

struct _GstXeveParseClass {
  GstBaseParseClass parent_class;
};

GType gst_xeve_parse_get_type(void);

G_END_DECLS

#endif /* __GST_XEVE_PARSER_H__ */