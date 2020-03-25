/*
 * mediascanner.h
 *
 *  Created on: 08-Apr-2019
 *      Author: ashish
 */

#ifndef MEDIASCANNER_H_
#define MEDIASCANNER_H_

#include </usr/local/usr/include/kivi/osal/thread.h>
#include </usr/local/usr/include/kivi/osal/wait_object.h>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <vector>
#include "../database/database.h"
#include "../mediatypes.h"
#include "CoverartExtractor.h"

using namespace std::chrono;
using namespace std;

namespace kivi {
namespace media {

/*!
 * @class MediaScanner
 * @brief it realizes IMediaScannerHandler and used as implementation class of
 * \MediaScanning
 *
 */
class MediaScanner {
   public:
    MediaScanner();
    virtual ~MediaScanner();

    MediaScanner(MediaScanner&&);
    MediaScanner& operator=(MediaScanner&&);
    /*!
     * @brief it initializes Scanning parameters
     * @param No parameters
     * @return media_error_t: returns respective media errors occurs
     */
    void Initialize(IBatchCallBack*);
    /*!
     * @brief This function scan all the songs with specified formats.
     * @param device_paths_t: device path to scan
     * @return media_error_t: returns respective media errors occurs
     */
    void StartScanning(device_paths_t);
    /*!
     * @brief It will just stop scanning of media while disconnecting device or
     * @brief any other media error occurs.
     * @param No parameter
     * @return media_error_t: returns respective media errors occurs
     */
    void CancelScanning();

    void ScanDevice(std::string, int);

    audio_format_t getFormat(std::string);
    /*!
     * @brief protected constructor and destructor for singleton object reference.
     */

   public:
    // TODO : Create Some provision to resolve total count
    static int totalFilesScanned;

    // Only to hold tester thread ...... NO use in kmediaengine
    static bool isIndexingCompleted;
    IBatchCallBack* m_asyncall;

   private:
    int m_batchSize;
    songs_path m_batchPathVect;
    int count;
    std::mutex m_reqMutex;
    /*!
     * @brief singleton instance reference of MediaScanner
     */
    static MediaScanner* theInstance;
};
}  // namespace media
}  // namespace kivi
#endif /* MEDIASCANNER_H_ */
