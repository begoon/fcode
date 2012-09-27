; ---------------------------------------------------------------------------
; The enviromment for the F-Code programs' execution
; Version 1.0
; Copyright (c) by Alexander Demin
; ---------------------------------------------------------------------------

                .model tiny
                .386
                .code

DataStkSize     equ     1024
RetStkSize      equ     256

                include Macros.inc
                include ExpConds.inc

                IfNDef DEBUG            ; COM-file mode ?
                    org 100h
                Endif
                .StartUp
                IfDef DEBUG             ; Debug EXE-file mode ?
                    push    cs cs
                    pop     ds es
                Endif

                lea     sp, DataStack-2
                mov     RP$, offset RetStack
                mov     IP$, offset Program
                next

IP$             dw      0               ; Instruction Pointer
RP$             dw      0               ; Ret-stack Pointer
Tmp$            dw      0

Registers       label   word
emDI            dw      0
emSI            dw      0
emBP            dw      0
emSP            dw      0
emBX            dw      0
emDX            dw      0
emCX            dw      0
emAX            dw      0
emFlags         dw      0

; ===========================================================================
; F-Code Address Interpreter

; Get next link

GetNext$:       cld
                mov     si, IP$
                lodsw
                mov     IP$, si
                retn

; Execute CALL-intruction

CALLR$:         add     RP$, 2
                mov     bp, RP$
                mov     ax, IP$
                mov     [bp], ax
                pop     word ptr IP$
                next

; Execute RET-intruction

RETR$:          mov     bp, RP$
                mov     ax, [bp]
                mov     IP$, ax
                sub     RP$, 2
                next

; Execute next link

NEXT$:          call    GetNext$
                jmp     ax

; Push the next link in the data-stack

osPush$:        call    GetNext$
                push    ax
                next

; ===========================================================================

                Include Asmes.inc       ; Assembler's routines

                Include FProc.inc       ; F-Code routines

                Include FTest.Pre       ; User's program

RetStack        label   word
                org     $+RetStkSize*2
StackLimit      label   word
                org     $+(DataStkSize-1)*2
DataStack       label   word
                org     $+2
Buffer          label   byte

                end
