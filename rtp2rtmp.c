//
// Created by cong chen on 6/29/17.
//

#include "rtp2rtmp.h"


#define VIDEO_RTP_CAPS "application/x-rtp, payload=(int)107, encoding-name=(string)H264, media=(string)video, clock-rate=(int)90000"
#define AUDIO_RTP_CAPS "application/x-rtp, payload=(int)111, encoding-name=(string)OPUS, media=(string)audio, clock-rate=(int)48000, channels=(int)2"

#define AUDIO_FLV_CAPS "audio/mpeg, mpegversion=(int)4,stream-format=(string)raw"


gboolean init_and_run_rtp2rtmp(rtp2rtmp *r2r,const gchar *id,int audio_port,int video_port,const gchar *rtmp_url){
    //todo(cc): add id to element name
    r2r->pipeline = gst_pipeline_new("rtp-rtmp-converter");
    if(!r2r->pipeline){
        g_printerr ("pipeline failed.\n");
        return -1;
    }

    r2r->videoUdpsrc = gst_element_factory_make("udpsrc","video-udp-src");

    if(!r2r->videoUdpsrc){
        g_printerr ("videoUdpsrc failed.\n");
        return -1;
    }

    r2r->rtph264depay = gst_element_factory_make("rtph264depay","h264-rtp");
    if(!r2r->rtph264depay){
        g_printerr ("rtph264depay failed.\n");
        return -1;
    }

    r2r->audioUdpsrc = gst_element_factory_make("udpsrc","audio-udp-src");
    if(!r2r->audioUdpsrc){
        g_printerr ("audioUdpsrc failed.\n");
        return -1;
    }

    r2r->rtpopusdepay = gst_element_factory_make("rtpopusdepay","opus-rtp");
    if(!r2r->rtpopusdepay){
        g_printerr ("rtpopusdepay failed.\n");
        return -1;
    }

    r2r->opusparse = gst_element_factory_make("opusparse","opus-parse");
    if(!r2r->opusparse){
        g_printerr ("opusparse failed.\n");
        return -1;
    }

    r2r->opusdec = gst_element_factory_make("opusdec","opus-dec");
    if(!r2r->opusdec){
        g_printerr ("opusdec failed.\n");
        return -1;
    }

    r2r->voaacenc = gst_element_factory_make("voaacenc","vo-aacenc");
    if(!r2r->voaacenc){
        g_printerr ("voaacenc failed.\n");
        return -1;
    }

    r2r->aacparse = gst_element_factory_make("aacparse","aac-parse");
    if(!r2r->aacparse){
        g_printerr ("aacparse failed.\n");
        return -1;
    }


    r2r->rtmp = gst_element_factory_make("rtmpsink","rtmp-sink");
    if(!r2r->rtmp){
        g_printerr ("rtmp failed.\n");
        return -1;
    }


    r2r->flvmux = gst_element_factory_make("flvmux","flvmux");
    if(!r2r->flvmux){
        g_printerr ("flvmux failed.\n");
        return -1;
    }


    r2r->q1 = gst_element_factory_make("queue2","q1");
    if(!r2r->q1){
        g_printerr ("q1 failed.\n");
        return -1;
    }

    r2r->q2 = gst_element_factory_make("queue2","q2");
    if(!r2r->q2){
        g_printerr ("q2 failed.\n");
        return -1;
    }

    r2r->q3 = gst_element_factory_make("queue2","q3");
    if(!r2r->q3){
        g_printerr ("q3 failed.\n");
        return -1;
    }

    r2r->q4 = gst_element_factory_make("queue2","q4");
    if(!r2r->q4){
        g_printerr ("q4 failed.\n");
        return -1;
    }


    g_object_set(G_OBJECT(r2r->audioUdpsrc),"port",audio_port,NULL);
    g_object_set(G_OBJECT(r2r->videoUdpsrc),"port",video_port,NULL);

    g_object_set(G_OBJECT(r2r->voaacenc),"bitrate",128000,NULL);

    g_object_set(G_OBJECT(r2r->flvmux),"streamable",TRUE,NULL);

    g_object_set(G_OBJECT(r2r->rtmp),"location",rtmp_url,NULL);


    gst_bin_add_many(GST_BIN(r2r->pipeline),r2r->videoUdpsrc,r2r->rtph264depay,r2r->q1,
                     r2r->audioUdpsrc,r2r->rtpopusdepay,r2r->opusparse,r2r->opusdec,r2r->q4, r2r->voaacenc,r2r->aacparse,r2r->q2,r2r->q3,
                     r2r->flvmux,r2r->rtmp,NULL);

    r2r->videoRTPCaps = gst_caps_from_string(VIDEO_RTP_CAPS);

    if (!GST_IS_CAPS(r2r->videoRTPCaps)) {
        g_printerr ("video rtp caps null ??.\n");
        return -1;
    }

    r2r->audioRTPCaps = gst_caps_from_string(AUDIO_RTP_CAPS);

    if (!GST_IS_CAPS(r2r->audioRTPCaps)) {
        g_printerr ("audio rtp caps null ??.\n");
        return -1;
    }


    //video link
    gst_element_link_filtered(r2r->videoUdpsrc,r2r->rtph264depay,r2r->videoRTPCaps);
    gst_element_link(r2r->rtph264depay,r2r->q1);
    //audio link
    gst_element_link_filtered(r2r->audioUdpsrc,r2r->rtpopusdepay,r2r->audioRTPCaps);
    gst_element_link_many(r2r->rtpopusdepay,r2r->opusparse,r2r->opusdec,r2r->q4,r2r->voaacenc,r2r->aacparse,r2r->q2,NULL);


    r2r->videoSinkPad = gst_element_get_request_pad (r2r->flvmux, "video");

    if (!r2r->videoSinkPad ) {
        g_printerr ("videoSinkPad null ??.\n");
        return -1;
    }

    r2r->videoSrcPad = gst_element_get_static_pad(r2r->q1,"src");
    if (!r2r->videoSrcPad ) {
        g_printerr ("videoSrcPad null ??.\n");
        return -1;
    }

    gst_pad_link (r2r->videoSrcPad, r2r->videoSinkPad);

    r2r->audioSinkPad = gst_element_get_request_pad (r2r->flvmux, "audio");
    r2r->audioAacCaps = gst_caps_from_string(AUDIO_FLV_CAPS);
    gst_pad_set_caps(r2r->audioSinkPad,r2r->audioAacCaps);

    r2r->audioSrcPad = gst_element_get_static_pad(r2r->q2,"src");

    gst_pad_link (r2r->audioSrcPad, r2r->audioSinkPad);

    gst_element_link_many(r2r->flvmux,r2r->q3,r2r->rtmp,NULL);

    gst_element_set_state (r2r->pipeline, GST_STATE_PLAYING);

    return 0;
}
