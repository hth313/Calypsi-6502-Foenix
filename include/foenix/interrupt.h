// Foenix C256 Junior interrupt controller
//

#ifndef __FOENIX_JR_INTERRUPT_H__
#define __FOENIX_JR_INTERRUPT_H__

#include <stdint.h>

typedef struct InterruptController {
  union {
    struct {
      uint8_t vicky_int0   : 1;
      uint8_t vicky_int1   : 1;
      uint8_t ps2_keyboard : 1;
      uint8_t ps2_mouse    : 1;
      uint8_t timer0       : 1;
      uint8_t timer1       : 1;
      uint8_t dma          : 1;
      uint8_t              : 1;
    };
    uint8_t part0;
  };
  union {
    struct {
      uint8_t uart         : 1;
      uint8_t vicky_int2   : 1;
      uint8_t vicky_int3   : 1;
      uint8_t vicky_int4   : 1;
      uint8_t rtc          : 1;
      uint8_t via          : 1;
      uint8_t iec          : 1;
      uint8_t sd_insert    : 1;
    };
    uint8_t part1;
  };
} InterruptController_t;

#define InterruptPending (* (volatile InterruptController_t*) 0xdd60)
#define InterruptMask    (* (volatile InterruptController_t*) 0xdd66)

#endif // __FOENIX_JR_INTERRUPT_H__
