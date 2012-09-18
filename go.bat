@echo off
setlocal

set main=main.pdb

del *.ilk
del *.pdb
ml /c /coff /Zi first.asm
link /debug /subsystem:console /entry:start /out:main.exe first.obj io.obj Kernel32.Lib 


IF EXIST %main% (
  windbg main.exe
) ELSE (
  echo debug 정보없음
)