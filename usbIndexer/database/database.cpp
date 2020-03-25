/*
 * database.cpp
 *
 *  Created on: 09-Apr-2019
 *      Author: kpit
 */

#include "database.h"

// kivi namespace
namespace kivi {

// media namespace
namespace media {

DataBase::DataBase()
    : DB(nullptr),
      db_path("mediadb.db"),
      m_waiting(false),
      isScanningComplete(false),
      async(NULL) {}

DataBase::~DataBase() {}

/*!
 * @brief this function open the database by calling OpenDataBase() method of
 * DataBasePrivate
 * @return no any return value
 */
void DataBase::OpenDataBase(IBatchCallBack* ptr) {
    int openStatus = 0;
    //  std::cout << "\n IReaderCallBack* = " << ptr;
    async = ptr;
    openStatus = sqlite3_open(db_path, &DB);
    if (openStatus) {
        // std::cout << "Can't open database: \n";
    } else {
        // std::cout << "Opened database successfully \n";
    }
}

long int DataBase::ThreadRun() {
    StartWritingToDB();
    return 0;
}

void DataBase::StartWritingToDB() {
    while (1) {
        if (m_metadataVec.empty() && isScanningComplete == false) {
            m_waiting = true;
            m_waitRead.Wait(500);
            //      std::cout << "\n DataBase Wait  Called !!!";
            m_waiting = false;
        } else if (m_metadataVec.empty() && isScanningComplete == true) {
            // Notify Reader & Release Thread .......... No use in kmediaengine
            async->OnIndexingComplete();
            return;
        } else {
            songs_metadata batchObj = m_metadataVec.front();
            ReadDataObjVec(batchObj);
            m_metadataVec.pop_front();
        }
    }
}

void DataBase::ReadDataObjVec(songs_metadata& instance) {
    std::string sql_query;
    for (int i = 0; i < instance.size(); i++) {

        sql_query = "INSERT INTO songMaster VALUES(NULL,10002,'" + instance[i].path + "', '" +
                    instance[i].title + "', '" + instance[i].album + "', '" + instance[i].artist +
                    "', '" + instance[i].genre + "',0,1,0,'" + instance[i].coverartPath + "');";
        //        std::cout << "\nQuery : " << sql_query;
        this->InsertQuery(sql_query);
    }
}
/*!
 * @brief this function close the database by calling CloseDataBase() method of
 * DataBasePrivate
 * @return no any return value
 */
void DataBase::CloseDataBase() { sqlite3_close(DB); }
// namespace media

/*!
 * @brief this function close the database and erase database by calling
 * RemoveDevice() method of DataBasePrivate
 * @return no any return value
 */
void DataBase::RemoveDevice() {
    // TODO:
}

void DataBase::NotifyScanReadComplete(bool scanSatatus) {
    //  std::cout << "\n  NotifyScanReadComplete : " << scanSatatus;
    isScanningComplete = true;
}

void DataBase::Enqueue(songs_metadata&& batch) {
    m_metadataVec.emplace_back(std::move(batch));
    if (m_waiting) m_waitRead.Set();
}

/*!
 * @brief this function create database tables and triggers by calling
 * InitDB() method of DataBasePrivate
 * @return success or failure status
 */
int DataBase::InitDB() {
    /*!
     * Create mp3songs database table with following attributes
     * Attributes:
     * device_id, song_id, song_title, song_path, album_name, artist_name,
     * genre_name
     *
     */
    this->InsertQuery(
        " CREATE TABLE if not exists songMaster("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " device_id INTEGER,"
        " song_path TEXT,"
        " song_title TEXT,"
        " album_name TEXT,"
        " artist_name TEXT,"
        " genre_name TEXT,"
        "folder_id INTEGER,"
        "item_mode INTEGER,"
        "art_avl INTEGER,"
        "art_path TEXT,"
        "FOREIGN KEY(device_id) REFERENCES deviceMaster(device_id) ON DELETE CASCADE);");

    return 0;
}

/*!
 * @brief this function execute database query by calling
 * InsertQuery(std::string l_query) method of DataBasePrivate
 * @return success or failure status
 */
int DataBase::InsertQuery(std::string l_query) {
    char* queryError = 0;
    // Execute SQL statement
    // std::cout << "\n QUERY : " << l_query;
    int rc = sqlite3_exec(DB, l_query.c_str(), 0, 0, &queryError);

    if (rc != SQLITE_OK) {
        std::cout << "SQL Erremplace_backor : " << queryError << "\n";
        sqlite3_free(queryError);
        return -1;

    } else {
        std::cout << "successfully  songs_metadata\n";
        return 1;
    }

    return 0;
}
}  // namespace media
}  // namespace kivi
