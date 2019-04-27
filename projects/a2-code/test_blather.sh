#!/bin/bash

# variables which detemine time to wait between client inputs
ticktime_norm=0.05              # ticks during normal test
ticktime_valg=0.5               # ticks during valgrind - longer due to debug delays
ticktime=$ticktime_valg         # global tick time, set during normal/valgrind

if [ ! -d $TESTDATA ]; then
    printf "Missing directory '%s': create this directory\n" "$TESTDATA"
    return 1
fi    

CODEDIR=".."                    # directory where all executables/scripts are stored
TESTDIR="test-data/"

# BL_SERVER="../bl_server"
# BL_CLIENT="../bl_client"

generate=0
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
# VALGRIND="valgrind --leak-check=full --show-leak-kinds=all  --suppressions=../test.supp"
# VALGRIND="valgrind --leak-check=full --show-leak-kinds=definite" #,indirect,possible"
# VALGRIND="valgrind"


# INPUT=test-data/input.tmp                    # name for expected output file
# EXPECT=test-data/expect.tmp                  # name for expected output file
# ACTUAL=test-data/actual.tmp                  # name for actual output file
# TMPFILE=test-data/temp.tmp                   # name for actual output file
# DIFFOUT=test-data/diff.tmp                   # name for diff output file
# VALGOUT=test-data/valgrind.tmp               # name for valgrind output file


function major_sep(){
    printf '%s\n' '====================================='
}
function minor_sep(){
    printf '%s\n' '-------------------------------------'
}

printf "Loading tests... "
source test_blather_data.sh
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

printf "\n"



printf "RUNNING NORMAL TESTS\n"
cd $TESTDIR

export valg=""
ticktime=$ticktime_norm

if [ "$run_norm" = "1" ]; then

    for i in $all_tests; do
        printf -v tid "test-%02d" "$i"         # test id for various places
        printf "TEST %2d %-18s : " "$i" "${tnames[i]}"
        
        # Run the test
        outfiles=()
        eval "${setup[i]}"
        while read -r act; do
            eval "$act"
            tick
        done <<< "${actions[i]}"
        eval "${teardown[i]}"

        # Check each client output file for failure, print side-by-side diff if problems
        failed=0
        for ((c=0; c<${#outfiles[*]}; c++)); do
            outfile=${outfiles[c]}
            printf "%s\n" "${expect_outs[i]}" | awk -F '\t' "{print \$$((c+1))}" > ${outfile}.expect
            
            if ! $DIFF ${outfile}.expect $outfile > ${outfile}.diff
            then
                printf "FAIL\n"
                minor_sep 
                printf "Difference between '%s' and '%s'\n" "${outfile}.expect" "$outfile" 
                printf "OUTPUT: EXPECT   vs   ACTUAL\n"
                cat ${outfile}.diff
                minor_sep
                failed=1
            else
                rm ${outfile}.diff
                # printf "'%s' OK " "${outfile}"
            fi
        done

        if [ "$failed" != "1" ]; then
            printf "ALL OK\n"
            ((NPASS++))
        elif [ "$generate" = "1" ]; then
            printf "GENERATE\n"
            echo "${outfiles[*]}"
            paste ${outfiles[*]}
            minor_sep
        fi
        #    printf "\n"

    done

    printf "%s\n" "$major_sep"
    printf "Finished:\n"
    printf "%2d / %2d Normal correct\n" "$NPASS" "$NTESTS"
    printf "\n"
fi

# ================================================================================

if [ "$run_valg" = "1" ]; then

    printf "RUNNING VALGRIND TESTS\n"
    export valg="$VALGRIND"
    export valgpref="valgrind"
    
    ticktime=$ticktime_valg

    for i in $all_tests; do
        printf -v tid "test-%02d" "$i"         # test id for various places
        printf "TEST %2d %-18s : " "$i" "${tnames[i]}"
        
        # Run the test
        outfiles=()
        eval "${setup[i]}"
        while read -r act; do
            eval "$act"
            tick
        done <<< "${actions[i]}"
        eval "${teardown[i]}"

        # Check each client for failure, print side-by-side diff if problems
        failed=0

        # Check each client for failure, print side-by-side diff if problems
        failed=0
        for ((c=0; c<${#outfiles[*]}; c++)); do
            outfile=${outfiles[c]}
            $CODEDIR/filter-semopen-bug.awk $outfile > x.tmp # appears that sem_open() bug is resolved
            mv x.tmp $outfile

            # Check various outputs from valgrind
            (grep -q 'ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)' $outfile &&   # look for 0 errors
             grep -q 'in use at exit: 0 bytes in 0 blocks'  $outfile)                               # look for in use at exit
            errs=$?                                                                                 # 0 return value if both match

            # special case where 'in use at exit' is likely; search only for errors
            if [[ "${tnames[i]}" == *"shutdown"* ]]; then
                (grep -q 'ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)' $outfile)
                errs=$?
            fi

            if [[ "$errs" != "0" ]]; then
                printf "errs: $errs\n"
                printf "FAIL\nValgrind detected problems in $outfile\n"
                major_sep
                cat $outfile
                major_sep
                failed=1
            fi
        done
        
        if [ "$failed" != "1" ]; then
            printf "ALL OK"
            ((NVALG++))
        fi
        printf "\n"
    done
    major_sep
    printf "Finished:\n"
    printf "%2d / %2d Valgrind correct\n" "$NVALG" "$VTESTS"
    printf "\n"

fi

major_sep
printf "OVERALL:\n"
printf "%2d / %2d Normal correct\n" "$NPASS" "$NTESTS"
printf "%2d / %2d Valgrind correct\n" "$NVALG" "$VTESTS"

