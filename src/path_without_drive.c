#include <tinycore/api.h>

const char *
__path_without_drive(const char *path, char *drive) {
  char drive_char = path[0];
  if ('0' <= drive_char && drive_char <= '8' && path[1] == ':') {
    *drive = path[0] - '0';
    return path + 2;
  } else {
    *drive = 0;
    return path;
  }
}
