#include<gtk/gtk.h>
#include<stdio.h>
#include <gst/gst.h>


static void destroy(GtkWidget*, gpointer);
static gboolean mouse_moved(GtkWidget *widget,GdkEvent *event,gpointer user_data);


void gst_config(){

//gst-launch-1.0 -e ximagesrc startx=0 use-damage=0 !
//'video/x-raw,framerate=30/1' !
//queue !
//videoconvert !
//x264enc threads=0 bitrate=400 tune=zerolatency key-int-max=30 !
//queue !
//flvmux name=flvmux !
//queue !
//rtmpsink location=rtmp://live-fra.twitch.tv/app/live_631791363_ZsrYHlHf3hvo7cAjWuuwwKJQKZI9AF


GstElement *ximagesrc, *queue0, *videoconvert, *x264enc;
GstElement *queue1, *flvmux, *queue2, *rtmpsink;
GstElement *pipeline;
GstCaps *caps;
GstBus *bus;

gst_init (NULL, NULL);

pipeline = gst_pipeline_new (NULL);
g_assert (pipeline);
gst_element_set_state (pipeline, GST_STATE_NULL);

ximagesrc = gst_element_factory_make ("ximagesrc", NULL);
g_assert(ximagesrc);

queue0 = gst_element_factory_make ("queue", NULL);
g_assert(queue0);

videoconvert = gst_element_factory_make ("videoconvert", NULL);
g_assert(videoconvert);

x264enc = gst_element_factory_make ("x264enc", NULL);
g_assert (x264enc);

queue1 = gst_element_factory_make ("queue", NULL);
g_assert (queue1);

flvmux = gst_element_factory_make ("flvmux", NULL);
g_assert (flvmux);

queue2 = gst_element_factory_make ("queue", NULL);
g_assert (queue2);

rtmpsink = gst_element_factory_make ("rtmpsink", NULL);
g_assert (rtmpsink);

caps = gst_caps_from_string ("video/x-raw,framerate=30/1");

gst_bin_add_many (GST_BIN (pipeline), ximagesrc, queue0, videoconvert, x264enc,
    queue1, flvmux, queue2, rtmpsink, NULL);

if (!gst_element_link_filtered (ximagesrc, queue0, caps))
  g_error ("Failed to link audioconvert and audioresample");

if (!gst_element_link (queue0, videoconvert))
  g_error ("Failed to link udpsrc and rawaudioparse");

if (!gst_element_link (videoconvert, x264enc))
  g_error ("Failed to link rawaudioparse and queue");

if (!gst_element_link (x264enc, queue1))
  g_error ("Failed to link queue and audioconvert");

  if (!gst_element_link (queue1, flvmux))
    g_error ("Failed to link queue and audioconvert");

if (!gst_element_link (flvmux, queue2))
  g_error ("Failed to link audioresample and level");

if (!gst_element_link (queue2, rtmpsink))
  g_error ("Failed to link level and alsasink");

  g_object_set (G_OBJECT (ximagesrc), "startx", 0, NULL);
  g_object_set (G_OBJECT (ximagesrc), "use-damage", 0, NULL);

  g_object_set (G_OBJECT (x264enc), "threads", 0, NULL);
  g_object_set (G_OBJECT (x264enc), "bitrate", 400, NULL);
  g_object_set (G_OBJECT (x264enc), "tune", 0x00000004, NULL);
  g_object_set (G_OBJECT (x264enc), "key-int-max", 30, NULL);

  g_object_set (G_OBJECT (rtmpsink), "location", "location=rtmp://live-fra.twitch.tv/app/live_631791363_ZsrYHlHf3hvo7cAjWuuwwKJQKZI9AF", NULL);


  gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

int main(int argc, char* argv[]) {

    GtkWidget *main_window;

    gtk_init(NULL, NULL);

    gst_config();

    main_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window),"Test");
    gtk_widget_set_size_request (main_window, 500, 300);

    // connect the window with signals
    g_signal_connect (G_OBJECT (main_window), "destroy",G_CALLBACK (destroy), NULL);
    g_signal_connect (G_OBJECT (main_window), "motion-notify-event",G_CALLBACK (mouse_moved), NULL);

    gtk_widget_set_events(main_window, GDK_POINTER_MOTION_MASK);

    // show window
    gtk_widget_show_all (main_window);

    gtk_main();
    return 0;
}


static void destroy(GtkWidget *window,gpointer data) {
    gtk_main_quit ();
}


static gboolean mouse_moved(GtkWidget *widget,GdkEvent *event, gpointer user_data) {

    if (event->type==GDK_MOTION_NOTIFY) {
        GdkEventMotion* e=(GdkEventMotion*)event;
        printf("Coordinates: (%u,%u)\n", (guint)e->x,(guint)e->y);
    }
}
