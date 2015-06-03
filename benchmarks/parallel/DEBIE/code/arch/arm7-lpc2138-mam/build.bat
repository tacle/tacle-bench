echo off
Rem Batch file to compile debie1 using the GCC ARM
Rem compiler that came with the iF-DEV-LPC kit from iSYSTEM.

Rem setlocal

Rem Set tpd to the Test Program Directory:
set tpd=..\..

Rem Set hnd to the "harness" directory:

set hnd=%tpd%\harness

Rem Set ccd to the compiler config directory:
set ccd=%tpd%\..\arm7-lpc2138-mam\gcc-if07

call %ccd%\setup.bat

Rem Compile the test program modules:

%CC% %CCOPT% -I%ccd% -I. -I%hnd%         %tpd%\class.c     -o class.o
%CC% %CCOPT% -I%ccd% -I. -I%hnd%         %tpd%\classtab.c  -o classtab.o
%CC% %CCOPT% -I%ccd% -I. -I%hnd%         %tpd%\debie.c     -o debie.o
%CC% %CCOPT% -I%ccd% -I. -I%hnd% -I%tpd% %hnd%\harness.c   -o harness.o
%CC% %CCOPT% -I%ccd% -I. -I%hnd%         %tpd%\health.c    -o health.o
%CC% %CCOPT% -I%ccd% -I. -I%hnd%         %tpd%\hw_if.c     -o hw_if.o
%CC% %CCOPT% -I%ccd% -I. -I%hnd%         %tpd%\measure.c   -o measure.o
%CC% %CCOPT% -I%ccd% -I. -I%hnd% -I%tpd%       target.c    -o target.o
%CC% %CCOPT% -I%ccd% -I. -I%hnd%         %tpd%\tc_hand.c   -o tc_hand.o
%CC% %CCOPT% -I%ccd% -I. -I%hnd%         %tpd%\telem.c     -o telem.o

Rem Compile the run-time support:

%CC% %ASOPT%         %ccd%\crt0.s       -o crt0.o
%CC% %ASOPT%         %ccd%\intvec.s     -o intvec.o
%CC% %CCOPT%         %ccd%\crt_asyst.c  -o crt_asyst.o
%CC% %CCOPT% -I%ccd% %ccd%\cpulib.c     -o cpulib.o

Rem Link the program:

%LD% %LDOPT% ^
   -Xlinker --output=debie1.elf          ^
   -Xlinker --script=%ccd%\link.txt      ^
   intvec.o crt0.o crt_asyst.o cpulib.o  ^
   class.o      ^
   classtab.o   ^
   debie.o      ^
   harness.o    ^
   health.o     ^
   hw_if.o      ^
   measure.o    ^
   target.o     ^
   tc_hand.o    ^
   telem.o

endlocal
