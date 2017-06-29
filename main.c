

#include <gst/gst.h>

#include "rtp2rtmp.h"

int main(int argc,char *argv[]) {

    gst_init(&argc,&argv);

    GMainLoop *loop;
    loop = g_main_loop_new(NULL,FALSE);


    rtp2rtmp *r2r = g_new0(rtp2rtmp, sizeof(rtp2rtmp));
    init_and_run_rtp2rtmp(r2r,"abc",12345,12347,"rtmp://publish3.cdn.ucloud.com.cn/ucloud/ruaruaruarua");


    g_main_loop_run (loop);
    g_main_loop_unref (loop);

    return 0;
}