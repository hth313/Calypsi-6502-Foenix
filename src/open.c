#include <microkernel/api.h>
#include <stdbool.h>
#include <string.h>
#include <calypsi/stubs.h>
#include <calypsi/intrinsics6502.h>

int
_Stub_open(const char *fname, int mode, ...) {
  int ret = 0;
  char drive;

  fname = __path_without_drive(fname, &drive);

  args.common.buf = (uint8_t*) fname;
  args.common.buflen = strlen(fname);
  args.file.open.drive = drive;
  if (mode == 1) {
    mode = 0;
  } else {
    mode = 1;
  }
  args.file.open.mode = mode;
  if (__kernel_call_failed(_MicroKernelCall(File.Open)())) {
    return -1;
  }

  for(;;) {
    event.type = 0;
    _MicroKernelCall(NextEvent)();
    if (event.type == EVENT(file.OPENED)) {
      return ret;
    }
    if (event.type == EVENT(file.NOT_FOUND)) {
      return -1;
    }
    if (event.type == EVENT(file.ERROR)) {
      return -1;
    }
  }
}
