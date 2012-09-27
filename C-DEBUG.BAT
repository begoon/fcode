@echo off
pre <fcode.asm >fcode.pre
pre <ftest.inc >ftest.pre
tasm /la /m2 fcode.pre
tlink /t fcode
