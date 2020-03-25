#include "MetaData.h"

// Invalid tag handling for media files containing no tags
// Unknown string handling for the corrupted file

#define INVALID "Invalid"
#define UNKNOWN "Unknown"
#define USE_TAGLIB

namespace kivi {
namespace media {
MetaData::MetaData() {
    m_FileRef = NULL;
    m_MPEGfile = NULL;
    m_WAVfile = NULL;
    m_unknown_cnt = 0;
    m_counter = 0;
    m_SrcImage = NULL;
    m_TagType = NORMAL_TAG;
}

string MetaData::getTitle() {
    string title;
    try {
#ifdef USE_TAGLIB

        switch (m_TagType) {
            case NORMAL_TAG:
                if (m_FileRef->tag()) { title = m_FileRef->tag()->title().to8Bit(true); }
                break;
            case INFO_TAG:
                if (m_WAVfile->InfoTag()) { title = m_WAVfile->InfoTag()->title().to8Bit(true); }
                break;
            case MP4_TAG:
                if (m_MPEGfile->tag()) { title = m_MPEGfile->tag()->title().to8Bit(true); }
                break;
        }
#else

        title = m_MediaInfo->Get(MediaInfoDLL::Stream_General, 0, "Title").c_str();
#endif

    } catch (std::exception &ex) { title = INVALID; }

    return title;
}

string MetaData::getArtist() {
    string artist;
    try {
#ifdef USE_TAGLIB

        switch (m_TagType) {
            case NORMAL_TAG:
                if (m_FileRef->tag()) { artist = m_FileRef->tag()->artist().to8Bit(true); }
                break;
            case INFO_TAG:
                if (m_WAVfile->InfoTag()) { artist = m_WAVfile->InfoTag()->artist().to8Bit(true); }
                break;
            case MP4_TAG:
                if (m_MPEGfile->tag()) { artist = m_MPEGfile->tag()->artist().to8Bit(true); }
                break;
        }
#else
        artist = m_MediaInfo->Get(MediaInfoDLL::Stream_General, 0, "Artist").c_str();
#endif

    } catch (std::exception &ex) {

        artist = UNKNOWN;
        return 0;
    }
    if (artist.length() == 0) { artist = "Unknown artist"; }

    return artist;
}

string MetaData::getAlbum() {
    string album;
    try {
#ifdef USE_TAGLIB

        switch (m_TagType) {
            case NORMAL_TAG:
                if (m_FileRef->tag()) { album = m_FileRef->tag()->album().to8Bit(true); }
                break;
            case INFO_TAG:
                if (m_WAVfile->InfoTag()) { album = m_WAVfile->InfoTag()->album().to8Bit(true); }
                break;
            case MP4_TAG:
                if (m_MPEGfile->tag()) { album = m_MPEGfile->tag()->album().to8Bit(true); }
                break;
        }
#else
        album = m_MediaInfo->Get(MediaInfoDLL::Stream_General, 0, "Album").c_str();
#endif
    } catch (std::exception &ex) {

        album = UNKNOWN;
        return 0;
    }
    if (album.length() == 0) { album = "Unknown album"; }
    return album;
}

string MetaData::getGenre() {
    string genre;
    try {
#ifdef USE_TAGLIB

        switch (m_TagType) {
            case NORMAL_TAG:
                if (m_FileRef->tag()) { genre = m_FileRef->tag()->genre().to8Bit(true); }
                break;
            case INFO_TAG:
                if (m_WAVfile->InfoTag()) { genre = m_WAVfile->InfoTag()->genre().to8Bit(true); }
                break;
            case MP4_TAG:
                if (m_MPEGfile->tag()) { genre = m_MPEGfile->tag()->genre().to8Bit(true); }
                break;
        }
#else
        genre = m_MediaInfo->Get(MediaInfoDLL::Stream_General, 0, "Genre").c_str();
#endif

    } catch (std::exception &ex) {
        genre = INVALID;
        return 0;
    }
    if (genre.length() == 0) { genre = UNKNOWN; }

    return genre;
}

int MetaData::assignFilePath(string &file_path, audio_format_t &audio_format) {
    int retStatus = 1;

    try {
#ifdef USE_TAGLIB

        switch (audio_format) {
            case AUDIO_AAC:
                if (m_MPEGfile) {
                    delete m_MPEGfile;
                    m_MPEGfile = NULL;
                }
                m_MPEGfile = new TagLib::MPEG::File(file_path.c_str());
                m_TagType = MP4_TAG;
                if (m_MPEGfile) retStatus = 0;
                break;

            case AUDIO_WAV:
                if (m_WAVfile) {
                    delete m_WAVfile;
                    m_WAVfile = NULL;
                }
                m_WAVfile = new TagLib::RIFF::WAV::File(file_path.c_str());
                m_TagType = INFO_TAG;
                if (m_WAVfile) retStatus = 0;
                break;

            default:
                if (m_FileRef) {
                    delete m_FileRef;
                    m_FileRef = NULL;
                }
                m_FileRef = new TagLib::FileRef(file_path.c_str());
                m_TagType = NORMAL_TAG;
                if (m_FileRef) retStatus = 0;
                break;
        }
#else
        if (m_MediaInfo) {
            delete m_MediaInfo;
            m_MediaInfo = NULL;
        }

        m_MediaInfo = new MediaInfoDLL::MediaInfo();
        if (m_MediaInfo->Open(file_path)) { retStatus = MM_STATUS_OK; }
#endif

    } catch (std::exception &ex) {}
    return retStatus;
}

}  // namespace media
}  // namespace kivi
