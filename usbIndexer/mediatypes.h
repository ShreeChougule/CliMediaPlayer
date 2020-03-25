/////////////////////////////////////////////////////////////////////////////
// NAME        : mediatypes.h
// PURPOSE     : All datatypes to be exposed to client
// AUTHOR      : KPIT Technologies Ltd.
// DATE        : 5 Dec 2018
//
// COPYRIGHT NOTICE:
// (C) 2018 KPIT Technologies Limited
// Created in 2018 as an unpublished copyright work.
// All rights reserved.
//
// Provided under the terms of KIVI Agreement
//
// This source code file is proprietary to KPIT Technologies Limited.
// It may not be used, copied, reproduced, transmitted, or stored
// in any form or by any means, electronic, recording, photocopying,
// mechanical or otherwise, without the prior written permission of
// KPIT Technologies Limited.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef MEDIATYPES_H_
#define MEDIATYPES_H_

#include <stddef.h>
#include <iostream>
#include <string>
#include <vector>
// Types for new Indexer

// TODO : merge with old types
typedef enum audio_format {
    NO_AUDIO = 0x00,
    AUDIO_MP3,
    AUDIO_WMA,
    AUDIO_WAV,
    AUDIO_OGG,
    AUDIO_FLAC,
    AUDIO_M4A,
    AUDIO_AAC
} audio_format_t;

typedef enum tag_type { NORMAL_TAG = 0x00, MP4_TAG, ID3V2_TAG, INFO_TAG } tag_type_t;

using device_paths_t = std::string;

struct song_data_struct {
    song_data_struct()
        : filename(""), title(""), path(""), artist(""), album(""), genre(""), coverartPath("") {}
    song_data_struct(const song_data_struct& source)
        : filename(source.filename),
          title(source.title),
          path(source.path),
          artist(source.artist),
          album(source.album),
          genre(source.genre),
          coverartPath(source.coverartPath) {}

    song_data_struct(const song_data_struct&& source)
        : filename(std::move(source.filename)),
          title(std::move(source.title)),
          path(std::move(source.path)),
          artist(std::move(source.artist)),
          album(std::move(source.album)),
          genre(std::move(source.genre)),
          coverartPath(std::move(source.coverartPath)) {}
    song_data_struct& operator=(const song_data_struct& source) {
        filename = source.filename;
        title = source.title;
        path = source.path;
        artist = source.artist;
        album = source.album;
        genre = source.genre;
        coverartPath = source.coverartPath;

        return *this;
    }

    song_data_struct& operator=(song_data_struct&& source) {
        filename = std::move(source.filename);
        title = std::move(source.title);
        path = std::move(source.path);
        artist = std::move(source.artist);
        album = std::move(source.album);
        genre = std::move(source.genre);
        coverartPath = std::move(source.coverartPath);

        return *this;
        typedef enum audio_format {
            AUDIO_MP3 = 0x00,
            AUDIO_WMA,
            AUDIO_WAV,
            AUDIO_OGG,
            AUDIO_FLAC,
            AUDIO_M4A,
            AUDIO_AAC
        } audio_format_t;
    }
    std::string filename;
    std::string title;
    std::string path;
    std::string artist;
    std::string album;
    std::string genre;
    std::string coverartPath;
};

struct CoverArtObj {
    CoverArtObj() : imgPath(""), filePath(""), fileName("") {}
    CoverArtObj(std::string imgpath, std::string filepath, std::string filename)
        : imgPath(imgpath), filePath(filepath), fileName(filename) {}
    CoverArtObj(const CoverArtObj& source)
        : imgPath(source.imgPath), filePath(source.filePath), fileName(source.fileName) {}
    CoverArtObj(CoverArtObj&& source)
        : imgPath(std::move(source.imgPath)),
          filePath(std::move(source.filePath)),
          fileName(std::move(source.fileName)) {}
    CoverArtObj& operator=(const CoverArtObj& source) {
        imgPath = source.imgPath;
        filePath = source.filePath;
        fileName = source.fileName;
        return *this;
    }
    CoverArtObj& operator=(CoverArtObj&& source) {
        imgPath = std::move(source.imgPath);
        filePath = std::move(source.filePath);
        fileName = std::move(source.fileName);
        return *this;
    }

    std::string imgPath;
    std::string filePath;
    std::string fileName;
};

struct file_info {
    file_info() : path(""), fileName(""), format(AUDIO_MP3) {}
    file_info(std::string filepath, std::string filename, audio_format_t Format)
        : path(filepath), fileName(filename), format(Format) {}
    file_info(const file_info& source)
        : path(source.path), fileName(source.fileName), format(source.format) {}
    file_info(file_info&& source)
        : path(std::move(source.path)),
          fileName(std::move(source.fileName)),
          format(std::move(source.format)) {}

    file_info& operator=(const file_info& source) {
        path = source.path;
        fileName = source.fileName;
        format = source.format;
        return *this;
    }
    file_info& operator=(file_info&& source) {
        path = std::move(source.path);
        fileName = std::move(source.fileName);
        format = source.format;
        return *this;
    }

    std::string path;
    std::string fileName;
    audio_format_t format;
};

using songs_path = std::vector<file_info>;

using songs_metadata = std::vector<song_data_struct>;

using media_file_path_iter = std::vector<file_info>::iterator;

#endif /* MEDIAPLAYBACKTYPES_H_ */
