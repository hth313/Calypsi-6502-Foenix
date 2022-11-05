// F256 Junior Tiny Vicky
//

#ifndef __FOENIX_JR_TINY_VICKY_H__
#define __FOENIX_JR_TINY_VICKY_H__

#include <stdint.h>

typedef struct vicky vicky_t;

#define Vicky (* (volatile vicky_t*) 0xd000)

typedef struct color {
  uint8_t blue;
  uint8_t green;
  uint8_t red;
} color_t;

// Flat video ram address 0 - 256K
typedef struct vram {
  uint8_t* base;
  uint8_t  bank;
} vram_ptr;

// ----------------------------------------------------------------------
//
// Vicky master control
//
// ----------------------------------------------------------------------

struct vicky {
  union {                                 // Master control
    struct {
      uint8_t text_enable         : 1;
      uint8_t text_overlay_enable : 1;
      uint8_t graphics_enable     : 1;
      uint8_t bitmap_enable       : 1;
      uint8_t tilemap_enable      : 1;
      uint8_t sprites_enbale      : 1;
      uint8_t gamma_enable        : 1;

      uint8_t resolution          : 1;
      uint8_t double_pixel_x      : 1;    // text mode only
      uint8_t double_pixel_y      : 1;    // text mode only
      uint8_t                     : 5;
    };
    uint16_t  master_control;
  };
  struct {                                // Layer manager
    uint8_t layer0 : 3;
    uint8_t        : 1;
    uint8_t layer1 : 3;
    uint8_t        : 1;
  };
  struct {
    uint8_t layer2 : 3;
    uint8_t        : 5;
  };
  struct {                                // Border control
    uint8_t border_enable : 1;
    uint8_t               : 3;
    uint8_t border_scroll : 3;
    uint8_t               : 1;
  };
  color_t border_color;
  uint8_t border_width;                   // 0 - 31
  uint8_t border_height;                  // 0 - 31
  uint8_t _reserved0;
  uint8_t _reserved1;
  uint8_t _reserved2;
  color_t background_color;               // Background color
  struct {                                // Cursor control
    uint8_t cursor_enable       : 1;
    uint8_t cursor_flash_rate   : 2;
    uint8_t cursor_flash_enable : 1;
  };
  uint8_t _reserved3;
  char cursor_char;
  uint8_t _reserved4;
  uint16_t cursor_x;
  uint16_t cursor_y;
  union {
    struct {                              // Write: Line interrupt
      struct {
        uint8_t line_interrupt_enable : 1;
      };
      uint16_t line_interrupt_line;
      uint8_t _reserved5;
    };
    struct {                              // Read: Pixel/line position
      uint16_t pixel_position;            // 0 - 799 (includes blanking)
      uint16_t active_line;               // 0 - 525 / 0 - 449 (includes blanking)
    };
  };
};

// Resolution enums
enum vicky_text_resolution {
  Text_640x480_60Hz, Text_640x480_70Hz
};

enum vicky_graphics_resolution {
  Graphics_320x240_60Hz, Graphics_320x200_70Hz
};

// Layers
enum vicky_layer {
  Layer_Bitmap0, Layer_Bitmap1,
  Layer_Timemap0, Layer_Timemap1, Layer_Timemap2
};

enum vicky_cursor_flash_rate {
  CursorFlash_1s, CursorFlash_0_5s, CursorFlash_0_25s, CursorFlash_0_20s
};

// ----------------------------------------------------------------------
//
// Bitmap
//
// ----------------------------------------------------------------------

typedef struct bitmap {
  struct {
    uint8_t enable : 1;
    uint8_t lut    : 2;
    uint8_t        : 5;
  };
  vram_ptr data;
  uint8_t _reserved[4];
} bitmap_t;

// There are two bitmap layers, use them as Bitmap[n].field
// where n is 0-1.
#define Bitmap ((bitmap_t *)0xd100)

// ----------------------------------------------------------------------
//
// Tilemap
//
// ----------------------------------------------------------------------

typedef struct tilemap {
  union {
    struct {
      uint8_t enable    : 1;
      uint8_t           : 3;
      uint8_t tile_size : 1;
      uint8_t           : 3;
    };
    uint8_t control;
  };
  vram_ptr data;
  uint8_t x_size;
  uint8_t _reserved0;
  uint8_t y_size;
  uint8_t _reserved1;
  struct {
    uint16_t x_smooth_scroll    : 4;  // 16x16 tiles, not used for 8x8
    uint16_t x_position         : 8;
    uint16_t                    : 3;
    uint16_t x_smooth_direction : 1;
  };
  struct {
    uint16_t y_smooth_scroll    : 4;  // 16x16 tiles, not used for 8x8
    uint16_t y_position         : 8;
    uint16_t                    : 3;
    uint16_t y_smooth_direction : 1;
  };
} tilemap_t;

// There are three tilemap layers, use them as Tilemap[n].field
// where n is 0-2.
#define Tilemap ((tilemap_t *)0xd200)

struct vram_ptr32 {
  vram_ptr ptr;
  uint8_t _filler;
};

#define TileAddy ((struct vram_ptr32 *)0xd280)

// ----------------------------------------------------------------------
//
// Sprite
//
// ----------------------------------------------------------------------

typedef struct sprite {
  union {                            // control
    struct {
      uint8_t enable : 1;
      uint8_t lut    : 2;
      uint8_t depth  : 2;
      uint8_t size   : 2;
      uint8_t        : 1;
    };
    uint8_t control;
  };
  vram_ptr data;
  uint16_t x;
  uint16_t y;
} sprite_t;

// There are 64 sprites, use them as Sprite[n].field
// where n is 0-63.
#define Sprite ((sprite_t *)0xd900)

enum sprite_size {
  Sprite_32x32, Sprite_24x24, Sprite_16x16, Sprite_8x8
};


#endif // __FOENIX_JR_TINY_VICKY_H__
