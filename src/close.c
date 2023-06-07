#include <tinycore/api.h>
#include "calypsi/intrinsics6502.h"

int _Stub_close(int fd) {
  args.file.close.stream = fd - 3;
  int result = - __tinycore_call_failed(_TinyCoreCall(File.Close)());
  do {
    event.type = 0;
    _TinyCoreCall(NextEvent)();
  } while (event.type != EVENT(file.CLOSED));
  return result;
}
