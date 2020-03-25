/*
 * GstreamerImpl.h
 *
 *  Created on: 19-Jul-2019
 *      Author: kpit
 */

#ifndef GSTREAMERIMPL_H_
#define GSTREAMERIMPL_H_
#include </usr/local/usr/include/kivi/osal/thread.h>
#include </usr/local/usr/include/kivi/osal/wait_object.h>
#include <iostream>
#include <string>
#include <gst/gst.h>

#include "../DbHandler/DbHandler.h"

static GstElement *audio = NULL;

enum playback_status { STOPPED = 0x00, PLAYING, PAUSED };

class GstreamerImpl : public thread_t {
   public:
    GstreamerImpl();
    ~GstreamerImpl();

    void Initialize();
    void create_pipeline(std::string);
    void PlayPause();
    void Previous();
    void Next();

    void stop();
    long int ThreadRun();
    void distroy_Pipeline();
    void StartPlayback(std::string);
    void Stop_Player();
    static void on_new_pad(GstElement *, GstPad *, GstElement *);
    static void on_pad_added(GstElement *dec, GstPad *pad, GstElement *fakesink);
    static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data);

   private:
    GMainLoop *loop;
    GstElement *pipeline;
    guint bus_watch_id;
    GstMessage *msg;
    std::string m_filepath;
    std::string m_album, m_title, m_artist;
    int playlist_size;
    int current_index;
    bool isPlaying;
    playback_status m_playbackStatus;
};

#endif /* GSTREAMERIMPL_H_ */
