#include <string.h>
#include <calypsi/stubs.h>
#include "calypsi/intrinsics6502.h"
#include <tinycore/api.h>

int _Stub_remove(const char *path) {
  char drive, stream;

  path = __path_without_drive(path, &drive);
  args.file.delete.drive = drive;
  args.common.buf = path;
  args.common.buflen = strlen(path);

  if (__tinycore_call_failed(_TinyCoreCall(File.Delete))) {
    return -1;
  }

  while (1) {
    event.type = 0;
    _TinyCoreCall(NextEvent)();
    if (event.type == EVENT(file.DELETED)) {
      break;
    }
    if (event.type == EVENT(file.ERROR)) {
      return -1;
    }
  }

  return 0;
}
