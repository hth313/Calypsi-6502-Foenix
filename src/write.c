#include <tinycore/api.h>
#include "calypsi/intrinsics6502.h"
#include <calypsi/stubs.h>
#include <errno.h>
#include <stdint.h>

// Just hard-coded for now.
#define MAX_ROW 60
#define MAX_COL 80

uint8_t _Row = 0;
uint8_t _Col = 0;
static char *line = (char*) 0xc000;

extern void _ClearScreen(void);

static void scroll() {
  int i;
  char *vram = (char*)0xc000;

  *(char*)1 = 2;

  for (i = 0; i < 80*59; i++) {
    vram[i] = vram[i+80];
  }
  vram += i;
  for (i = 0; i < 80; i++) {
    *vram++ = 32;
  }
}

static void out(char c) {
  switch (c) {
  case 12:
    _ClearScreen();
    line = (char*) 0xc000;
    break;
  default:
    *(char*)1 = 2;
      line[_Col] = c;
    _Col++;
    if (_Col != MAX_COL) {
      break;
    }
  case 10:
  case 13:
    _Col = 0;
    _Row++;
    if (_Row == MAX_ROW) {
      scroll();
      _Row--;
      break;
    }
    line += 80;
    break;
  }

  *(char *)1 = 0;
  *(int *)0xd016 = _Row;
  *(int *)0xd014 = _Col;
}

size_t _Stub_write(int fd, const void *buf, size_t count) {
  if (fd == 1 || fd == 2) {
    char *text = (char *)buf;
    for (size_t i = count; i > 0; i--) {
      out(*text);
      text++;
    }
    return count;
  }

  size_t total = 0;
  while (count) {
    size_t n = count > 254 ? 254 : count;
    if (__tinycore_call_failed(_TinyCoreCall(File.Write)())) {
      // Failure means either out of event descriptors or bad
      // descriptor. Take a guess it is a bad descriptor.
      __set_errno(ENXIO);
      return -1;
    }

    while (1) {
      event.type = 0;
      _TinyCoreCall(NextEvent)();
      if (event.type == EVENT(file.WROTE)) {
        size_t delivered = event.file.wrote.delivered;
        total += delivered;
	count -= delivered;
      }
    }
  }

  return total;
}
