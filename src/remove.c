#include <string.h>
#include <calypsi/stubs.h>
#include "calypsi/intrinsics6502.h"
#include <microkernel/api.h>

int _Stub_remove(const char *path) {
  char drive, stream;

  path = __path_without_drive(path, &drive);
  args.file.delete.drive = drive;
  args.common.buf = path;
  args.common.buflen = strlen(path);

  if (__kernel_call_failed(_MicroKernelCall(File.Delete))) {
    return -1;
  }

  while (1) {
    event.type = 0;
    _MicroKernelCall(NextEvent)();
    if (event.type == EVENT(file.DELETED)) {
      break;
    }
    if (event.type == EVENT(file.ERROR)) {
      return -1;
    }
  }

  return 0;
}
