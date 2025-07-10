# lcevcparser
gst-inspect-1.0 /usr/lib/x86_64-linux-gnu/gstreamer-1.0/gstxevdparser.so 
Plugin Details:
  Name                     xevdparser
  Description              XEVD Video Parser
  Filename                 /usr/lib/x86_64-linux-gnu/gstreamer-1.0/gstxevdparser.so
  Version                  1.0.0
  License                  LGPL
  Source module            gstlcevcparser
  Binary package           GStreamer
  Origin URL               https://example.com

  xevdparser: XEVD Video Parser

  1 features:
  +-- 1 elements
st-inspect-1.0 xevdparser                                               
Factory Details:
  Rank                     primary (256)
  Long-name                XEVD Video Parser
  Klass                    Codec/Decoder/Video
  Description              Parse video streams using XEVD
  Author                   Erwan Le Blond <erwanleblond@gmail.com>

Plugin Details:
  Name                     xevdparser
  Description              XEVD Video Parser
  Filename                 /usr/lib/x86_64-linux-gnu/gstreamer-1.0/gstxevdparser.so
  Version                  1.0.0
  License                  LGPL
  Source module            gstlcevcparser
  Binary package           GStreamer
  Origin URL               https://example.com

GObject
 +----GInitiallyUnowned
       +----GstObject
             +----GstElement
                   +----GstBaseParse
                         +----GstXeveParse

Element Flags:
  - REQUIRE_INDEXABLE

Pad Templates:
  SINK template: 'sink'
    Availability: Always
    Capabilities:
      video/x-lvc1
                 parsed: true
          stream-format: byte-stream
              alignment: au
  
  SRC template: 'src'
    Availability: Always
    Capabilities:
      video/x-lvc1
                 parsed: false

Element has no clocking capabilities.
Element has no URI handling capabilities.

Pads:
  SINK: 'sink'
    Pad Template: 'sink'
  SRC: 'src'
    Pad Template: 'src'

Element Properties:

  disable-passthrough : Force processing (disables passthrough)
                        flags: readable, writable
                        Boolean. Default: false
  
  name                : The name of the object
                        flags: readable, writable
                        String. Default: "xeveparse0"
  
  parent              : The parent of the object
                        flags: readable, writable
                        Object of type "GstObject"

