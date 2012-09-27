; ---------------------------------------------------------------------------
; osPick ( Index -> Value )
; Get the indexed element from data stack
; ... z y x Index -> Value
; └┬┘ │ │ └──────── 0       ┐
;  │  │ └────────── 1       │  Examples of the Index values
;  │  └──────────── 2       ┘
;  └─────────────── etc ...

osPick$:        CALLR                   ; -> Index
                osDup                   ; Index Index
                osAdd                   ; Index*2
                osDP                    ; Index*2 DP
                osAdd                   ; Index*2+DP
                ofAdd   2               ; Index*2+DP+2
                osPeek                  ; Value ->
                RETR

; ---------------------------------------------------------------------------
; osDup ( x -> x x )
; Duplicate the top element on the data stack
; Warning: Can't be defined by 'osOver', because osOver uses 'osDup' itself.

osDup$:         CALLR
                osDP
                osPeek
                RETR

; ---------------------------------------------------------------------------
; os2Dup ( x y -> x y x y )
; Duplicate the double top element on the data stack
; Warning: Defined by 'osOver', because osOver doesn't use 'os2Dup' itself.

os2Dup$:        CALLR
                osOver
                osOver
                RETR

; ---------------------------------------------------------------------------
; osOver ( x y -> x y x )
; Get the data stack element which is under the stack top

osOver$:        CALLR
                ofPick  1
                RETR

; ---------------------------------------------------------------------------
; osAdd ( x y -> x+y )
; Add two top elements

osAdd$:         CALLR                   ; x y ->
                osAdc                   ; Sum isCarry
                osDrop                  ; Sum
                RETR                    ; ->

; ---------------------------------------------------------------------------
; osInc ( x -> x+1 )
; Increment of the top element

osInc$:         CALLR
                ofPush  1
                osAdd
                RETR

; ---------------------------------------------------------------------------
; osXor ( x y -> x^y )
; Xor by x^y=(x&!y)|(!x&y)

osXor$:         CALLR                   ; x y
                os2Dup                  ; x y x y
                osNot                   ; x y x !y
                osAnd                   ; x y x&!y
                osSwap                  ; x x&!y y
                ofPick  2               ; x x&!y y x
                osNot                   ; x x&!y y !x
                osAnd                   ; x x&!y y&!x
                osOr                    ; x (x&!y)|(y&!x)
                osSwap                  ; (x&!y)|(y&!x) x
                osDrop                  ; (x&!y)|(y&!x)
                RETR

osNot$:         CALLR
                osDup
                osNor
                RETR

osAnd$:         CALLR                   ; -> x y
                osNot                   ; x !y
                osSwap                  ; !y x
                osNot                   ; !y !x
                osOr                    ; !y|!x
                osNot                   ; ->
                RETR

osOr$:          CALLR
                osNor
                osNot
                RETR

osNeg$:         CALLR
                osNot
                osInc
                RETR

osDos$:         CALLR
                ofToAX  4C00h
                ofInt   21h
                RETR

osSub$:         CALLR
                osNeg
                osAdd
                RETR

; osIs0? ( value -> isCarry )
; if value=0 isCarry=0 else isCarry=FFFF

osIs0$:         CALLR                   ; -> Value
                ofAdc   0FFFFh          ; Value-1 isCarry
                osSwap                  ; isCarry Value-1
                osDrop                  ; isCarry ->
                RETR

; Jnz - ( Cond Addr -> )

osJnz$:         CALLR                   ; -> Cond Addr
                osSwap                  ; Addr Cond
                osIs0                   ; Addr Sign
                osDup                   ; Addr Sign Sign
                osNot                   ; Addr Sign !Sign
                osRP                    ; Addr Sign !Sign RP
                osPeek                  ; Addr Sign !Sign IP
                osAnd                   ; Addr Sign !Sign&IP
                osSwap                  ; Addr !Sign&IP Sign
                ofPick  2               ; Addr !Sign&IP Sign Addr
                osAnd                   ; Addr !Sign&IP Sign&Addr
                osOr                    ; Addr !Sign&IP|Sign&Addr
                osSwap                  ; !Sign&IP|Sign&Addr Addr
                osDrop                  ; !Sign&IP|Sign&Addr
                osRP                    ; !Sign&IP|Sign&Addr RP
                osPoke                  ; ->
                RETR

osDo$:          CALLR                   ; -> Start End
                osGet                   ; Start End RetAddr
                osSwap                  ; Start RetAddr End
                osPut                   ; Start RetAddr
                osSwap                  ; RetAddr Start
                osPut                   ; RetAddr
                osPut                   ; ->
                RETR

osI$:           CALLR                   ; ->
                osGet                   ; RetAddr
                osGet                   ; RetAddr Start
                osDup                   ; RetAddr Start Start
                osPut                   ; RetAddr Start
                osSwap                  ; Start RetAddr
                osPut                   ; Start ->
                RETR

osStep$:        CALLR                   ; -> Step
                osGet                   ; Step RetAddr
                osSwap                  ; RetAddr Step
                osGet                   ; RetAddr Step Start
                osAdd                   ; RetAddr Start+Step
                osGet                   ; RetAddr Start End
                os2Dup                  ; RetAddr Start End Start End
                osPut                   ; RetAddr Start End Start
                osPut                   ; RetAddr Start End
                osCmp                   ; RetAddr isCarry isZero
                osNot                   ; RetAddr isCarry !isZero
                osAnd                   ; RetAddr ?
                osSwap                  ; ? RetAddr
                osPut                   ; ? ->
                RETR

osChar$:        CALLR                   ; -> Char
                osToAX                  ;
                ofInt   29h             ; ->
                RETR

osStr$:         CALLR
                osToDX
                ofToAX  0900h
                ofInt   21h
                RETR

osPeekb$:       CALLR                   ; -> Addr
                osPeek                  ; Value
                ofPush  0FFh            ; Value Mask
                osAnd                   ; Lo ->
                RETR

osPokeb$:       CALLR                   ; -> Value Addr
                osDup                   ; Value Addr Addr
                osPeek                  ; Value Addr Old
                ofPush  0FF00h          ; Value Addr Old 0FF00
                osAnd                   ; Value Addr Masked
                ofPick  2               ; Value Addr Masked Value
                ofPush  0FFh            ; Value Addr Masked Value 0FF
                osAnd                   ; Value Addr Masked Byte
                osOr                    ; Value Addr Word
                osSwap                  ; Value Word Addr
                osPoke                  ; Value
                osDrop                  ; ->
                RETR

osExch$:        CALLR                   ; -> Value
                osDP                    ; Value DP
                osDup                   ; Value DP DP
                osPeekb                 ; Value DP L
                osOver                  ; Value DP L DP
                osInc                   ; Value DP L DP+1
                osPeekb                 ; Value DP L H
                ofPick  2               ; Value DP L H DP
                osPokeb                 ; Value DP L
                osSwap                  ; Value L DP
                osInc                   ; Value L DP+1
                osPokeb                 ; Value ->
                RETR

osRcl$:         CALLR                   ; -> Value Carry
                osIs0                   ; Value TrueCY
                osNeg                   ; Value !TrueCY
                osSwap                  ; !TrueCY Value
                osShl                   ; TrueCY Value*2 isCarry
                osRot                   ; Value*2 isCarry !TrueCY
                osRot                   ; isCarry TrueCY Value*2
                osOr                    ; isCarry TrueCY|Value*2
                osSwap                  ; !TrueCY|Value*2 isCarry
                RETR

osShl$:         CALLR                   ; -> Value
                osDup                   ; Value Value
                osAdc                   ; Value*2 isCY ->
                RETR

; osCmp$ ( x y -> x y isCarry isZero) - compare
;   if x=y isZero=FFFF else (x<>y) isZero=0
;   if x<y isCarry=FFFF else (x>=y) isCarry=0

osCmp$:         CALLR                   ; x y ->
                osNeg                   ; x !y
                osSwap                  ; !y x
                osAdc                   ; Sum isCarry
                osNot                   ; Sum !isCarry
                osSwap                  ; !isCarry Sum
                osIs0                   ; !isCarry isZero ->
                osNot
                RETR

osToAX$:        CALLR
                ofPoke  *, emAX
                RETR

osToDX$:        CALLR
                ofPoke  *, emDX
                RETR
