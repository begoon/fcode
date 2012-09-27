; -[*]-----------------------------------------------------------------------
; Useful macros for F-Code for advanced executing control
; Version 1.0
; Copyright (c) 1996 by Alexander Demin
; ---------------------------------------------------------------------------

; ---------------------------------------------------------------------------
; Compile true conditional jump
; Warning: If Cond='@', compile the unconditional jump
;
TrueCondJumps   MACRO         Cond:REQ, Way:REQ
                IfIdn         <&Cond>, <EQ>             ; =
                  ofJz        &Way
                ElseIfIdn     <&Cond>, <NE>             ; <>
                  ofJnz        &Way
                ElseIfIdn     <&Cond>, <LE>             ; <=
                  ofJle        &Way
                ElseIfIdn     <&Cond>, <LT>             ; <
                  ofJlt        &Way
                ElseIfIdn     <&Cond>, <GE>             ; >=
                  ofJge        &Way
                ElseIfIdn     <&Cond>, <GT>             ; >
                  ofJgt        &Way
                ElseIfIdn     <&Cond>, <@>              ; Jump
                  ofJmp        &Way
                Else                                    ; Error in condition
                  Err "Incorrect condition &Cond"
                EndIf
                ENDM

; ---------------------------------------------------------------------------
; Compile inversed conditional jump
; Warning: If Cond='@', compile the unconditional jump
;
NotCondJumps    MACRO         Cond:REQ, Way:REQ
                IfIdn         <&Cond>, <EQ>             ; =
                  ofJnz        &Way
                ElseIfIdn     <&Cond>, <NE>             ; <>
                  ofJz         &Way
                ElseIfIdn     <&Cond>, <LE>             ; <=
                  ofJgt        &Way
                ElseIfIdn     <&Cond>, <LT>             ; <
                  ofJge        &Way
                ElseIfIdn     <&Cond>, <GE>             ; >=
                  ofJlt        &Way
                ElseIfIdn     <&Cond>, <GT>             ; >
                  ofJle        &Way
                ElseIfIdn     <&Cond>, <@>              ; Jump
                  ofJmp        &Way
                Else                                    ; Error in condition
                  Err "Incorrect condition &Cond"
                EndIf
                ENDM

; ---------------------------------------------------------------------------
; Start $If construction

$If             MACRO         Cond:REQ, IfCnt:REQ
                Local         Way
                Way           CatStr <@@if_>, <&IfCnt>
                NotCondJumps  Cond, Way                 ; Compile inversed
                ENDM                                    ; jump

; ---------------------------------------------------------------------------
; Make $Else way in $If contruction

$Else           MACRO         IfCnt:REQ
                Local         Way, ElseWay
                Way           CatStr <@@if_>, <&IfCnt>
                ElseWay       CatStr <@@else_>, <&IfCnt>
                ofJmp         &ElseWay
&Way:           ENDM

; ---------------------------------------------------------------------------
; Finish $If construction
; If there was $Else-way -> Tag=1, otherwise -> Tag=0

$Endif          MACRO         IfCnt:REQ, Tag:REQ
                Local         Way
                If Tag EQ 1                          ; Is there $Else-way ?
                  Way   CatStr <@@else_>, <&IfCnt>   ; Finish $Else
                Else
                  Way   CatStr <@@if_>, <&IfCnt>     ; Finish $If
                EndIf
&Way:           ENDM

; ---------------------------------------------------------------------------
; Start $Do construnction

$Do             MACRO         DoCnt:REQ
                Local         Way
                Way CatStr <@@do_>, <&DoCnt>
&Way:           ENDM

; ---------------------------------------------------------------------------
; Break $Do construction

$ExitDo         MACRO         Cond:REQ, DoCnt:REQ
                Local         Exit
                Exit  CatStr <@@exit_do_>, <&DoCnt>
                TrueCondJumps Cond, Exit             ; Compile conditional
                ENDM                                 ; jump

; ---------------------------------------------------------------------------
; Go to the next $Do iteration

$ContDo         MACRO         Cond:REQ, DoCnt:REQ
                Local         Cont
                Cont CatStr <@@cont_do_>, <&DoCnt>
                TrueCondJumps Cond, Cont             ; Compile conditional
                ENDM                                 ; jump

; ---------------------------------------------------------------------------
; Finish $Do contruction

$EndDo          MACRO Cond:REQ, DoCnt:REQ
                Local         Way, Cont, Exit
                Way   CatStr <@@do_>,      <&DoCnt>  ; Loop-label
                Cont  CatStr <@@cont_do_>, <&DoCnt>  ; Continue-label
                Exit  CatStr <@@exit_do_>, <&DoCnt>  ; Exit-label
&Cont:          TrueCondJumps Cond, Way              ; Compile conditional
&Exit:          ENDM                                 ; jump

; ---------------------------------------------------------------------------
; Start $For construnction
; Warning: When using format with non-blanked arguments, both arguments
;          must be present

$For            MACRO         ForCnt:REQ, AStart, AEnd
                Local         Way
                Way   CatStr <@@for_>, <&ForCnt>
                IfNB <&AStart>                       ; What's calling format ?
                  ofDo AStart, AEnd                  ; Immediate format
                Else
                  osDo                               ; In-stack format
                EndIf
&Way:           ENDM

; ---------------------------------------------------------------------------
; Break $For construction

$ExitFor        MACRO         Cond:REQ, ForCnt:REQ
                Local         Exit
                Exit  CatStr <@@exit_for_>, <&ForCnt>
                TrueCondJumps Cond, Exit
                ENDM

; ---------------------------------------------------------------------------
; Go to the next $For iteration

$ContFor        MACRO         Cond:REQ, ForCnt:REQ
                Local         Cont
                Cont CatStr <@@cont_for_>, <&ForCnt>
                TrueCondJumps Cond, Cont
                ENDM

; ---------------------------------------------------------------------------
; Finish $For construnction with Step
; Warning: If Delta is absent, Delta must be in the data-stack

$Step           MACRO         ForCnt:REQ, Delta
                Local         Way, Nearly, Cont, Exit
                Cont CatStr   <@@cont_for_>, <&ForCnt>
                Exit  CatStr  <@@exit_for_>, <&ForCnt>
                Way CatStr <@@for_>, <&ForCnt>
                IfNB <&Delta>                             ; Is Delta present ?
&Cont:            ofStep        &Way, Delta
                Else
&Cont:            ofPush        &Way                      ; Delta Addr
                  osSwap                                  ; Addr Delta
                  osStep
                EndIf
&Exit:          ENDM

; ---------------------------------------------------------------------------
; Finish $For construction w/o Step

$Loop           MACRO         ForCnt:REQ
                Local         Way, Nearly, Cont, Exit
                Cont CatStr   <@@cont_for_>, <&ForCnt>
                Exit  CatStr  <@@exit_for_>, <&ForCnt>
                Way           CatStr <@@for_>, <&ForCnt>
&Cont:          ofLoop        &Way
&Exit:          ENDM
