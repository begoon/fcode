XorMask         = 0A5h
Defstr          MACRO   StrName, Str
&StrName        label   byte
                IRPC    ch, <&Str>
                IFIDN   <&ch>, <'>
                  db    "&ch" xor XorMask
                ELSE
                  db    '&ch' xor XorMask
                ENDIF
                ENDM
&StrName&Size   equ     $-&StrName
                ENDM
