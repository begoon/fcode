; ---------------------------------------------------------------------------
; Test program on F-Code
; Version 1.0
; One of the correct passwords is 'FRaG'
; Copyright (c) 1996 by Alexander Demin
; ---------------------------------------------------------------------------

                include XorStr.inc

TrueCRC         =       6160H   ; DR0P, was FRaG --07BDFh

                Defstr  Header, <■ Simple code jumper v1.0  Copyright (c) by Alexander Demin'97 ■$>
                Defstr  Prompt, <■ Enter password: $>
                Defstr  Thanks, <■ Thanks$>
                Defstr  Incorp, <■ Incorrect password$>

ofXStr          MACRO   Str:REQ
                ofPush  Str
                ofPush  Str&Size
                ofCUS   Xoring
                ofStr   Str
                ENDM

Program         label   word

                ofXStr  Header
                ofCUS   WriteCrLf

                ofXStr  Prompt

                ofPokeb 17, Buffer
                ofToAX  0A00h
                ofToDX  Buffer
                ofInt   21h

                ofCUS   WriteCrLf

                ofPeekb Buffer+1
                $if =0
                    ofPush  Incorp
                    ofPush  IncorpSize
                $Else
                    ofPush  Thanks        ; Msg
                    ofPush  ThanksSize    ; Msg Size

                    ofCUS   CalcCRC       ; Msg Size CRC

                    ofPush  TrueCRC       ; Msg Size CRC True
                    osSwap                ; Msg Size True CRC
                    ofXor   0AAAAh
                    osSwap
                    ofXor   0AAAAh
                    osXor                 ; Msg Size Res

                    $If <>0               ; Msg Size
                        osDrop                ; Msg
                        osDrop                ; ->
                        ofPush  Incorp        ; Msg
                        ofPush  IncorpSize    ; Msg Size
                    $Endif

                $Endif

                os <osOver>, <osSwap> ; Msg Msg Size

                ofCUS   Xoring        ; Msg
                osStr                 ; ->
                ofCUS   WriteCrLf

                osDos

CalcCRC:        CALLR                 ; ->
                ofPush  0             ; CRC
                ofPush  0             ; CRC 0
                ofPeekb Buffer+1      ; CRC 0 Size
                $For                  ; CRC
                    osI                   ; CRC i
                    ofPush  Buffer+2      ; CRC i Buffer+2
                    osAdd                 ; CRC Addr
                    osPeekb               ; CRC Value
                    osExch                ; CRC Value*256
                    $For    0, 8          ; CRC Value
                        osShl                 ; CRC Value*2 isCarry
                        osRot                 ; Value*2 isCarry CRC
                        osSwap                ; Value*2 CRC isCarry
                        osRcl                 ; Value*2 CRC*2 isCarry
                        $If <>0               ; Value*2 CRC*2
                            ofXor 8408h           ; Value*2 CRC*2^Const
                        $Endif
                        osSwap                ; CRC*2 Value*2
                    $Loop                   ; CRC Value*2
                    osDrop                ; CRC
                $Loop                 ; CRC
                RETR

Xoring:         CALLR                 ; -> Msg Size
                osOver                ; Msg Size Msg
                osAdd                 ; Msg Msg+Size
                $For                  ; ->
                    osI                   ; Addr
                    osDup                 ; Addr Addr
                    osPeekb               ; Addr Value
                    ofPush  XorMask       ; Addr Value XorMask
                    osXor                 ; Addr Xored
                    osSwap                ; Xored Addr
                    osPokeb               ; ->
                $Loop
                RETR

WriteCrLf:      CALLR
                ofChar  13
                ofChar  10
                RETR
