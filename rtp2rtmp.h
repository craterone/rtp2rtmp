//
// Created by cong chen on 6/29/17.
//

#ifndef WEBRTC2RTMP_RTP2RTMP_H
#define WEBRTC2RTMP_RTP2RTMP_H

#include <gst/gst.h>

typedef struct _rtp2rtmp{
    GstElement *pipeline;
    GstElement *videoUdpsrc,*rtph264depay; //video
    GstElement *audioUdpsrc,*rtpopusdepay,*opusparse,*opusdec,*voaacenc,*aacparse; //audio
    GstElement *flvmux;//mux
    GstElement *rtmp;

    GstElement *q1,*q2,*q3,*q4;
    GstCaps *videoRTPCaps;
    GstCaps *audioRTPCaps;
    GstCaps *audioAacCaps;

    GstPad *videoSinkPad,*audioSinkPad;
    GstPad *videoSrcPad,*audioSrcPad;
} rtp2rtmp;

gboolean init_and_run_rtp2rtmp(rtp2rtmp *r2r,const gchar *id,int audio_port,int video_port,const gchar *rtmp_url);

#endif //WEBRTC2RTMP_RTP2RTMP_H
