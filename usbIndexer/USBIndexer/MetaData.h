#ifndef MULTIMEDIA_METADATAIMPL_H
#define MULTIMEDIA_METADATAIMPL_H

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <taglib/mp4file.h>
#include <taglib/flacfile.h>
#include <taglib/oggfile.h>
#include <taglib/rifffile.h>
#include <taglib/infotag.h>
#include <taglib/wavfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/fileref.h>
#include <taglib/id3v2extendedheader.h>
#include <taglib/id3v2footer.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2framefactory.h>
#include <taglib/id3v2header.h>
#include <taglib/id3v2synchdata.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/mpegheader.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>
#include <unistd.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "../mediatypes.h"

using namespace std;
using namespace TagLib::ID3v2;
namespace kivi {
namespace media {
class MetaData {
   public:
    MetaData();
    string getTitle();
    string getArtist();
    string getAlbum();
    string getGenre();
    int assignFilePath(string &file_path, audio_format_t &);

   private:
    TagLib::FileRef *m_FileRef;
    TagLib::MPEG::File *m_MPEGfile;
    TagLib::RIFF::WAV::File *m_WAVfile;
    int m_unknown_cnt;
    void *m_SrcImage;
    int m_counter;
    tag_type_t m_TagType;
};
}  // namespace media
}  // namespace kivi
#endif
