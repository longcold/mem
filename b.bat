@echo off
setlocal

set main=main.obj
set monitor=monitor.obj



del *.obj

ml /c /coff monitor.asm
ml /c /coff init.asm
cl /c main.c
cl /c /GS- t1.c

cl main.obj monitor.obj

link /subsystem:console /entry:init /nodefaultlib /out:t1.exe /base:0x00430000 init.obj monitor.obj t1.obj