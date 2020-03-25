/*
 * metadatareaderimpl.cpp
 *
 *  Created on: 09-Apr-2019
 *      Author: kpit
 */

#include "metadatareader.h"
#include "printprogress.h"

using namespace TagLib;

namespace kivi {
namespace media {

int MetaDataReader::m_PercCount = 0;
bool MetaDataReader::isScanningComplete = false;
bool MetaDataReader::isIndexingComplete = false;

MetaDataReader::~MetaDataReader() { delete m_scanner; }

MetaDataReader::MetaDataReader()
    : m_scanner(new MediaScanner()),
      m_dataBase(new DataBase()),
      m_artWorkExtractor(new CoverartExtractor()),
      m_metadata(new MetaData),
      m_waiting(false),
      count(0),
      m_counter(0) {}

MetaDataReader::MetaDataReader(MetaDataReader&& source)
    : m_scanner(NULL),
      m_dataBase(NULL),
      m_artWorkExtractor(NULL),
      m_metadata(NULL),
      m_waiting(false),
      count(0),
      m_counter(0) {
    std::cout << "\nInside Metadatareader Move Constructor\n";

    // copy data
    m_scanner = source.m_scanner;
    m_dataBase = source.m_dataBase;
    m_artWorkExtractor = source.m_artWorkExtractor;
    m_metadata = source.m_metadata;
    m_waiting = source.m_waiting;
    count = source.count;
    m_counter = source.m_counter;

    // release source data
    delete source.m_scanner;
    delete source.m_dataBase;
    delete source.m_artWorkExtractor;
    delete source.m_metadata;
    source.m_waiting = false;
    source.count = 0;
    source.m_counter = 0;
}

MetaDataReader& MetaDataReader::operator=(MetaDataReader&& source) {
    std::cout << "\nInside Metadatareader Move = Operator\n";

    if (this != &source) {
        // free current resources
        delete m_scanner;
        delete m_dataBase;
        delete m_artWorkExtractor;
        delete m_metadata;

        // copy data
        m_scanner = source.m_scanner;
        m_dataBase = source.m_dataBase;
        m_artWorkExtractor = source.m_artWorkExtractor;
        m_metadata = source.m_metadata;
        m_waiting = source.m_waiting;
        count = source.count;
        m_counter = source.m_counter;

        // release source data
        delete source.m_scanner;
        delete source.m_dataBase;
        delete source.m_artWorkExtractor;
        delete source.m_metadata;
        source.m_waiting = false;
        source.count = 0;
        source.m_counter = 0;
    }
    return *this;
}

void MetaDataReader::ReadMetaData(songs_path& songPathVect) {
    song_data_struct instance;
    std::string sql_query;

    int status = 0;
    for (media_file_path_iter iter = songPathVect.begin(); iter != songPathVect.end(); iter++) {
        file_info fileinfo = *iter;
        status = m_metadata->assignFilePath(fileinfo.path, fileinfo.format);
        if (status == 0) {
            count++;
            instance.filename = fileinfo.fileName;
            instance.title = m_metadata->getTitle();
            instance.artist = m_metadata->getArtist();
            instance.album = m_metadata->getAlbum();
            instance.genre = m_metadata->getGenre();
            instance.path = fileinfo.path;
            if (fileinfo.format == AUDIO_MP3)
                instance.coverartPath =
                    m_artWorkExtractor->getArtWork(fileinfo.path, fileinfo.fileName);

            //      std::cout << "\n instance.coverartPath = " <<
            //      instance.coverartPath;

            if (instance.title == "") instance.title = fileinfo.fileName;
            // removing  quoting constraints
            findAndReplaceAll(instance.title, "'", "''");
            findAndReplaceAll(instance.artist, "'", "''");
            findAndReplaceAll(instance.album, "'", "''");
            findAndReplaceAll(instance.path, "'", "''");
            findAndReplaceAll(instance.genre, "'", "''");

            m_metadataBatch.push_back(instance);

            //      std::cout << " ###### Song File Name : " << fileinfo.fileName
            //                << "\n TITLE: " << instance.title
            //                << " ALBUM: " << instance.album << " GENRE: " <<
            //                instance.genre
            //                << "\n";

            if (count == 10) {
                m_dataBase->Enqueue(std::move(m_metadataBatch));
                //                m_metadataBatch.clear();
                count = 0;
            }
        }
        m_PercCount++;
        if (isScanningComplete) {
            // Notify Percentage
            //            std::cout << "\n Completed Read files : " << m_PercCount;
            // printProgress((m_PercCount), MediaScanner::totalFilesScanned);
        }
    }
}

void MetaDataReader::OnScanningComplete() {
    isScanningComplete = true;
    std::cout << "\n Files Scanned = " << MediaScanner::totalFilesScanned;
}

void MetaDataReader::OnIndexingComplete() {
    m_stop_time = high_resolution_clock::now();
    isIndexingComplete = true;
    m_waitRead.Set();
}

void MetaDataReader::StartReading() {
    m_artWorkExtractor->Initialize();
    m_artWorkExtractor->Start();
    while (1) {
        if (m_batchQueue.empty() && isScanningComplete == false) {
            //      std::cout << "\n if (m_dataList.empty() && "
            //                   "MediaScanner::isScanningCompleted == "
            //                   "false)\t## WAIT ##\n";
            m_waiting = true;
            m_waitRead.Wait(5000);
            m_waiting = false;
        } else if (m_batchQueue.empty() && isScanningComplete == true) {
            // Notify Database i.e Metadata Finished
            if (m_metadataBatch.size() != 0) {
                m_dataBase->Enqueue(std::move(m_metadataBatch));
                m_metadataBatch.clear();
            }
            m_dataBase->NotifyScanReadComplete(true);
            // Release Thread
            if (isIndexingComplete) {
                m_artWorkExtractor->OnIndexingCompleted();
                std::cout << "\n Completed Read files : " << m_PercCount;
                std::cout << "\n Files Scanned = " << MediaScanner::totalFilesScanned;
                return;
            } else {
                m_waitRead.Wait(1000);
            }
        } else {
            songs_path batchObj = std::move(m_batchQueue.front());
            ReadMetaData(batchObj);
            m_batchQueue.pop_front();
        }
    }
}
void MetaDataReader::CalculateTimeTaken(high_resolution_clock::time_point start_time) {
    duration<double> time_span = duration_cast<duration<double>>(m_stop_time - start_time);
    std::cout << "\n\t######## Time Taken : " << time_span.count() << " seconds. ########\n";
}

void MetaDataReader::OnBatchComplete(songs_path&& list) {
    m_batchQueue.push_back(std::move(list));
    if (m_waiting) m_waitRead.Set();
}

long int MetaDataReader::ThreadRun() {
    m_dataBase->InitDB();
    m_dataBase->Start();
    StartReading();
    return 0;
}

void MetaDataReader::findAndReplaceAll(std::string& data, std::string toSearch,
                                       std::string replaceStr) {
    // Get the first occurrence
    std::size_t pos = data.find(toSearch);

    // Repeat till end is reached
    while (pos != std::string::npos) {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

void MetaDataReader::removeArtworkData() {
    string path = "/home/kpit/Work/usbIndexer/Coverart/";
    DIR* dirp = opendir(path.c_str());
    if (!dirp) { LOG3((TEXT(" Issue opening dir : %s \r\n"), path.c_str())); }
    struct dirent* dp;
    std::string filepath;
    while ((dp = readdir(dirp)) != NULL) {
        filepath = path;
        if (dp->d_type == DT_REG) {
            filepath.append(dp->d_name);

            int ret_code = remove(filepath.c_str());
            if (ret_code == 0) {
                LOG3((TEXT(" Deleted : %s \r\n"), dp->d_name));
            } else {
                LOG3((TEXT(" Error during the deletion : %s \r\n"), dp->d_name));
            }
        }
    }
    closedir(dirp);
}
}  // namespace media
}  // namespace kivi
