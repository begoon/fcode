@echo off
pre <demo.asm >demo.pre
tasm /m5 /la demo.pre
tlink /t demo
