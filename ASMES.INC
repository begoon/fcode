ExecInt:        pusha
                cli
                mov     emSP, sp
                mov     sp, offset Registers
                popa
                mov     sp, emSP
                sti
IntCode:        int     0
                cli
                mov     Tmp$, sp
                mov     sp, offset Registers+8*2+2
                pushf
                pusha
                mov     sp, Tmp$
                mov     emSP, sp
                sti
                popa
                next

osDP$:          push    sp
                next

osRP$:          push    RP$
                next

; Adc  ( a b -> c isCarry )
; if a+b>FFFF isCarry = FFFF else isCarry=0

osAdc$:         pop     ax  dx          ; -> a b
                add     ax, dx
                sbb     dx, dx
                push    ax  dx          ; c isCarry ->
                next

; osSwap ( a b -> b a ) Crash AX, BX

osSwap$:        pop      ax bx
                push     ax bx
                NEXT

; osRot ( a b c -> b c a ) Crash AX, BX, CX

osRot$:         pop      ax bx cx
                push     bx ax cx
                NEXT

osInt$:         pop     ax
                mov     byte ptr IntCode+1, al
                jmp     short ExecInt

osPut$:         add     RP$, 2
                mov     bp, RP$
                pop     word ptr [bp]
                NEXT

osGet$:         mov     bp, RP$
                push    word ptr [bp]
                sub     RP$, 2
                NEXT

osDrop$:        add     sp, 2
                NEXT

; osNor ( a b -> a NOR b ) Crash AX, BX

osNor$:         pop     ax bx
                or      ax, bx
                not     ax
                push    ax
                NEXT

osTrap$:        int     3
                NEXT

; osPeek ( addr -> value ) Crash BX

osPeek$:        pop     bx
                push    word ptr [bx]
                NEXT

; osPoke ( Value Addr -> ) Crash BX

osPoke$:        pop     bx              ; -> Value Addr
                pop     word ptr [bx]   ; ->
                NEXT
