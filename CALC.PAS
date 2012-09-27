var
  S     : string;
  CRC   : word;
  AC    : word;
  count : longint;

procedure WriteCRC( CRC : word );
const
  H : string[16]='0123456789ABCDEF';
begin
  writeln( H[CRC div $1000+1], H[((CRC and $F00) shr 8)+1],
           H[((CRC and $F0) shr 4)+1], H[CRC mod $10+1] );
end;

function CalcCRC( S : string ) : word;
var
  CRC : word;
  i   : word;
  c   : char;
begin
  CRC:=$0000;
  for i:=1 to length(S) do begin
    c:=S[i];
    asm
      mov bl, &c
      mov bh, 8
     @@1:
      shl bl, 1
      mov ax, CRC
      rcl ax, 1
      jnc @@2
      xor ax, $8408
     @@2:
      mov CRC, ax
      dec bh
      jnz @@1
    end;
{
    asm
      cbw
      xchg al, ah
      mov dx, CRC
      xor dx, ax
      mov cx, 8
     @@1:
      mov bx, dx
      shl dx, 1
      and bx, $8000
      jz @@2
      xor dx, $1021
     @@2:
      loop @@1
      mov CRC, dx
    end;
}
  end;
  CalcCRC:=CRC;
end;

procedure Search( var S : string; index : word );
var
  i : char;
begin
  if index<=length(S) then begin
    for i:='A' to chr($7F) do begin
      S[Index]:=i;
      CRC:=CalcCRC( S );
      if CRC=AC then begin writeln('<', S, '>'); inc(count); end;
      Search( S, index+1);
    end;
  end;
end;

begin
  AC:= CalcCRC( 'DR0P' );
  WriteCRC( AC );
  readln;
  S:='    ';
  Search( S, 1 );
  writeln( count );
end.
