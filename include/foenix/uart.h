// F256 Junior UART
//

#ifndef __FOENIX_JR_UART_H__
#define __FOENIX_JR_UART_H__

#include <stdint.h>

typedef struct uart uart_t;

#define UART (* (volatile uart_t*) 0xd630)

struct uart {
  union {
    uint16_t baud;     // when DLAB=1
    struct {
      union {          // data register, send and receive
        uint8_t data;
        uint8_t rhr;
        uint8_t thr;
      };
      union {          // interrupt enable
        struct {
          uint8_t interrupt_enable_receive      : 1;
          uint8_t interrupt_enable_transmit     : 1;
          uint8_t interrupt_enable_line_status  : 1;
          uint8_t interrupt_enable_modem_status : 1;
        };
        uint8_t ier;
      };
    };
  };
  union {
    union {
      struct {
        uint8_t interrupt_status   : 1;
        uint8_t interrupt_priority : 3;
        uint8_t                    : 2;
        uint8_t fifos_enabled      : 2;     // ????
      };
      uint8_t isr;       // interrupt status (read)
      union {          // when DLAB=1
        uint8_t prescaler;
        uint8_t psd;
      };
    };
    union {
      struct {
        uint8_t fifo_enable         : 1;
        uint8_t reset_fifo_receive  : 1;
        uint8_t reset_fifo_transmit : 1;
        uint8_t dma_mode            : 1;
        uint8_t                     : 2;
        uint8_t receive_trigger     : 2;
      };
      uint8_t fcr;       // FIFO control (write)
    };
  };
  union {
    struct {
      uint8_t word_length          : 2;
      uint8_t stop_bits            : 1;
      uint8_t parity_enable        : 1;
      uint8_t even_parity          : 1;
      uint8_t set_parity           : 1;
      uint8_t set_break            : 1;
      uint8_t divisor_latch_enable : 1;
    };
    uint8_t lcr;         // line control
  };
  union {
    struct {
      uint8_t dtr                  : 1;
      uint8_t rts                  : 1;
      uint8_t op1                  : 1;
      uint8_t op2                  : 1;
      uint8_t loop_back            : 1;
      uint8_t                      : 3;
    };
    uint8_t mcr;         // modem control
  };
  union {
    struct {
      uint8_t receive_data_ready   : 1;
      uint8_t overrun_error        : 1;
      uint8_t parity_error         : 1;
      uint8_t framing_error        : 1;
      uint8_t break_interrupt      : 1;
      uint8_t transmit_hold_empty  : 1;
      uint8_t transmit_empty       : 1;
      uint8_t fifo_data_error      : 1;
    };
    uint8_t lsr;         // line status
  };
  uint8_t msr;         // scratch pad
  uint8_t spr;         // scratch pad
};

#define BAUD_300       0x1472
#define BAUD_600       0x0a39
#define BAUD_1200      0x051d
#define BAUD_2400      0x028e
#define BAUD_4800      0x0147
#define BAUD_9600      0x00a4
#define BAUD_19200     0x0052
#define BAUD_31250     0x0032
#define BAUD_38400     0x0029
#define BAUD_57600     0x001b
#define BAUD_115200    0x000d

#endif // __FOENIX_JR_UART_H__
