#!/bin/bash

generate=1
run_norm=1                                 # run normal tests
run_valg=1                                 # run valgrind tests

# Determine column width of the terminal
if [[ -z "$COLUMNS" ]]; then
    printf "Setting COLUMNS based on stty\n"
    COLUMNS=$(stty size | awk '{print $2}')
fi
if (($COLUMNS == 0)); then
    COLUMNS=126
fi

printf "COLUMNS is $COLUMNS\n"
DIFF="diff -bBy -W $COLUMNS"                    # -b ignore whitespace, -B ignore blank lines, -y do side-by-side comparison, -W to increase width of columns

VALGRIND="valgrind --leak-check=full --show-leak-kinds=all"

INPUT=test-data/input.tmp                    # name for expected output file
EXPECT=test-data/expect.tmp                  # name for expected output file
ACTUAL=test-data/actual.tmp                  # name for actual output file
TMPFILE=test-data/temp.tmp                   # name for actual output file
DIFFOUT=test-data/diff.tmp                   # name for diff output file
VALGOUT=test-data/valgrind.tmp               # name for valgrind output file

function major_sep(){
    printf '%s\n' '============================================================'
}
function minor_sep(){
    printf '%s\n' '++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
}

printf "Loading tests... "
source test_commando_data.sh
printf "%d tests loaded\n" "$T"

NTESTS=$T
VTESTS=$T
NPASS=0
NVALG=0

all_tests=$(seq $NTESTS)

# Check whether a single test is being run
single_test=$1
if ((single_test > 0 && single_test <= NTESTS)); then
    printf "Running single TEST %d\n" "$single_test"
    all_tests=$single_test
    NTESTS=1
    VTESTS=1
else
    printf "Running %d tests\n" "$NTESTS"
fi

# printf "tests: %s\n" "$all_tests"
printf "\n"

# Run normal tests: capture output and check against expected
if [ "$run_norm" = "1" ]; then
    printf "RUNNING NORMAL TESTS\n"
    for i in $all_tests; do
        printf "TEST %2d %-18s : " "$i" "${tnames[i]}"
        FAIL="0"
        
        # Run the test
        # run program with given input
        printf "%s\n" "${input[i]}" | ./commando --echo >& $TMPFILE
        ./standardize_pids $TMPFILE > $ACTUAL

        # generate expected output
        printf "%s\n" "${output[i]}" > $EXPECT

        # Check for output differences, print side-by-side diff if problems
        if ! $DIFF $EXPECT $ACTUAL > $DIFFOUT
        then
            printf "FAIL: Output Incorrect\n"
            minor_sep
            printf "INPUT:\n%s\n" "${input[i]}"
            minor_sep
            # Mac OSX has a different version sed so use awk to add on the ACTUAL/EXPECT
            $DIFF <(awk 'BEGIN{print "***EXPECT OUTPUT***"} {print}' $EXPECT) <(awk 'BEGIN{print "***ACTUAL OUTPUT***"} {print}' $ACTUAL) > $DIFFOUT   # regen the diff with the headers
            cat $DIFFOUT
            if [ "$generate" == "1" ]; then
                printf "%s\n" "---FULL ACTUAL---"
                cat $ACTUAL
                echo
            fi
            minor_sep
            FAIL="1"
        fi

        if [ -n "${tfiles[i]}" ]; then                   # if there is an output file to check
            TEXPECT=test-data/texpect.txt                # name for expected output file
            TACTUAL="${tfiles[i]}"                       # name for actual output file
            TDIFFOUT=test-data/ttest.diff                # name for diff output file
            
            # generate expected output
            printf "%s\n" "${tfiles_expect[i]}" > $TEXPECT

            # Check for output differences, print side-by-side diff if problems
            if ! $DIFF $TEXPECT $TACTUAL > $TDIFFOUT
            then
                printf "FAIL: File %s incorrect\n" "$TACTUAL"
                minor_sep
                printf "INPUT:\n%s\n" "${input[i]}"
                printf "OUTPUT: EXPECT   vs   ACTUAL\n"
                cat $TDIFFOUT
                if [ "$generate" == "1" ]; then
                    printf "%s\n" "---FULL ACTUAL---"
                    cat $TACTUAL
                fi
                minor_sep
                FAIL="1"
            fi
        fi


        if (( FAIL == 0 )); then
            printf "OK\n"
            ((NPASS++))
        fi            
    done
    printf "Finished:\n"
    printf "%2d / %2d Normal correct\n" "$NPASS" "$NTESTS"
    printf "\n"
fi

# ================================================================================

# Run valgrind tests: check only for problems identified by valgrind
if [ "$run_valg" = "1" ]; then
    printf "RUNNING VALGRIND TESTS\n"

    for i in $all_tests; do
        printf "TEST %2d %-18s : " "$i" "${tnames[i]}"
        
        # run code through valgrind
        printf "%s\n" "${input[i]}" | $VALGRIND ./commando --echo >& $VALGOUT

        # Check various outputs from valgrind
        if ! grep -q 'ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)' $VALGOUT ||
           ! grep -q 'in use at exit: 0 bytes in 0 blocks'  $VALGOUT ||
             grep -q 'definitely lost: 0 bytes in 0 blocks' $VALGOUT;
        then
            printf "FAIL: Valgrind detected problems\n"
            minor_sep
            cat $VALGOUT
            minor_sep
        else
            printf "Valgrind OK\n"
            ((NVALG++))
        fi
    done
    printf "Finished:\n"
    printf "%2d / %2d Valgrind correct\n" "$NVALG" "$VTESTS"
    printf "\n"
fi


major_sep
printf "OVERALL:\n"
printf "%2d / %2d Normal correct\n" "$NPASS" "$NTESTS"
printf "%2d / %2d Valgrind correct\n" "$NVALG" "$VTESTS"

