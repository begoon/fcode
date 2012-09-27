.model tiny
.code
org 100h

     include Conds.inc
demo:
     mov ax, 0F002h
     cmp ax, 0F001h

     $Do
       $ExitDo =
       $ContDo
     $EndDo Loop

     lea dx, msg2
     $if <>0
       db 256 dup (0)
       lea dx, msg1
     $else
       nop
       $if =0
         hlt
       $endif
       nop
     $endif

     $if =0
     $endif

     mov cx, 2
     $Do
       nop
       $Do
       $EndDo =
       dec cx
       dw 256 dup (0)
     $EndDo Loop
     $Do
       $ExitDo
       $ContDo
     $EndDo Loop

     Store ds, si, ax
       Store ax, bx,cx, es, bp
       Restore
     Restore

     mov ah, 9
     int 21h
     int 20h

msg2  db 'False!$'
msg1  db 'True!$'

end demo
