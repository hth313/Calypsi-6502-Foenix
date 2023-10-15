#include <microkernel/api.h>
#include "calypsi/intrinsics6502.h"
#include <calypsi/stubs.h>

int _Stub_close(int fd) {
  args.file.close.stream = fd;
  int result = - __kernel_call_failed(_MicroKernelCall(File.Close)());
  do {
    event.type = 0;
    _MicroKernelCall(NextEvent)();
  } while (event.type != EVENT(file.CLOSED));
  return result;
}
