

#include <gst/gst.h>

#include "rtp2rtmp.h"

int main(int argc,char *argv[]) {


    gint audio_port = 12345;
    gint video_port = 12347;
    gchar *rtmp_url = "rtmp://192.168.1.165:1935/myapp/testav";

    GOptionContext *ctx;
    GError *err = NULL;
    GOptionEntry entries[] = {
            { "audio-port", 'a', 0, G_OPTION_ARG_INT, &audio_port,
                    "input audio port","12345"  },
            { "video-port", 'v', 0, G_OPTION_ARG_INT, &video_port,
                    "input video port","12347"  },
            { "rtmp", 'r', 0, G_OPTION_ARG_STRING, &rtmp_url,
                    "rtmp url","..."  },
            { NULL }
    };

    ctx = g_option_context_new (NULL);
    g_option_context_add_main_entries (ctx, entries, NULL);
    g_option_context_add_group (ctx, gst_init_get_option_group ());
    if (!g_option_context_parse (ctx, &argc, &argv, &err)) {
        g_print ("Failed to initialize: %s\n", err->message);
        g_clear_error (&err);
        g_option_context_free (ctx);
        return 1;
    }



    GMainLoop *loop;
    loop = g_main_loop_new(NULL,FALSE);


    rtp2rtmp *r2r = g_new0(rtp2rtmp, sizeof(rtp2rtmp));
    //init_and_run_rtp2rtmp(r2r,"abc",12345,12347,"rtmp://publish3.cdn.ucloud.com.cn/ucloud/ruaruaruarua");
    init_and_run_rtp2rtmp(r2r,"abc",video_port,audio_port,rtmp_url);


    g_main_loop_run (loop);
    g_main_loop_unref (loop);

    return 0;
}