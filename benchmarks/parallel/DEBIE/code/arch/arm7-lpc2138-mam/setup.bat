echo off
Rem Batch file to set up environment for GCC ARM7 C compiler from iSYSTEMS
Rem for the iF-DEV-LPC kit, for all test programs.
Rem
Rem Copyright (c) 2008 Tidorum Ltd.
Rem
Rem $Revision: 1.2 $


Rem GCC installation paths:

set IFDEVD=C:\iFDEV\2007
set GCCD=%IFDEVD%\gcc\arm
set GCCbin=%GCCD%\bin
set GCClib=%GCCD%\lib

set PATH=%GCCbin%;%PATH%

Rem C compiler and options:

set CC=arm-elf-gcc.exe

set CCOPT=-c -mcpu=arm7tdmi -g -O2

set ASOPT=-c -mcpu=arm7tdmi -g

Rem Linker and options:

set LD=arm-elf-gcc.exe

set LDOPT=-nostartfiles       ^
    -mlittle-endian         ^
    -mcpu=arm7tdmi          ^
    -Xlinker -lc             ^
    -lnosys                  ^
    -Xlinker -Map=link.map   ^
    -Xlinker -n

Rem Common runtime files to be included in the link:
Rem    crt0.s      (use ASOPT)
Rem    intvec.s    (use ASOPT)
Rem    crt_asyst.c (use CCOPT)
Rem    cpulib.c    (use CCOPT)
Rem
Rem Use the linker script file link.txt, with the LD option
Rem -Xlinker --script=%dir%\link.txt.
