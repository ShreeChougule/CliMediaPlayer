/*
 * mediaindexerimpl.h
 *
 *  Created on: 07-Apr-2019
 *      Author: ashish
 */

#ifndef USBINDEXER_H_
#define USBINDEXER_H_

#include "../mediatypes.h"
#include "metadatareader.h"

namespace kivi {
namespace media {

class UsbIndexer : public thread_t {
   public:
    UsbIndexer();
    virtual ~UsbIndexer();
    static UsbIndexer* InstancePtr();

    void Initialize();
    void StartIndexing(device_paths_t, int);
    void CancelIndexing();
    long int ThreadRun();

   public:
    high_resolution_clock::time_point m_start_time;
    MetaDataReader* m_reader;

   private:
    device_paths_t m_devicepath;
    int m_batchCount;
    std::mutex m_reqMutex;
    wait_object_t m_waitRead;
    MediaScanner* m_scanner;
};
}  // namespace media
}  // namespace kivi
#endif /* DBUS_INDEXER_MEDIAINDEXERIMPL_H_ */
