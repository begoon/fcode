@echo off
pre <fcode.asm >fcode.pre
pre <ftest.inc >ftest.pre
tasm /dDEBUG /zi /la /m2 fcode.pre
tlink /v fcode
