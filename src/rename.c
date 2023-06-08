#include <stdbool.h>
#include <string.h>
#include "calypsi/intrinsics6502.h"
#include <calypsi/stubs.h>
#include <tinycore/api.h>

static bool
find_name(const char *name, int *offset) {
  int i, pos;

  for (i = pos = 0; name[i]; i++) {
    if (name[i] == '/') {
      pos = i+1;
      if (!name[pos]) {
	// No base name found!
	return false;
      }
    }
  }

  *offset = pos;
  return true;
}

int _Stub_rename(const char *name, const char *to) {
  char drive, drive2, stream;
  int path1, path2;

  name = __path_without_drive(name, &drive);
  to = __path_without_drive(to, &drive2);

  // ensure that the paths match
  if (     drive != drive2
        || !find_name(name, &path1)
        || !find_name(to, &path2)
        || (path1 != path2)
        || (strncmp(name, to, path1) != 0)
        ) {
    return -1;
  }

  to += path2;

  args.file.delete.drive = drive;
  args.common.buf = name;
  args.common.buflen = strlen(name);
  args.common.ext = to;
  args.common.extlen = strlen(to);
  if (__tinycore_call_failed(_TinyCoreCall(File.Rename))) {
    return -1;
  }

  while (1) {
    event.type = 0;
    _TinyCoreCall(NextEvent)();
    if (event.type == EVENT(file.RENAMED)) {
      break;
    }
    if (event.type == EVENT(file.ERROR)) {
      return -1;
    }
  }

  return 0;
}
