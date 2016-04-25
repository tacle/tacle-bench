#!/bin/bash

COMPILER=gcc # Please adapt this line to your favorite compiler.
#COMPILER=patmos-clang

OPTIONS=" -Wall -Wno-unknown-pragmas -Werror "

EXEC= # Adapt if the executable is to be executed via another program
#EXEC=valgrind\ -q
#EXEC=pasim

PASS=0
FAIL_COMP=0
FAIL_EXEC=0

for dir in */; do

    cd "$dir"

    printf "Entering ${dir::-1} \n"

    for BENCH in */; do
        cd "$BENCH"
                
        printf "Checking ${BENCH::-1} ..."
        if [ -f a.out ]; then
            rm a.out
        fi
        
        if [ -f *.o ]; then
            rm *.o
        fi
        
        
        # Please remove '&>/dev/null' to identify the warnings (if any)
        $COMPILER $OPTIONS *.c  &>/dev/null
        
        if [ -f a.out ]; then
            $EXEC ./a.out &>/dev/null
            RETURNVALUE=$(echo $?)
            if [ $RETURNVALUE -eq 0 ]; then
                printf "passed. \n"
                ((PASS++))
            else
                printf "failed (wrong return value $RETURNVALUE). \n"
                ((FAIL_EXEC++))
            fi
        else
            printf "failed (compiled with errors/warnings). \n"
            ((FAIL_COMP++))
        fi 
        
        cd ..
    done

    printf "Leaving ${dir::-1} \n\n"
    
    cd ..
done

echo "PASS: $PASS, FAIL_COMP: $FAIL_COMP, FAIL_EXEC: $FAIL_EXEC"