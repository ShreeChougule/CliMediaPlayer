/*
 * CoverartExtractor.h
 *
 *  Created on: 25-Jun-2019
 *      Author: kpit
 */
#ifndef USBINDEXER_COVERARTEXTRACTOR_H_
#define USBINDEXER_COVERARTEXTRACTOR_H_

#include <stdio.h>
#include <string.h>

#include <taglib/mp4file.h>
#include <taglib/flacfile.h>
#include <taglib/oggfile.h>
#include <taglib/rifffile.h>
#include <taglib/infotag.h>
#include <taglib/mpegfile.h>
#include <taglib/mpegheader.h>

#include <taglib/attachedpictureframe.h>
#include <taglib/fileref.h>
#include <taglib/id3v2extendedheader.h>
#include <taglib/id3v2footer.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2framefactory.h>
#include <taglib/id3v2header.h>
#include <taglib/id3v2synchdata.h>
#include <taglib/id3v2tag.h>
#include <taglib/tpropertymap.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>
#include <iostream>
#include <list>
#include <string>

#include "mediascanner.h"

using namespace std;
using namespace TagLib::ID3v2;
using namespace TagLib;

class CoverartExtractor : public thread_t {
   public:
    CoverartExtractor();
    ~CoverartExtractor();

    void Initialize();
    long int ThreadRun();

    std::string getArtWork(std::string, std::string);
    void CreateArtWork(CoverArtObj&);
    void OnIndexingCompleted();

   private:
    int m_counter;
    wait_object_t m_waitRead;
    bool m_waiting;
    bool isIndexingComplete;
    std::list<CoverArtObj> m_coverartQueue;
};

#endif /* USBINDEXER_COVERARTEXTRACTOR_H_ */
