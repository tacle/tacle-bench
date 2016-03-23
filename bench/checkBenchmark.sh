#!/bin/bash

COMPILER=gcc # Please adapt this line to your favorite compiler.

OPTIONS=" -Wall -Wno-unknown-pragmas -Werror "

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
            ./a.out  &>/dev/null
            RETURNVALUE=$(echo $?)
            if [ $RETURNVALUE -eq 0 ]; then
                printf "passed. \n"
            else
                printf "failed (wrong return value $RETURNVALUE). \n"
            fi
        else
            printf "failed (compiled with errors/warnings). \n"
        fi 
        
        cd ..
    done

    printf "Leaving ${dir::-1} \n\n"
    
    cd ..
done
