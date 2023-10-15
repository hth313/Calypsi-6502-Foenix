#include <microkernel/api.h>
#include "calypsi/intrinsics6502.h"
#include <calypsi/stubs.h>
#include <errno.h>

static char getin(void) {
  while (1) {
    if (__kernel_call_failed(_MicroKernelCall(NextEvent)())) {
      // No event pending, give kernel time to do other things.
      _MicroKernelCall(Yield);
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
  }

  // Limit number of bytes to request
  if (count > 256) {
    count = 256;
  }

  args.file.read.stream = fd;
  args.file.read.buflen = count;

  if (__kernel_call_failed(_MicroKernelCall(File.Read)())) {
    __set_errno(EIO);
    return 0;
  }

  while (1) {
    event.type = 0;
    _MicroKernelCall(NextEvent)();
    switch (event.type) {
    case EVENT(file.DATA):
	args.common.buf = buf;
      args.common.buflen = event.file.data.delivered;
      _MicroKernelCall(ReadData)();
	return event.file.data.delivered;
    case EVENT(file.EOF):
	return 0;
    case EVENT(file.ERROR):
	__set_errno(EIO);
	return 0;
    }
  }
}
