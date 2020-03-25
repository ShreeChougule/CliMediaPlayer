/*
 * metadatareader.h
 *
 *  Created on: 09-Apr-2019
 *      Author: kpit
 */

#ifndef METADATAREADER_H_
#define METADATAREADER_H_

#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <list>

#include "../mediatypes.h"
#include "CoverartExtractor.h"
#include "mediascanner.h"
#include "MetaData.h"

using namespace TagLib::ID3v2;
using namespace TagLib;

namespace kivi {
namespace media {

/*!
 * @class  MetaDataReader
 * @brief class realizes IMetaDataHandler class and the implementation of
 * read metadata
 */

class MetaDataReader : public IBatchCallBack, public thread_t {
   public:
    /*!
     * @brief protected constructor and destructor creation of singleton object
     */
    MetaDataReader();
    MetaDataReader(MetaDataReader&&);
    virtual ~MetaDataReader();

    MetaDataReader& operator=(MetaDataReader&&);
    /*!
     * @brief Override function of IMetaDataHandler to read Meta-data
     * @param DataBase interface parameter to use database functionality.
     * @return No return
     */
    void ReadMetaData(songs_path&);
    void StartReading();
    void removeArtworkData();
    long int ThreadRun();
    void CalculateTimeTaken(high_resolution_clock::time_point);
    void OnBatchComplete(songs_path&&);
    void OnScanningComplete();
    void OnIndexingComplete();

   private:
    /*!
     * @brief removing  quoting constraints while inserting into database
     * @param reference of string to which replace character
     * @param search to replace string
     * @param actual replaceable string
     * @return No return
     */
    void findAndReplaceAll(std::string&, std::string, std::string);
    std::string getArtWork(std::string);

   public:
    static bool isScanningComplete;
    static bool isIndexingComplete;
    /*!
     * @brief reference to IDataBase
     */
    DataBase* m_dataBase;

   private:
    std::list<songs_path> m_batchQueue;

    songs_metadata m_metadataObjVect;
    /*!
     * @brief reference to ImediaScanner
     */
    MediaScanner* m_scanner;
    /*!
     * @brief reference to Coverart extractor
     */
    CoverartExtractor* m_artWorkExtractor;

    MetaData* m_metadata;

    /*!
     * @brief used as scanning percentage parameter
     */
    static int m_PercCount;
    bool m_waiting;
    std::mutex m_reqMutex;
    high_resolution_clock::time_point m_stop_time;
    wait_object_t m_waitRead;
    std::vector<song_data_struct> m_metadataBatch;
    int count;
    int m_counter;
};
}  // namespace media
}  // namespace kivi
#endif /* METADATAREADER_H_ */
