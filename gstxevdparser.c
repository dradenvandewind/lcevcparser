#include "gstxevdparser.h"
#include <gst/base/gstbaseparse.h>
#include <gst/gst.h>

#define XEVD_NAL_UNIT_LENGTH_BYTE 4
#define PACKAGE "gstlcevcparser"
#define VERSION "1.0.0"

GST_DEBUG_CATEGORY_STATIC(gst_xeve_parser_debug);
#define GST_CAT_DEFAULT gst_xeve_parser_debug

/* typedef struct _GstXeveParse {
  GstBaseParse parent;
  guint bs_read_pos;
  guint state;
} GstXeveParse;

typedef struct _GstXeveParseClass {
  GstBaseParseClass parent_class;
} GstXeveParseClass;
 */

enum { STATE_NORMAL = 0, STATE_BUMPING = 1 };

#define SRC_CAPS_TMPL "video/x-lvc1, parsed=(boolean)false"
#define SINK_CAPS_TMPL "video/x-lvc1, parsed=(boolean)true"

GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE(
    "sink", GST_PAD_SINK, GST_PAD_ALWAYS,
    GST_STATIC_CAPS(
        SINK_CAPS_TMPL
        ", stream-format = (string) byte-stream, alignment = (string) au"));

GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE(
    "src", GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS(SRC_CAPS_TMPL));

G_DEFINE_TYPE(GstXeveParse, gst_xeve_parse, GST_TYPE_BASE_PARSE);

static void gst_xeve_parse_set_property(GObject *object, guint prop_id,
                                        const GValue *value, GParamSpec *pspec);
static void gst_xeve_parse_get_property(GObject *object, guint prop_id,
                                        GValue *value, GParamSpec *pspec);
static void gst_xeve_parse_dispose(GObject *object);

static gboolean gst_xeve_parse_start(GstBaseParse *parse);
static gboolean gst_xeve_parse_stop(GstBaseParse *parse);
static gboolean gst_xeve_parse_set_format(GstBaseParse *parse, GstCaps *caps,
                                          GstVideoCodecState *state);
static GstFlowReturn gst_xeve_parse_handle_frame(GstBaseParse *parse,
                                                 GstBaseParseFrame *frame,
                                                 gint *skipsize);

static void gst_xeve_parse_class_init(GstXeveParseClass *klass) {
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS(klass);
  GstBaseParseClass *base_parse_class = GST_BASE_PARSE_CLASS(klass);

  GST_DEBUG_CATEGORY_INIT(gst_xeve_parser_debug, "xeveparser", 0,
                          "XEVE video parser");

  gobject_class->set_property = gst_xeve_parse_set_property;
  gobject_class->get_property = gst_xeve_parse_get_property;
  gobject_class->dispose = gst_xeve_parse_dispose;

  base_parse_class->handle_frame = gst_xeve_parse_handle_frame;

  gst_element_class_add_static_pad_template(element_class, &sink_template);
  gst_element_class_add_static_pad_template(element_class, &src_template);

  gst_element_class_set_static_metadata(
      element_class, "XEVD Video Parser", "Codec/Decoder/Video",
      "Parse video streams using XEVD",
      "Erwan Le Blond <erwanleblond@gmail.com>");

  base_parse_class->start = gst_xeve_parse_start;
  base_parse_class->stop = gst_xeve_parse_stop;

  base_parse_class->handle_frame = gst_xeve_parse_handle_frame;
}

static void gst_xeve_parse_init(GstXeveParse *self) {
  GST_DEBUG_OBJECT(self, "init called");

  self->bs_read_pos = 0;
  self->state = STATE_NORMAL;
}
static void gst_xeve_parse_set_property(GObject *object, guint prop_id,
                                        const GValue *value,
                                        GParamSpec *pspec) {
  GstXeveParse *self = GST_XEVE_PARSE(object);
  switch (prop_id) {
  // Handle other properties if needed
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}
static void gst_xeve_parse_get_property(GObject *object, guint prop_id,
                                        GValue *value, GParamSpec *pspec) {
  GstXeveParse *self = GST_XEVE_PARSE(object);
  switch (prop_id) {
  // Handle other properties if needed
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}
static void gst_xeve_parse_dispose(GObject *object) {
  GstXeveParse *self = GST_XEVE_PARSE(object);
  GST_DEBUG_OBJECT(self, "dispose called");

  // Clean up resources if needed
  self->bs_read_pos = 0;
  self->state = STATE_NORMAL;

  G_OBJECT_CLASS(gst_xeve_parse_parent_class)->dispose(object);
}
static gboolean gst_xeve_parse_start(GstBaseParse *parse) {
  GstXeveParse *xeveparse = GST_XEVE_PARSE(parse);
  GST_DEBUG_OBJECT(xeveparse, "start called");
  // Initialize any resources needed for parsing
  xeveparse->bs_read_pos = 0;
  xeveparse->state = STATE_NORMAL;
  return TRUE; // Indicate that parsing can start
}
static gboolean gst_xeve_parse_stop(GstBaseParse *parse) {
  GstXeveParse *xeveparse = GST_XEVE_PARSE(parse);
  GST_DEBUG_OBJECT(xeveparse, "stop called");
  // Clean up resources if needed
  xeveparse->bs_read_pos = 0;
  xeveparse->state = STATE_NORMAL;
  return TRUE; // Indicate that parsing can stop
}
static gboolean gst_xeve_parse_set_format(GstBaseParse *parse, GstCaps *caps,
                                          GstVideoCodecState *state) {
  GstXeveParse *xeveparse = GST_XEVE_PARSE(parse);
  GST_DEBUG_OBJECT(xeveparse, "set_format called");
  // Set the format based on the provided caps and state
  if (!gst_caps_is_fixed(caps)) {
    GST_ERROR_OBJECT(xeveparse, "Caps are not fixed");
    return FALSE; // Indicate failure to set format
  }

  return TRUE; // Indicate success in setting format
}

static GstFlowReturn gst_xeve_parse_handle_frame(GstBaseParse *parse,
                                                 GstBaseParseFrame *frame,
                                                 gint *skipsize) {
  GstXeveParse *xeveparse = GST_XEVE_PARSE(parse);
  GstBuffer *buffer = frame->buffer;
  GstMapInfo map;
  guint8 *data;
  gsize size;
  guint32 nal_size;

  if (!gst_buffer_map(buffer, &map, GST_MAP_READ))
    return GST_FLOW_ERROR;

  data = map.data;
  size = map.size;

  // Lire la taille du NAL unit (4 bytes)
  if (size < XEVD_NAL_UNIT_LENGTH_BYTE) {
    gst_buffer_unmap(buffer, &map);
    return GST_FLOW_OK; // Pas assez de données
  }

  // Extraire la taille du NAL unit
  nal_size = GST_READ_UINT32_BE(data);

  if (nal_size <= 0) {
    xeveparse->state = STATE_BUMPING;
    gst_buffer_unmap(buffer, &map);
    return GST_FLOW_OK;
  }

  // Vérifier si on a assez de données
  if (size < XEVD_NAL_UNIT_LENGTH_BYTE + nal_size) {
    gst_buffer_unmap(buffer, &map);
    return GST_FLOW_OK; // Pas assez de données
  }

  // Créer un nouveau buffer avec seulement les données du NAL unit
  GstBuffer *out_buffer = gst_buffer_new_and_alloc(nal_size);
  gst_buffer_fill(out_buffer, 0, data + XEVD_NAL_UNIT_LENGTH_BYTE, nal_size);

  frame->out_buffer = out_buffer;
  frame->size = XEVD_NAL_UNIT_LENGTH_BYTE + nal_size;

  gst_buffer_unmap(buffer, &map);
  return GST_FLOW_OK;
}

static gboolean plugin_init(GstPlugin *plugin) {
  return gst_element_register(plugin, "xevdparser", GST_RANK_PRIMARY,
                              GST_TYPE_XEVE_PARSE);
}

GST_PLUGIN_DEFINE(GST_VERSION_MAJOR, GST_VERSION_MINOR, xevdparser,
                  "XEVD Video Parser", plugin_init, VERSION, "LGPL",
                  "GStreamer", "https://example.com")
