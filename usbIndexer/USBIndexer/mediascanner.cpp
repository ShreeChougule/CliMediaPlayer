/*
 * mediascanner.cpp
 *
 *  Created on: 08-Apr-2019
 *      Author: ashish
 */

#include "mediascanner.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

namespace kivi {
namespace media {

MediaScanner* MediaScanner::theInstance = NULL;
bool MediaScanner::isIndexingCompleted = false;
int MediaScanner::totalFilesScanned = 0;

MediaScanner::MediaScanner() : count(0), m_asyncall(NULL), m_batchSize(10) {}

// Move Constructor
MediaScanner::MediaScanner(MediaScanner&& source) : m_asyncall(NULL), m_batchSize(10), count(0) {
    std::cout << "\n INSIDE Scanner Move Constructor";

    m_asyncall = source.m_asyncall;
    m_batchSize = source.m_batchSize;
    count = source.count;

    source.m_asyncall = NULL;
    source.m_batchSize = 0;
    source.count = 0;
}

MediaScanner& MediaScanner::operator=(MediaScanner&& source) {
    std::cout << "\n INSIDE Scanner Move = Operator";
    if (this != &source) {
        // free current resources
        delete m_asyncall;

        // copy data
        m_asyncall = source.m_asyncall;
        m_batchSize = source.m_batchSize;
        count = source.count;

        // release source data so that destructor does not free memory multiple times
        source.m_asyncall = NULL;
        source.m_batchSize = 0;
        source.count = 0;
    }

    return *this;
}

MediaScanner::~MediaScanner() {}

void MediaScanner::Initialize(IBatchCallBack* asynccall) {
    // TODO:
    m_asyncall = asynccall;
}

void MediaScanner::StartScanning(device_paths_t path) {
    DIR* dirp = opendir(path.c_str());

    struct dirent* dp;
    std::string dir_name;
    std::vector<std::string> internal_dirs;  // to store internal directories(recursive
    std::vector<std::string>::iterator internal_dirs_iterator;  // iterator for internal

    while ((dp = readdir(dirp)) != NULL) {
        std::string d_name(dp->d_name);
        if (dp->d_type == DT_REG) {
            audio_format_t format = getFormat(d_name);
            if (format != NO_AUDIO) {
                // file's parent directory
                dir_name = path;
                // File's name
                dir_name.append(d_name);

                m_batchPathVect.emplace_back(dir_name, d_name, format);
                count++;
                if (count == m_batchSize) {
                    MediaScanner::totalFilesScanned += m_batchSize;
                    m_asyncall->OnBatchComplete(std::move(m_batchPathVect));
                    count = 0;
                }
            }
        } else if (dp->d_type == DT_DIR) {
            if ((d_name == ".") || (d_name == "..") || (d_name[0] == '.')) continue;

            dir_name = path;
            dir_name.append(dp->d_name);
            dir_name.append("/");
            internal_dirs.push_back(dir_name);
        }
    }

    closedir(dirp);

    for (internal_dirs_iterator = internal_dirs.begin();
         internal_dirs_iterator != internal_dirs.end(); internal_dirs_iterator++) {
        StartScanning(*internal_dirs_iterator);
    }
}

void MediaScanner::ScanDevice(std::string dev_path, int batchSize) {
    m_batchSize = batchSize;
    StartScanning(dev_path);
    if (m_batchPathVect.size() != 0) {
        MediaScanner::totalFilesScanned += m_batchPathVect.size();
        m_asyncall->OnBatchComplete(std::move(m_batchPathVect));
        count = 0;
    }
    m_asyncall->OnScanningComplete();
}

audio_format_t MediaScanner::getFormat(std::string FileName) {

    std::transform(FileName.begin(), FileName.end(), FileName.begin(), ::tolower);

    if (std::string::npos != FileName.find(".mp3"))
        return AUDIO_MP3;
    else if (std::string::npos != FileName.find(".wma"))
        return AUDIO_WMA;
    else if (std::string::npos != FileName.find(".flac"))
        return AUDIO_FLAC;
    else if (std::string::npos != FileName.find(".ogg"))
        return AUDIO_OGG;
    else if (std::string::npos != FileName.find(".wav"))
        return AUDIO_WAV;
    else if (std::string::npos != FileName.find(".m4a"))
        return AUDIO_M4A;
    else if (std::string::npos != FileName.find(".aac"))
        return AUDIO_AAC;
    else
        return NO_AUDIO;
}

void MediaScanner::CancelScanning() {
    // TODO:
}

}  // namespace media
}  // namespace kivi
