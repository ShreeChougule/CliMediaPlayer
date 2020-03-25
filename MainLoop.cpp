#include <iostream>
#include "DbHandler/DbHandler.h"
#include "GstreamerLocal/GstreamerImpl.h"
#include "usbIndexer/USBIndexer/usbindexer.h"

using namespace kivi::media;

int main(int args, char* argv[]) {
  wait_object_t m_waitRead;
  UsbIndexer* m_usbindexer = new UsbIndexer();
  GstreamerImpl* player = new GstreamerImpl();
  DbHandler* Handle = new DbHandler();

  m_usbindexer->StartIndexing(argv[1], 10);

  player->Initialize();
  player->Start();
  m_waitRead.Wait(100);

  int ch;
  do {
    std::cout
        << "\n\t#### MENU ####\n 1)Previous\n 2) Play / Pause\n 3) Next\n "
           "4)Exit Player \n\t Enter Action = ";
    std::cin >> ch;

    switch (ch) {
      case 1:
        player->Previous();
        m_waitRead.Wait(100);
        break;
      case 2:
        player->PlayPause();
        m_waitRead.Wait(100);

        break;
      case 3:
        player->Next();
        m_waitRead.Wait(100);

        break;
      case 4:
        std::cout << "\nExiting Player !\n\n";
        player->Stop_Player();
        break;
      default:
        std::cout << "\nERROR : INVALID ACTION !";
    }
  } while (ch != 4);

  m_usbindexer->m_reader->removeArtworkData();
  return 0;
}
