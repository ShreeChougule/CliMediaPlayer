///*
// * GstreamerImp

#include "GstreamerImpl.h"

GstreamerImpl::GstreamerImpl()
    : loop(nullptr),
      pipeline(nullptr),
      msg(nullptr),
      m_filepath(""),
      m_album(""),
      m_title(""),
      m_artist(""),
      playlist_size(0),
      current_index(0),
      isPlaying(false),
      m_playbackStatus(STOPPED) {}

GstreamerImpl::~GstreamerImpl() {}

void GstreamerImpl::Initialize() {
    /* Initialisation */
    gst_init(NULL, NULL);
    loop = g_main_loop_new(NULL, FALSE);
    playlist_size = DbHandler::getCount();
    //    std::cout << "\nplaylist_size : " << playlist_size << "\n";
    if (playlist_size > 0) current_index = 1;
}

long int GstreamerImpl::ThreadRun() {
    isPlaying = true;
    StartPlayback(DbHandler::getTrackPath(current_index));
    stop();
    isPlaying = false;
    return 0;
}

void GstreamerImpl::Next() {
    stop();
    this->Terminate();

    if (current_index != playlist_size)
        ++current_index;
    else
        current_index = 1;

    this->Start();
}

void GstreamerImpl::Previous() {
    stop();
    this->Terminate();
    if (current_index != 1)
        --current_index;
    else
        current_index = playlist_size;

    this->Start();
}

void GstreamerImpl::PlayPause() {
    if (m_playbackStatus == PLAYING) {
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
        m_playbackStatus = PAUSED;
    } else if (m_playbackStatus == PAUSED) {
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
        m_playbackStatus = PLAYING;
    }
}

void GstreamerImpl::stop() {
    if (isPlaying) {
        if (pipeline) {
            gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
            gst_object_unref(GST_OBJECT(pipeline));
        }
        g_source_remove(bus_watch_id);
        if (g_main_loop_is_running(loop)) {
            g_main_loop_quit(loop);
            isPlaying = false;
        }
    }
    m_playbackStatus = STOPPED;
}

void GstreamerImpl::distroy_Pipeline() {
    /* Destroy the pipeline */
    if (isPlaying) {
        if (pipeline) {
            gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
            gst_object_unref(GST_OBJECT(pipeline));
        }
    }
    g_main_loop_unref(loop);
    m_playbackStatus = STOPPED;
}

void GstreamerImpl::Stop_Player() {
    stop();
    distroy_Pipeline();
}

gboolean GstreamerImpl::bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
    GMainLoop *loop = (GMainLoop *)data;

    switch (GST_MESSAGE_TYPE(msg)) {

        case GST_MESSAGE_EOS:
            g_print("End of stream\n");
            if (g_main_loop_is_running(loop)) g_main_loop_quit(loop);
            break;

        case GST_MESSAGE_ERROR: {
            gchar *debug;
            GError *error;

            gst_message_parse_error(msg, &error, &debug);
            g_free(debug);

            g_printerr("Error: %s\n", error->message);
            g_error_free(error);

            g_main_loop_quit(loop);
            break;
        }
        default: break;
    }
    return TRUE;
}

void GstreamerImpl::on_new_pad(GstElement *dec, GstPad *pad, GstElement *fakesink) {
    GstPad *sinkpad;
    std::cout << "In the function :: " << __FUNCTION__ << std::endl;
    sinkpad = gst_element_get_static_pad(fakesink, "sink");
    if (!gst_pad_is_linked(sinkpad)) {
        if (gst_pad_link(pad, sinkpad) != GST_PAD_LINK_OK) { g_error("Failed to link pads!"); }
    }
    gst_object_unref(sinkpad);
}

void GstreamerImpl::on_pad_added(GstElement *dec, GstPad *pad, GstElement *fakesink) {

    GstCaps *caps;
    GstStructure *str;
    GstPad *audiopad;

    /* only link once */
    audiopad = gst_element_get_static_pad(audio, "sink");
    if (GST_PAD_IS_LINKED(audiopad)) {
        g_object_unref(audiopad);
        return;
    }

    /* check media type */
    caps = gst_pad_query_caps(pad, NULL);
    str = gst_caps_get_structure(caps, 0);
    if (!g_strrstr(gst_structure_get_name(str), "audio")) {
        gst_caps_unref(caps);
        gst_object_unref(audiopad);
        return;
    }
    gst_caps_unref(caps);

    /* link'n'play */
    gst_pad_link(pad, audiopad);

    g_object_unref(audiopad);
}

void GstreamerImpl::StartPlayback(std::string filePath) {
    create_pipeline(filePath);
    DbHandler::ReadMetadata(current_index);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    m_playbackStatus = PLAYING;
    g_main_loop_run(loop);

    // exit
    stop();
}

void GstreamerImpl::create_pipeline(std::string filePath) {
    GstElement *source, *decoder, *conv, *sink;
    GstPad *audiopad;
    GstBus *bus;

    /* Create gstreamer elements */
    pipeline = gst_pipeline_new("audio-player");
    source = gst_element_factory_make("filesrc", "file-source");
    decoder = gst_element_factory_make("decodebin", "decoder");
    conv = gst_element_factory_make("audioconvert", "aconv");
    sink = gst_element_factory_make("autoaudiosink", "audio-output");

    if (!pipeline || !source || !decoder || !conv || !sink) {
        g_printerr("One element could not be created. Exiting.\n");
        return;
    }

    /* Set up the pipeline */
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);

    g_object_set(G_OBJECT(source), "location", filePath.c_str(), NULL);
    g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), NULL);
    gst_bin_add_many(GST_BIN(pipeline), source, decoder, NULL);
    gst_element_link(source, decoder);

    /* create audio output */
    audio = gst_bin_new("audiobin");
    audiopad = gst_element_get_static_pad(conv, "sink");

    gst_bin_add_many(GST_BIN(audio), conv, sink, NULL);
    gst_element_link(conv, sink);
    gst_element_add_pad(audio, gst_ghost_pad_new("sink", audiopad));
    gst_object_unref(audiopad);

    gst_bin_add(GST_BIN(pipeline), audio);
}
