#include <tinycore/api.h>
#include "calypsi/intrinsics6502.h"
#include <calypsi/stubs.h>
#include <errno.h>

static char getin(void) {
  while (1) {
    if (__tinycore_call_failed(_TinyCoreCall(NextEvent)())) {
      // No event pending, give kernel time to do other things.
      _TinyCoreCall(Yield);
    }
    else if (event.type == EVENT(key.PRESSED) && !event.key.flags) {
	// Non Meta key pressed
        return event.key.ascii;
    }
  }
}

size_t _Stub_read(int fd, void *buf, size_t count) {
  if (fd == 0) {
    *(char*)buf = getin();
    return 1;
  } else {

    // Limit number of bytes to request
    if (count > 256) {
      count = 256;
    }

    args.file.read.stream = fd;
    args.file.read.buflen = count;

    if (__tinycore_call_failed(_TinyCoreCall(File.Read)())) {
      __set_errno(EIO);
      return 0;
    }

    while (1) {
      event.type = 0;
      _TinyCoreCall(NextEvent)();
      switch (event.type) {
      case EVENT(file.DATA):
	args.common.buf = buf;
        args.common.buflen = event.file.data.delivered;
        _TinyCoreCall(ReadData)();
	return event.file.data.delivered;
      case EVENT(file.EOF):
	return 0;
      case EVENT(file.ERROR):
	__set_errno(EIO);
	return 0;
      }
    }
  }
}
