/*
 * database.h
 *
 *  Created on: 09-Apr-2019
 *      Author: kpit
 */

#ifndef DBUS_DATABASE_DATABASE_H_
#define DBUS_DATABASE_DATABASE_H_

#include </usr/local/usr/include/kivi/osal/thread.h>
#include </usr/local/usr/include/kivi/osal/wait_object.h>
#include <sqlite3.h>
#include <list>
#include <mutex>
#include <string>
#include <vector>
#include "../USBIndexer/IBatchCallBack.h"

// kivi namespace
namespace kivi {

// media namespace
namespace media {

/*!
 * @class Database and this is publicly inherited from IDatabase
 * @brief This class is abstract replica of actual database.
 * @brief To use as API interface
 */
class DataBase : public thread_t {
   public:
    /*!
     *  Constructor
     */
    DataBase();

    /*!
     *  virtual Destructor
     */
    virtual ~DataBase();

   public:
    /*!
     *  @fn void OpenDataBase()
     *  @brief A member function for open database
     *  @param no any parameter
     *  @return no any return value.
     */
    void OpenDataBase(IBatchCallBack*);

    /*!
     *  @fn void CloseDataBase()
     *  @brief A member function to close database
     *  @param no any parameter
     *  @return no any return value.
     */
    void CloseDataBase();

    /*!OpenDataBase
     *  @fn void RemoveDevice()  This is TODO
     *  @brief A member function to after remove device close database
     *  @param no any parameter
     *  @return no any return value.
     */
    void RemoveDevice();
    void NotifyScanReadComplete(bool);

    void Enqueue(songs_metadata&&);
    /*!
     *  @fn void InitDB()
     *  @brief A member function for create database tables and triggers
     *  @param no any parameter
     *  @return return int value to check success or fail query.
     */
    int InitDB();
    long int ThreadRun();
    void StartWritingToDB();
    void ReadDataObjVec(songs_metadata&);

    /*!
     *  @fn void InsertQuery()
     *  @brief A member function used for to execute database query
     *  @param std::string type parameter required as a database query
     *  @return return int value to check success or fail database query.
     */
    int InsertQuery(std::string);

   public:
    std::list<songs_metadata> m_metadataVec;

   private:
    sqlite3* DB;          // Database object used to access database
    const char* db_path;  // Database file path location
    bool m_waiting;
    std::mutex m_reqMutex;
    wait_object_t m_waitRead;
    bool isScanningComplete;
    IBatchCallBack* async;
};

}  // namespace media
}  // namespace kivi

#endif /* DBUS_DATABASE_DATABASE_H_ */
