#include "api.h"
#include <stdbool.h>
#include <string.h>
#include <calypsi/stubs.h>
#include "intrinsics6502.h"

#define args (*(__zpage struct call_args*)0xf0)
#define EVENT(member)  (size_t) (&((struct events*) 0)->member)

extern struct event_t event;
extern bool _kern_NextEvent(void);
extern bool _kern_Open(void);

const char *
_path_without_drive(const char *path, char *drive) {
  char drive_char = path[0];
  if ('0' <= drive_char && drive_char <= '8' && path[1] == ':') {
    *drive = path[0] - '0';
    return path + 2;
  } else {
    *drive = 0;
    return path;
  }
}

int
_Stub_open(const char *fname, int mode, ...) {
  int ret = 0;
  char drive;

  fname = _path_without_drive(fname, &drive);

  args.common.buf = (uint8_t*) fname;
  args.common.buflen = strlen(fname);
  args.file.open.drive = drive;
  if (mode == 1) {
    mode = 0;
  } else {
    mode = 1;
  }
  args.file.open.mode = mode;
  if (__tinycore_call_failed(_kern_Open())) {
    return -1;
  }

  for(;;) {
    event.type = 0;
    _kern_NextEvent();
    if (event.type == EVENT(file.OPENED)) {
      return ret + 3;
    }
    if (event.type == EVENT(file.NOT_FOUND)) {
      return -1;
    }
    if (event.type == EVENT(file.ERROR)) {
      return -1;
    }
  }
}

extern bool kern(void);

int test() {
  if (__tinycore_call_failed(kern())) {
    return 2;
  } else {
    return 4;
  }
}

// 3. TinyCore call
extern _Bool TinyCoreGetTime(void);

int bcd_time() {
  if (__tinycore_call_failed(TinyCoreGetTime())) {
    return -1;
  } else {
    // Handle the time return
    return 0;
  }
}

// We can also get it as a value.
_Bool bcd_time_call() {
  return __tinycore_call_failed(TinyCoreGetTime());
}
