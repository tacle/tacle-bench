#!/bin/sh
#
# Script to cross-compile "debie1" for the MPC5554 with the
# CodeSourcery "G++ lite" GCC package.
#
# Any command-line arguments go into the CCOPT (eg. -Wpadded).
# As default, the trace output from the harness and target
# modules are disabled.
#
# One has to set the path to the compiler (i.e. CC) accordingly.


# Set tpd to the Test Program Directory:
tpd=../..

# Set hnd to the "harness" directory:
hnd=${tpd}/harness

# Target gcc and options:
export CC="/local/swegener/CodeSourcery/Sourcery_G++_Lite/bin/powerpc-eabi-gcc"
export CCOPT="-te500v1 -fshort-double -g -O2 -I. -I${hnd} -Wall -Wextra $*"
export LD="/local/swegener/CodeSourcery/Sourcery_G++_Lite/bin/powerpc-eabi-gcc"
export LDOPT="-te500v1 -fshort-double -Wl,-T./linker-mpc55xx-gcc.ld -static"

${CC} ${CCOPT} -c ${tpd}/class.c
${CC} ${CCOPT} -c ${tpd}/classtab.c
${CC} ${CCOPT} -c ${tpd}/debie.c
${CC} ${CCOPT} -c -I${tpd} ${hnd}/harness.c
${CC} ${CCOPT} -c ${tpd}/health.c
${CC} ${CCOPT} -c ${tpd}/hw_if.c
${CC} ${CCOPT} -c ${tpd}/measure.c
${CC} ${CCOPT} -c -I${tpd} target.c
${CC} ${CCOPT} -c ${tpd}/tc_hand.c
${CC} ${CCOPT} -c ${tpd}/telem.c


${CC} ${LDOPT}           \
    -o debie1.elf        \
    class.o              \
    classtab.o           \
    debie.o              \
    harness.o            \
    health.o             \
    hw_if.o              \
    measure.o            \
    target.o             \
    tc_hand.o            \
    telem.o

