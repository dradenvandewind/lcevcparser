#ifndef __GST_XEVE_ENC_H__
#define __GST_XEVE_ENC_H__

#include <gst/gst.h>

#include <gst/video/video.h>

G_BEGIN_DECLS

#define GST_TYPE_XEVE_PARSE (gst_xeve_parse_get_type())
#define GST_XEVE_PARSE(obj)                                                    \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_XEVE_PARSE, GstXeveParse))

#define GST_TYPE_XEVE_PARSER (gst_xeve_parser_get_type())
G_DECLARE_FINAL_TYPE(GstXeveParser, gst_xeve_parser, GST, XEVE_PARSER,
                     GstBaseParse)

struct _GstXeveParser {
  GstBaseParse parent;

  /*< private >*/
  guint bs_read_pos;
  guint state; // Current state of the parser

  /*< private >*/
  gpointer priv;
};

G_END_DECLS

#endif /* __GST_XEVE_DEC_H__ */
