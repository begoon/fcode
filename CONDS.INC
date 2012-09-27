; -[*]--------------------------------------------------------------------
; Useful macro for assembler
; Version 1.0
; Copyright (c) 1996 by Alexander Demin
; ------------------------------------------------------------------------

; cmp a, b (?)

TrueCondJumps   MACRO         Cond:REQ, Way:REQ
                IfIdn         <&Cond>, <EQ>             ; =
                 jz           &Way
                ElseIfIdn     <&Cond>, <NE>             ; <>
                 jnz          &Way
                ElseIfIdn     <&Cond>, <LE>             ; <=
                 jbe          &Way
                ElseIfIdn     <&Cond>, <LT>             ; <
                 jb           &Way
                ElseIfIdn     <&Cond>, <GE>             ; >=
                 jae          &Way
                ElseIfIdn     <&Cond>, <GT>             ; >
                 ja           &Way
                ElseIfIdn     <&Cond>, <@>              ; Jump
                 jmp          &Way
                Else                                    ; Error
                  Err "Incorrect condition"
                EndIf
                ENDM

NotCondJumps    MACRO         Cond:REQ, Way:REQ
                IfIdn         <&Cond>, <EQ>             ; =
                  jnz          &Way
                ElseIfIdn     <&Cond>, <NE>             ; <>
                  jz           &Way
                ElseIfIdn     <&Cond>, <LE>             ; <=
                  ja           &Way
                ElseIfIdn     <&Cond>, <LT>             ; <
                  jae          &Way
                ElseIfIdn     <&Cond>, <GE>             ; >=
                  jb           &Way
                ElseIfIdn     <&Cond>, <GT>             ; >
                  jbe          &Way
                ElseIfIdn     <&Cond>, <@>              ; Jump
                  jmp          &Way
                Else                                    ; Error
                  Err "Incorrect condition"
                EndIf
                ENDM

$If             MACRO         Cond:REQ, IfCnt:REQ
                Local         Way, Nearly
                Way           CatStr <@@if_>, <&IfCnt>
                Nearly        CatStr <@@if_near>, <&IfCnt>

                If (((&Way-$) LE 127) and ((&Way-$) GE -128)) or (@CPU and 18h)
                  NotCondJumps Cond, Way
                Else
                  TrueCondJumps Cond, &Nearly
                  jmp  &Way
&Nearly:        Endif

                ENDM

$Else           MACRO         IfCnt:REQ
                Local         Way, ElseWay
                Way           CatStr <@@if_>, <&IfCnt>
                ElseWay       CatStr <@@else_>, <&IfCnt>
                jmp           &ElseWay
&Way:           ENDM

$Endif          MACRO         IfCnt:REQ, Tag:REQ
                Local         Way
                If Tag EQ 1
                  Way   CatStr <@@else_>, <&IfCnt>
                Else
                  Way   CatStr <@@if_>, <&IfCnt>
                EndIf
&Way:           ENDM

$Do             MACRO         DoCnt:REQ
                Local         Way
                Way CatStr <@@do_>, <&DoCnt>
&Way:           ENDM

$ExitDo         MACRO         Cond:REQ, DoCnt:REQ
                Local         Exit
                Exit  CatStr <@@exit_>, <&DoCnt>
                TrueCondJumps Cond, Exit
                ENDM

$ContDo         MACRO         Cond:REQ, DoCnt:REQ
                Local         Cont
                Cont CatStr <@@cont_>, <&DoCnt>
                TrueCondJumps Cond, Cont
                ENDM

$EndDo          MACRO Cond:REQ, DoCnt:REQ
                Local         Way, Nearly, Exit, Cont
                Way   CatStr <@@do_>, <&DoCnt>
                Exit  CatStr <@@Exit_>, <&DoCnt>
                Cont  CatStr <@@Cont_>, <&DoCnt>
&Cont:          IfIdn <&Cond>, <LOOP>
                  If ((&Way-$) LE 127) and ((&Way-$) GE -128)
                    loop &Way
                  Else
                    Nearly CatStr <@@do_near>, <&DoCnt>
                    jcxz &Nearly
                    jmp  &Way
&Nearly:          Endif
                Else
                  If (((&Way-$) LE 127) and ((&Way-$) GE -128)) or (@CPU and 18h)
                    TrueCondJumps Cond, Way
                  Else
                    NotCondJumps  Cond, &Nearly
                    jmp &Way
&Nearly:          EndIf
                Endif
&Exit:          ENDM
