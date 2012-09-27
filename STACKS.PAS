{$A+,B-,D+,E-,F+,G+,I+,L+,N+,O-,P-,Q-,R-,S+,T-,V-,X+,Y+}
{-[■]------------------------------------------------------------------------
  Some stack objects
  Copyright (c) 1997 by Alexander Demin
 ----------------------------------------------------------------------------
}
Unit Stacks;

Interface

Const

  { Maximum size of If/Else/Endif, Do/EndDo, For/Loop stacks }
  MaxCnt = 10000;
  { Maximus size of Store/Restore stack }
  MaxSty = 10;

Type

  Method = function : word;

  TIntStack = object
    Data  : array [ 1..MaxCnt ] of word;
    Ptr   : word;
    Count : word;
    constructor Init;
    function Push : word;
    function Pop : word;
    function Top : word;
  end;

  TStrStack = object
    Data  : array [ 1..MaxSty ] of string;
    Ptr   : word;
    constructor Init;
    function Push( S : string ) : string;
    function Pop : string;
    function Top : string;
  end;

implementation

Constructor TIntStack.Init;
begin
  Ptr:=0;
  Count:=0;
end;

function TIntStack.Push;
begin
  inc( Ptr );
  Data[ Ptr ]:=Count;
  Push:=Count;
  inc( Count );
end;

function TIntStack.Top : word;
begin
  Top:=Data[ Ptr ];
end;

function TIntStack.Pop : word;
begin
  Pop:=Data[ Ptr ];
  Dec( Ptr );
end;

Constructor TStrStack.Init;
begin
  Ptr:=0;
end;

function TStrStack.Push;
begin
  inc( Ptr );
  Data[ Ptr ]:=S;
  Push:=S;
end;

function TStrStack.Top;
begin
  Top:=Data[ Ptr ];
end;

function TStrStack.Pop;
begin
  Pop:=Data[ Ptr ];
  Dec( Ptr );
end;

end.
