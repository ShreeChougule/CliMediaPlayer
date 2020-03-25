#include "USBIndexer/usbindexer.h"

using namespace kivi::media;
int main(int args, char* argv[]) {
    UsbIndexer* m_usbindexer = new UsbIndexer();
    m_usbindexer->StartIndexing(argv[1], 10);

    char ch;
    while (ch != 'y') {
        std::cout << "\n Clear Coverarts ?? (y/n) = ";
        std::cin >> ch;

        if (ch == 'y') m_usbindexer->m_reader->removeArtworkData();
        if (ch == 'n') break;
    }
    return 0;
}
