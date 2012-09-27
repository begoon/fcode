{$A+,B-,D+,E-,F+,G+,I+,L+,N+,O-,P-,Q-,R-,S+,T-,V-,X+,Y+}
{-[■]------------------------------------------------------------------------
   F-Code and Asm source preprocessor
   Version 1.0
   Copyright (c) 1997 by Alexander Demin
 ----------------------------------------------------------------------------
}
Uses Stacks;

Const

  { Conditions table size }
  N      = 9;

  { Conditions tables }
  { Warning: Short words in Conds table must follow after longer ones }
  Conds  : array [ 1..N ] of string[4]
           = ( 'LOOP', '<>0', '<=', '>=', '<>', '=0', '<',  '>', '=' );
  Repls  : array [ 1..N ] of string[4]
           = ( 'LOOP', 'NE', 'LE', 'GE', 'NE', 'EQ', 'LT', 'GT', 'EQ' );

var
  IfStack  : TIntStack;
  DoStack  : TIntStack;
  ForStack : TIntStack;
  StyStack : TStrStack;
  S        : string;
  c, R     : string;
  i, j, Y  : word;

function UpStr( S : string ) : string;
var
  i   : word;
begin
  for i:=1 to length( S ) do S[i]:=UpCase(S[i]);
  UpStr:=S;
end;

function IntToStr( N : word ) : string;
var
  T : string;
begin
  Str( N:5, T );
  IntToStr:=T;
end;

{ --------------------------------------------------------------------------
  Replace words in the C in accordance with Conds/Repls tables.
  If replacement has been done, function returns the index of the next
  position in the C after this replacement and the value of Index
  doesn't matter.
  Otherwise function inserts ' @' ( unconditional jump sign )
  into the Index-prosition and returns the the index of then position
  after inserted ' @' ( value of Index+2 ).
}
function ReplaceConds( var C, S : string; Index : integer ) : integer;
var
  i, j : integer;
  Res  : integer;
begin
  Res:=0;
  for j:=1 to N do begin
    i:=Pos( Conds[j], C );               { Search current Condition }
    if i<>0 then begin                   { Found ? }
      delete( S, i, Length(Conds[j]) );  { Delete condition }
      insert( Repls[j], S, i );          { Insert replacement }
      Res:=i+Length(Repls[j]);           { Calculate the index after }
      break;                             { The replacement and break loop }
    end;                                 { Ignore the value of Index }
  end;
  if Res=0 then begin                    { Are there no replacements ? }
    Insert( ' @', S, Index );            { Yeeh... Insert the unconditional }
    Res:=Index+2;                        { jump sign }
  end;                                   { and return the value of Index+2 }
  ReplaceConds:=Res;
end;

{ ------------------------------------------------------------------------- }

begin

  IfStack.Init;  { 15th bit of the IF-stack means:
                     0 - there is no Else-way
                     1 - there is Else-way
                 }
  DoStack.Init;
  ForStack.Init;
  StyStack.Init;

  { Main processing loop }
  while not eof do begin
    readln( S );
    C:=UpStr( S );

    { Replace $If-condition.
      Warning: Condition must be present
    }
    if Pos( '$IF', C )<>0 then begin
      i:=ReplaceConds( C, S, 0);

      { Clear the 15th bit -> there is no Else-way }
      insert( ', '+IntToStr( IfStack.Push ), S, i );
    end

    { Process $Else-way
    }
    else if Pos( '$ELSE', C )<>0 then begin
      i:=Pos( '$ELSE', C );
      insert( ' '+IntToStr( IfStack.Top ), S, i+5 );

      { Set up the 15th bit -> there is Else-way }
      IfStack.Data[ IfStack.Ptr ]:=IfStack.Data[ IfStack.Ptr ] or $8000;
    end

    { Process $EndIf word
    }
    else if Pos( '$ENDIF', C )<>0 then begin
      i:=Pos( '$ENDIF', C );
      insert( ' '+
              { Mask the 15th bit - it isn't the part of the number }
              IntToStr( IfStack.Pop and $7FFF )+', '+
              { Set this number in accordance with the 15th bit }
              IntToStr( (IfStack.Data[ IfStack.Ptr+1 ] and $8000) shr 15),
              S, i+6 );
    end

    { Process $Do word
    }
    else if Pos( '$DO', C )<>0 then begin
      i:=Pos( '$DO', C );
      insert( ' '+IntToStr( DoStack.Push), S, i+3 );
    end

    { Replace $ExitDo condition
      Warning: Condition can be absent.
               if there is no condition, insert '@'- unconditional jump sign
               otherwise replace the condition in accordance with
               Conds/Repls tables.
    }
    else if Pos( '$EXITDO', C )<>0 then begin
      i:=Pos( '$EXITDO', C )+7;
      j:=ReplaceConds( C, S, i);
      insert( ', '+IntToStr( DoStack.Top), S, j );
    end

    { Replace $ContDo condition
      Warning: Condition can be absent.
               if there is no condition, insert '@'- unconditional jump sign
               otherwise replace the condition in accordance with
               Conds/Repls tables.
    }
    else if Pos( '$CONTDO', C )<>0 then begin
      i:=Pos( '$CONTDO', C )+7;
      j:=ReplaceConds( C, S, i);
      insert( ', '+IntToStr( DoStack.Top), S, j );
    end

    { Replace $EndDo-condition.
      Warning: Condition must be present
    }
    else if Pos( '$ENDDO', C )<>0 then begin
      i:=ReplaceConds( C, S, 0)+6;
      insert( ', '+IntToStr( DoStack.Pop ), S, i );
    end

    { Process $For word
    }
    else if Pos( '$FOR', C )<>0 then begin
      i:=Pos( '$FOR', C );
      insert( ' '+IntToStr( ForStack.Push), S, i+4 );
    end

    { Replace $ExitFor condition
      Warning: Condition can be absent.
               if there is no condition, insert '@'- unconditional jump sign
               otherwise replace the condition in accordance with
               Conds/Repls tables.
    }
    else if Pos( '$EXITFOR', C )<>0 then begin
      i:=Pos( '$EXITFOR', C )+8;
      j:=ReplaceConds( C, S, i);
      insert( ', '+IntToStr( ForStack.Top), S, j );
    end

    { Replace $ContFor condition
      Warning: Condition can be absent.
               if there is no condition, insert '@'- unconditional jump sign
               otherwise replace the condition in accordance with
               Conds/Repls tables.
    }
    else if Pos( '$CONTFOR', C )<>0 then begin
      i:=Pos( '$CONTFOR', C )+8;
      j:=ReplaceConds( C, S, i);
      insert( ', '+IntToStr( ForStack.Top), S, j );
    end

    { Process $Step word
    }
    else if Pos( '$STEP', C )<>0 then begin
      i:=Pos( '$STEP', C );
      insert( ' '+IntToStr( ForStack.Pop), S, i+5 );
    end

    { Process $Loop word
    }
    else if Pos( '$LOOP', C )<>0 then begin
      i:=Pos( '$LOOP', C );
      insert( ' '+IntToStr( ForStack.Pop), S, i+5 );
    end

    { Restore registers
    }
    else if Pos( 'RESTORE', C )<>0 then begin
      i:=Pos( 'RESTORE', C );
      delete( S, i, 7 );
      insert( 'Pop', S, i );
      Insert( StyStack.Pop, S, i+3 );
    end

    { Push registers and save reversed order of them
    }
    else if Pos( 'STORE', C )<>0 then begin
      i:=Pos( 'STORE', C );
      delete( S, i, 5 );
      insert( 'Push', S, i );
      C:='';                                  { Reversed order string }
      i:=i+4;
      j:=i;                                   { Start index }
      while j<=Length(S) do begin             { Replace all comma to blank }
        if S[j]=',' then S[j]:=' ';
        inc(j);
      end;

      { Take the each word and insert it at the begining of C }
      while i<=Length(S) do begin

        { Pass through all blanks and tabs }
        while (i<=Length(S)) and (S[i]<=' ') do Inc( i );

        { Get current word }
        R:='';
        while (i<=Length(S)) and (S[i]>' ') do begin
          R:=R+S[i];
          Inc( i );
        end;

        { Insert the word at the begining of the register's string }
        Insert( ' '+R+' ', C, 1 );
        inc(i);
      end;
      StyStack.Push(C);           { Put the registers string into the stack }
    end;
    writeln( S );
  end;
end.
