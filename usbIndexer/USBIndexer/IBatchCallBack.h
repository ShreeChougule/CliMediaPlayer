#include <stdio.h>
#include "../mediatypes.h"
namespace kivi {
namespace media {
class IBatchCallBack {
   public:
    virtual ~IBatchCallBack() {}
    virtual void OnBatchComplete(songs_path&&) = 0;
    virtual void OnScanningComplete() = 0;
    virtual void OnIndexingComplete() = 0;
};
}  // namespace media
}  // namespace kivi
