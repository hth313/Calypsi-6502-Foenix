#include <stdio.h>
#undef EOF
#include <errno.h>
#include "calypsi/intrinsics6502.h"
#include <calypsi/stubs.h>
#include <tinycore/api.h>

long _Stub_lseek(int fd, long offset, int whence) {
  if (whence != SEEK_SET) {
    // The TinyCore API only allows for absolute positioning
    // at the moment.
    __set_errno(ENOTSUP);
    return -1;
  }

  args.file.seek.stream = fd;
  args.file.seek.position = offset;

  if (__tinycore_call_failed(_TinyCoreCall(File.Seek)())) {
      __set_errno(EIO);
      return -1;
    }

  while (1) {
    event.type = 0;
    _TinyCoreCall(NextEvent)();
      switch (event.type) {
      case EVENT(file.SEEK):
	return args.file.seek.position;
      case EVENT(file.ERROR):
      __set_errno(EIO);
      return -1;
      }
  }
}


