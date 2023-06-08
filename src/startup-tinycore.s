;;; Startup variant, change attribute value if you make your own
              .rtmodel cstartup,"tinycore"

              .rtmodel version, "1"

              .rtmodel core, "*"

              .section stack
              .section cstack
              .section heap
              .section data_init_table

              .extern main, exit, event
              .extern _Zp, _Vsp, _Vfp

	      ;; TinyCore argument area
args:	      .equ    0xf0

;;; ***************************************************************************
;;;
;;; __program_start - actual start point of the program
;;;
;;; Set up CPU stack, C stack, initialize sections and call main().
;;; You can override this with your own routine, or tailor it as needed.
;;; The easiest way to make custom initialization is to provide your own
;;; __low_level_init which gets called after stacks have been initialized.
;;;
;;; ***************************************************************************

              .section code, noreorder
              .pubweak __program_start
__program_start:
__program_root_section:
	      tsx		    ; save original stack pointer for later exit()
	      stx     __saved_sp

	      lda     #.byte0(.sectionEnd cstack)   ; set up the C stack
              sta     zp:_Vsp
              lda     #.byte1(.sectionEnd cstack)
              sta     zp:_Vsp+1
              jsr     __low_level_init

;;; **** Initialize data sections if needed.
              .section code, noroot, noreorder
              .pubweak __data_initialization_needed
              .extern __initialize_sections
__data_initialization_needed:
              lda     #.byte0 (.sectionStart data_init_table)
              sta     zp:_Zp
              lda     #.byte1 (.sectionStart data_init_table)
              sta     zp:_Zp+1
              lda     #.byte0 (.sectionEnd data_init_table)
              sta     zp:_Zp+2
              lda     #.byte1 (.sectionEnd data_init_table)
              sta     zp:_Zp+3
              jsr     __initialize_sections

;;; **** Initialize streams if needed.
              .section code, noroot, noreorder
              .pubweak __call_initialize_global_streams
              .extern __initialize_global_streams
__call_initialize_global_streams:
              jsr     __initialize_global_streams

;;; **** Initialize heap if needed.
              .section code, noroot, noreorder
              .pubweak __call_heap_initialize
              .extern __heap_initialize, __default_heap
__call_heap_initialize:
              lda     #.byte0 __default_heap
              sta     zp:_Zp+0
              lda     #.byte1 __default_heap
              sta     zp:_Zp+1
              lda     #.byte0 (.sectionStart heap)
              sta     zp:_Zp+2
              lda     #.byte1 (.sectionStart heap)
              sta     zp:_Zp+3
              lda     #.byte0 (.sectionSize heap)
              sta     zp:_Zp+4
              lda     #.byte1 (.sectionSize heap)
              sta     zp:_Zp+5
              jsr     __heap_initialize

              .section code, root, noreorder
	      lda     #.byte0 event ; set up event buffer for TinyCore
	      sta     zp:args

#ifdef __CALYPSI_CORE_65C02__
	      stz     zp:_Zp	    ; argc = 0
	      stz     zp:_Zp+1
#else
	      lda     #0
	      sta     zp:_Zp	    ; argc = 0
	      sta     zp:_Zp+1
#endif
              jsr     main
              jmp     exit

;;; ***************************************************************************
;;;
;;; __low_level_init - custom low level initialization
;;;
;;; This default routine just returns doing nothing. You can provide your own
;;; routine, either in C or assembly for doing custom low leve initialization.
;;;
;;; ***************************************************************************

              .section code
              .pubweak __low_level_init
__low_level_init:
              rts

;;; ***************************************************************************
;;;
;;; _Stub_exit - this is where programs exits back to the kernel.
;;;
;;; ***************************************************************************

	      .section code
	      .public _Stub_exit
_Stub_exit:   ldx     __saved_sp
	      tsx
	      rts

	      .section znear, bss
__saved_sp:   .space  1
