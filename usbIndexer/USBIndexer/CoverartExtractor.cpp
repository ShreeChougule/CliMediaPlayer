/*
 * CoverartExtractor.cpp
 *
 *  Created on: 25-Jun-2019
 *      Author: kpit
 */

#include "CoverartExtractor.h"

CoverartExtractor::CoverartExtractor()
    : m_counter(0), isIndexingComplete(false), m_waiting(false) {}

CoverartExtractor::~CoverartExtractor() {}

void CoverartExtractor::Initialize() {
    m_counter = 0;
    isIndexingComplete = false;
    m_waiting = false;
}

void CoverartExtractor::OnIndexingCompleted() {
    isIndexingComplete = true;
    if (m_waiting) m_waitRead.Set();
}

long int CoverartExtractor::ThreadRun() {
    ///////////////////////////////////////////////////////////////////
    while (1) {
        if (m_coverartQueue.empty() && isIndexingComplete == false) {
            std::cout << "\n\t## WAIT in Coverart extractor thread##\n";
            m_waiting = true;
            m_waitRead.Wait(1000);
            m_waiting = false;
        } else if (m_coverartQueue.empty() && isIndexingComplete == true) {
            // Release Thread
            std::cout << "\n Completed Writing CoverArt files ! \n";
            return 0;

        } else {
            CoverArtObj CoverArtObj = std::move(m_coverartQueue.front());
            CreateArtWork(CoverArtObj);
            m_coverartQueue.pop_front();
        }
    }
    ///////////////////////////////////////////////////////////////////
    return 0;
}

std::string CoverartExtractor::getArtWork(std::string filepath, std::string filename) {
    TagLib::MPEG::File mp3File(filepath.c_str());
    TagLib::ID3v2::Tag *mp3Tag = NULL;
    FrameList listOfMp3Frames;

    std::string m_imageoutPath = "";
    if (mp3File.hasID3v2Tag()) mp3Tag = mp3File.ID3v2Tag();
    if (mp3Tag) {
        listOfMp3Frames = mp3Tag->frameListMap()["APIC"];  // look for picture frames only
        if (!listOfMp3Frames.isEmpty()) {
            m_counter++;

            m_imageoutPath =
                "/home/kpit/Work/usbIndexer/Coverart/Image" + std::to_string(m_counter) + ".jpg";
            //            cout << "\n\nFile Name : " << filename << "\nImageFile : " <<
            //            m_imageoutPath;
            m_coverartQueue.emplace_back(m_imageoutPath, filepath, filename);

        } else {
            //  cerr << "there seem to be no picture frames (APIC) frames in this file" << endl <<
            //  endl;
        }
    } else {
        // cerr << "the file " << filepath << "does not appear to have any mp3 tags" << endl <<
        // endl;
    }

    return m_imageoutPath;
}

void CoverartExtractor::CreateArtWork(CoverArtObj &item) {
    std::string m_imageoutPath = item.imgPath;
    std::string filepath = item.filePath;

    TagLib::MPEG::File mp3File(filepath.c_str());
    TagLib::ID3v2::Tag *mp3Tag;
    FrameList listOfMp3Frames;
    AttachedPictureFrame *pictureFrame;

    mp3Tag = mp3File.ID3v2Tag();
    if (mp3Tag) {
        listOfMp3Frames = mp3Tag->frameListMap()["APIC"];  // look for picture frames only
        if (!listOfMp3Frames.isEmpty()) {
            FrameList::ConstIterator it = listOfMp3Frames.begin();
            for (; it != listOfMp3Frames.end(); it++) {
                pictureFrame = static_cast<AttachedPictureFrame *>(*it);

                FILE *fout;
                fout = fopen(m_imageoutPath.c_str(), "wb");

                fwrite(pictureFrame->picture().data(), pictureFrame->picture().size(), 1, fout);

                fclose(fout);
            }
        } else {
            cerr << "there seem to be no picture frames (APIC) frames in this file" << endl << endl;
        }
    } else {
        cerr << "the file " << filepath << "does not appear to have any mp3 tags" << endl << endl;
    }
}
