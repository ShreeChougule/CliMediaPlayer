/*
 * mediaindexerimpl.cpp
 *
 *  Created on: 07-Apr-2019
 *      Author: ashish
 */

#include "usbindexer.h"
#include <unistd.h>

namespace kivi {
namespace media {

UsbIndexer::UsbIndexer()
    : m_scanner(new MediaScanner()),
      m_reader(new MetaDataReader()),
      m_batchCount(0) {}

UsbIndexer::~UsbIndexer() {
  delete m_scanner;
  delete m_reader;
}

void UsbIndexer::Initialize() {
  // TODO :
  m_scanner->Initialize(m_reader);
  m_reader->m_dataBase->OpenDataBase(m_reader);
}

void UsbIndexer::StartIndexing(device_paths_t devicepath, int batch_size) {
  Initialize();
  m_devicepath = devicepath;
  m_batchCount = batch_size;

  this->Start();
  // Holding thread .... NO use in kmediaemgine
  while (1) {
    if (MetaDataReader::isIndexingComplete) {
      m_reader->CalculateTimeTaken(m_start_time);
      return;
    } else {
      m_waitRead.Wait(500);
    }
  }
}

long int UsbIndexer::ThreadRun() {
  m_start_time = high_resolution_clock::now();
  m_reader->Start();
  m_scanner->ScanDevice(m_devicepath, m_batchCount);
  return 0;
}

void UsbIndexer::CancelIndexing() {}  // namespace media
}  // namespace media
}  // namespace kivi
