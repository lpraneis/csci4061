#!/bin/bash
T=0                             # global test number

# Basic test to check for help and exit
((T++))
tnames[T]="help-exit"
read  -r -d '' input[$T] <<"ENDIN"
help
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> help
COMMANDO COMMANDS
help               : show this message
exit               : exit the program
list               : list all jobs that have been started giving information on each
pause nanos secs   : pause for the given number of nanseconds and seconds
output-for int     : print the output for given job number
output-all         : print output for all jobs
wait-for int       : wait until the given job number finishes
wait-all           : wait for all jobs to finish
command arg1 ...   : non-built-in is run as a job
@> exit
ALERTS:
ENDOUT

# Check for presence of list
((T++))
tnames[T]="list-exit"
read  -r -d '' input[$T] <<"ENDIN"
list
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
@> exit
ALERTS:
ENDOUT

# Check for proper handling of end of input
((T++))
tnames[T]="end-of-input"
read  -r -d '' input[$T] <<"ENDIN"
help
list
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> help
COMMANDO COMMANDS
help               : show this message
exit               : exit the program
list               : list all jobs that have been started giving information on each
pause nanos secs   : pause for the given number of nanseconds and seconds
output-for int     : print the output for given job number
output-all         : print output for all jobs
wait-for int       : wait until the given job number finishes
wait-all           : wait for all jobs to finish
command arg1 ...   : non-built-in is run as a job
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
@> 
End of input
ALERTS:
ENDOUT

# Check for proper handling of blank lines
((T++))
tnames[T]="blank-lines"
read  -r -d '' input[$T] <<"ENDIN"
list


help

list

exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
@> 
@> 
@> help
COMMANDO COMMANDS
help               : show this message
exit               : exit the program
list               : list all jobs that have been started giving information on each
pause nanos secs   : pause for the given number of nanseconds and seconds
output-for int     : print the output for given job number
output-all         : print output for all jobs
wait-for int       : wait until the given job number finishes
wait-all           : wait for all jobs to finish
command arg1 ...   : non-built-in is run as a job
@> 
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
@> 
@> exit
ALERTS:
ENDOUT

# Simple test which runs an ls of a directory, waits for it, lists and
# outputs it
((T++))
tnames[T]="ls-stuff"
read  -r -d '' input[$T] <<"ENDIN"
ls -a -F test-data/stuff/
wait-for 0
list
output-for 0
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> ls -a -F test-data/stuff/
@> wait-for 0
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)   55 ls -a -F test-data/stuff/ 
@> output-for 0
@<<< Output for ls[%0] (55 bytes):
----------------------------------------
./
../
empty
gettysburg.txt
quote.txt
table.sh*
util.o
----------------------------------------
@> exit
ALERTS:
@!!! ls[%0]: EXIT(0)
ENDOUT

# Simple test which cats output of quote.txt
((T++))
tnames[T]="cat-quote"
read  -r -d '' input[$T] <<"ENDIN"
cat test-data/quote.txt
wait-for 0
output-for 0
list
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> cat test-data/quote.txt
@> wait-for 0
@> output-for 0
@<<< Output for cat[%0] (125 bytes):
----------------------------------------
Object-oriented programming is an exceptionally bad idea which could
only have originated in California.

-- Edsger Dijkstra
----------------------------------------
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)  125 cat test-data/quote.txt 
@> exit
ALERTS:
@!!! cat[%0]: EXIT(0)
ENDOUT

# Simple sleeps 1 second, no output
((T++))
tnames[T]="sleep-1"
read  -r -d '' input[$T] <<"ENDIN"
sleep 1
wait-for 0
output-for 0
list
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> sleep 1
@> wait-for 0
@> output-for 0
@<<< Output for sleep[%0] (0 bytes):
----------------------------------------
----------------------------------------
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)    0 sleep 1 
@> exit
ALERTS:
@!!! sleep[%0]: EXIT(0)
ENDOUT


# Run two ls calls, should have identical output but distinct pids
((T++))
tnames[T]="ls-multiple"
read  -r -d '' input[$T] <<"ENDIN"
ls -a -F test-data/stuff/
ls -a -F test-data/stuff/
wait-for 0
wait-for 1
list
output-for 0
output-for 1
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> ls -a -F test-data/stuff/
@> ls -a -F test-data/stuff/
@> wait-for 0
@> wait-for 1
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)   55 ls -a -F test-data/stuff/ 
1    %1           0    EXIT(0)   55 ls -a -F test-data/stuff/ 
@> output-for 0
@<<< Output for ls[%0] (55 bytes):
----------------------------------------
./
../
empty
gettysburg.txt
quote.txt
table.sh*
util.o
----------------------------------------
@> output-for 1
@<<< Output for ls[%1] (55 bytes):
----------------------------------------
./
../
empty
gettysburg.txt
quote.txt
table.sh*
util.o
----------------------------------------
@> exit
ALERTS:
@!!! ls[%0]: EXIT(0)
@!!! ls[%1]: EXIT(0)
ENDOUT

# Run two calls, ls stuff/ and table.sh
((T++))
tnames[T]="ls-table"
read  -r -d '' input[$T] <<"ENDIN"
ls -a -F test-data/stuff/
test-data/table.sh
wait-for 0
wait-for 1
list
output-for 0
output-for 1
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> ls -a -F test-data/stuff/
@> test-data/table.sh
@> wait-for 0
@> wait-for 1
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)   55 ls -a -F test-data/stuff/ 
1    %1           0    EXIT(0) 1140 test-data/table.sh 
@> output-for 0
@<<< Output for ls[%0] (55 bytes):
----------------------------------------
./
../
empty
gettysburg.txt
quote.txt
table.sh*
util.o
----------------------------------------
@> output-for 1
@<<< Output for test-data/table.sh[%1] (1140 bytes):
----------------------------------------
i^1=      1  i^2=      1  i^3=      1
i^1=      2  i^2=      4  i^3=      8
i^1=      3  i^2=      9  i^3=     27
i^1=      4  i^2=     16  i^3=     64
i^1=      5  i^2=     25  i^3=    125
i^1=      6  i^2=     36  i^3=    216
i^1=      7  i^2=     49  i^3=    343
i^1=      8  i^2=     64  i^3=    512
i^1=      9  i^2=     81  i^3=    729
i^1=     10  i^2=    100  i^3=   1000
i^1=     11  i^2=    121  i^3=   1331
i^1=     12  i^2=    144  i^3=   1728
i^1=     13  i^2=    169  i^3=   2197
i^1=     14  i^2=    196  i^3=   2744
i^1=     15  i^2=    225  i^3=   3375
i^1=     16  i^2=    256  i^3=   4096
i^1=     17  i^2=    289  i^3=   4913
i^1=     18  i^2=    324  i^3=   5832
i^1=     19  i^2=    361  i^3=   6859
i^1=     20  i^2=    400  i^3=   8000
i^1=     21  i^2=    441  i^3=   9261
i^1=     22  i^2=    484  i^3=  10648
i^1=     23  i^2=    529  i^3=  12167
i^1=     24  i^2=    576  i^3=  13824
i^1=     25  i^2=    625  i^3=  15625
i^1=     26  i^2=    676  i^3=  17576
i^1=     27  i^2=    729  i^3=  19683
i^1=     28  i^2=    784  i^3=  21952
i^1=     29  i^2=    841  i^3=  24389
i^1=     30  i^2=    900  i^3=  27000
----------------------------------------
@> exit
ALERTS:
@!!! ls[%0]: EXIT(0)
@!!! test-data/table.sh[%1]: EXIT(0)
ENDOUT


# Remove an executabl if present, compile it, then run it
((T++))
tnames[T]="compile-run"
read  -r -d '' input[$T] <<"ENDIN"
rm -f ./test-data/print_args
wait-for 0
gcc -o test-data/print_args test-data/print_args.c
wait-for 1
test-data/print_args hello goodbye so long
wait-for 2
list
output-for 0
output-for 1
output-for 2
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> rm -f ./test-data/print_args
@> wait-for 0
@> gcc -o test-data/print_args test-data/print_args.c
@> wait-for 1
@> test-data/print_args hello goodbye so long
@> wait-for 2
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)    0 rm -f ./test-data/print_args 
1    %1           0    EXIT(0)    0 gcc -o test-data/print_args test-data/print_args.c 
2    %2           0    EXIT(0)   74 test-data/print_args hello goodbye so long 
@> output-for 0
@<<< Output for rm[%0] (0 bytes):
----------------------------------------
----------------------------------------
@> output-for 1
@<<< Output for gcc[%1] (0 bytes):
----------------------------------------
----------------------------------------
@> output-for 2
@<<< Output for test-data/print_args[%2] (74 bytes):
----------------------------------------
5 args received
0: test-data/print_args
1: hello
2: goodbye
3: so
4: long
----------------------------------------
@> exit
ALERTS:
@!!! rm[%0]: EXIT(0)
@!!! gcc[%1]: EXIT(0)
@!!! test-data/print_args[%2]: EXIT(0)
ENDOUT

# Same as above but uses output-all
((T++))
tnames[T]="output-all"
read  -r -d '' input[$T] <<"ENDIN"
rm -f ./test-data/print_args
wait-for 0
gcc -o test-data/print_args test-data/print_args.c
wait-for 1
test-data/print_args hello goodbye so long
wait-for 2
list
output-all
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> rm -f ./test-data/print_args
@> wait-for 0
@> gcc -o test-data/print_args test-data/print_args.c
@> wait-for 1
@> test-data/print_args hello goodbye so long
@> wait-for 2
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)    0 rm -f ./test-data/print_args 
1    %1           0    EXIT(0)    0 gcc -o test-data/print_args test-data/print_args.c 
2    %2           0    EXIT(0)   74 test-data/print_args hello goodbye so long 
@> output-all
@<<< Output for rm[%0] (0 bytes):
----------------------------------------
----------------------------------------
@<<< Output for gcc[%1] (0 bytes):
----------------------------------------
----------------------------------------
@<<< Output for test-data/print_args[%2] (74 bytes):
----------------------------------------
5 args received
0: test-data/print_args
1: hello
2: goodbye
3: so
4: long
----------------------------------------
@> exit
ALERTS:
@!!! rm[%0]: EXIT(0)
@!!! gcc[%1]: EXIT(0)
@!!! test-data/print_args[%2]: EXIT(0)
ENDOUT

# Start several independent jobs then wait-all for them
((T++))
tnames[T]="wait-all"
read  -r -d '' input[$T] <<"ENDIN"
ls -a -F test-data/stuff/
gcc -o test-data/print_args test-data/print_args.c
cat test-data/quote.txt
cat test-data/gettysburg.txt
wait-all
list
output-all 
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> ls -a -F test-data/stuff/
@> gcc -o test-data/print_args test-data/print_args.c
@> cat test-data/quote.txt
@> cat test-data/gettysburg.txt
@> wait-all
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)   55 ls -a -F test-data/stuff/ 
1    %1           0    EXIT(0)    0 gcc -o test-data/print_args test-data/print_args.c 
2    %2           0    EXIT(0)  125 cat test-data/quote.txt 
3    %3           0    EXIT(0) 1511 cat test-data/gettysburg.txt 
@> output-all 
@<<< Output for ls[%0] (55 bytes):
----------------------------------------
./
../
empty
gettysburg.txt
quote.txt
table.sh*
util.o
----------------------------------------
@<<< Output for gcc[%1] (0 bytes):
----------------------------------------
----------------------------------------
@<<< Output for cat[%2] (125 bytes):
----------------------------------------
Object-oriented programming is an exceptionally bad idea which could
only have originated in California.

-- Edsger Dijkstra
----------------------------------------
@<<< Output for cat[%3] (1511 bytes):
----------------------------------------
Four score and seven years ago our fathers brought forth on this
continent, a new nation, conceived in Liberty, and dedicated to the
proposition that all men are created equal.

Now we are engaged in a great civil war, testing whether that nation,
or any nation so conceived and so dedicated, can long endure. We are
met on a great battle-field of that war. We have come to dedicate a
portion of that field, as a final resting place for those who here
gave their lives that that nation might live. It is altogether fitting
and proper that we should do this.

But, in a larger sense, we can not dedicate -- we can not consecrate
-- we can not hallow -- this ground. The brave men, living and dead,
who struggled here, have consecrated it, far above our poor power to
add or detract. The world will little note, nor long remember what we
say here, but it can never forget what they did here. It is for us the
living, rather, to be dedicated here to the unfinished work which they
who fought here have thus far so nobly advanced. It is rather for us
to be here dedicated to the great task remaining before us -- that
from these honored dead we take increased devotion to that cause for
which they gave the last full measure of devotion -- that we here
highly resolve that these dead shall not have died in vain -- that
this nation, under God, shall have a new birth of freedom -- and that
government of the people, by the people, for the people, shall not
perish from the earth.

Abraham Lincoln
November 19, 1863
----------------------------------------
@> exit
ALERTS:
@!!! ls[%0]: EXIT(0)
@!!! gcc[%1]: EXIT(0)
@!!! cat[%2]: EXIT(0)
@!!! cat[%3]: EXIT(0)
ENDOUT

# Initially output should be empty for jobs but filled in later after
# a wait-for; this test may be a little shakey
((T++))
tnames[T]="output-checks"
read  -r -d '' input[$T] <<"ENDIN"
cat test-data/quote.txt
list
output-for 0
wait-for 0
list
output-for 0
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> cat test-data/quote.txt
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0          -1        RUN   -1 cat test-data/quote.txt 
@> output-for 0
@<<< Output for cat[%0] (-1 bytes):
----------------------------------------
cat[%0] : output not ready
----------------------------------------
@> wait-for 0
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)  125 cat test-data/quote.txt 
@> output-for 0
@<<< Output for cat[%0] (125 bytes):
----------------------------------------
Object-oriented programming is an exceptionally bad idea which could
only have originated in California.

-- Edsger Dijkstra
----------------------------------------
@> exit
ALERTS:
@!!! cat[%0]: EXIT(0)
ENDOUT


# Check that pause is implemented
((T++))
tnames[T]="pause-present"
read  -r -d '' input[$T] <<"ENDIN"
list
pause 10000 0
pause 0 1
list
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
@> pause 10000 0
@> pause 0 1
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
@> exit
ALERTS:
ENDOUT

# Pause should allow short jobs to finish
((T++))
tnames[T]="pause-works"
read  -r -d '' input[$T] <<"ENDIN"
cat test-data/quote.txt
pause 500000000 0
list
output-for 0
cat test-data/quote.txt
pause 0 1
list
output-for 1
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> cat test-data/quote.txt
@> pause 500000000 0
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)  125 cat test-data/quote.txt 
@> output-for 0
@<<< Output for cat[%0] (125 bytes):
----------------------------------------
Object-oriented programming is an exceptionally bad idea which could
only have originated in California.

-- Edsger Dijkstra
----------------------------------------
@> cat test-data/quote.txt
@> pause 0 1
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)  125 cat test-data/quote.txt 
1    %1           0    EXIT(0)  125 cat test-data/quote.txt 
@> output-for 1
@<<< Output for cat[%1] (125 bytes):
----------------------------------------
Object-oriented programming is an exceptionally bad idea which could
only have originated in California.

-- Edsger Dijkstra
----------------------------------------
@> exit
ALERTS:
@!!! cat[%0]: EXIT(0)
@!!! cat[%1]: EXIT(0)
ENDOUT

# Multiple jobs should finish after a medium pause
((T++))
tnames[T]="pause-medium"
read  -r -d '' input[$T] <<"ENDIN"
cat test-data/quote.txt
cat test-data/gettysburg.txt
grep printf test-data/print_args.c
pause 500000000 0
list
output-all
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> cat test-data/quote.txt
@> cat test-data/gettysburg.txt
@> grep printf test-data/print_args.c
@> pause 500000000 0
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)  125 cat test-data/quote.txt 
1    %1           0    EXIT(0) 1511 cat test-data/gettysburg.txt 
2    %2           0    EXIT(0)   71 grep printf test-data/print_args.c 
@> output-all
@<<< Output for cat[%0] (125 bytes):
----------------------------------------
Object-oriented programming is an exceptionally bad idea which could
only have originated in California.

-- Edsger Dijkstra
----------------------------------------
@<<< Output for cat[%1] (1511 bytes):
----------------------------------------
Four score and seven years ago our fathers brought forth on this
continent, a new nation, conceived in Liberty, and dedicated to the
proposition that all men are created equal.

Now we are engaged in a great civil war, testing whether that nation,
or any nation so conceived and so dedicated, can long endure. We are
met on a great battle-field of that war. We have come to dedicate a
portion of that field, as a final resting place for those who here
gave their lives that that nation might live. It is altogether fitting
and proper that we should do this.

But, in a larger sense, we can not dedicate -- we can not consecrate
-- we can not hallow -- this ground. The brave men, living and dead,
who struggled here, have consecrated it, far above our poor power to
add or detract. The world will little note, nor long remember what we
say here, but it can never forget what they did here. It is for us the
living, rather, to be dedicated here to the unfinished work which they
who fought here have thus far so nobly advanced. It is rather for us
to be here dedicated to the great task remaining before us -- that
from these honored dead we take increased devotion to that cause for
which they gave the last full measure of devotion -- that we here
highly resolve that these dead shall not have died in vain -- that
this nation, under God, shall have a new birth of freedom -- and that
government of the people, by the people, for the people, shall not
perish from the earth.

Abraham Lincoln
November 19, 1863
----------------------------------------
@<<< Output for grep[%2] (71 bytes):
----------------------------------------
  printf("%d args received\n",argc);
    printf("%d: %s\n",i,argv[i]);
----------------------------------------
@> exit
ALERTS:
@!!! cat[%0]: EXIT(0)
@!!! cat[%1]: EXIT(0)
@!!! grep[%2]: EXIT(0)
ENDOUT

# Longer jobs should not finish during the pause
((T++))
tnames[T]="pause-not-done"
read  -r -d '' input[$T] <<"ENDIN"
cat test-data/quote.txt
test-data/table.sh 20 2
sleep 2
cat test-data/gettysburg.txt
grep printf test-data/print_args.c
pause 500000000 0
list
output-all
wait-all
list
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> cat test-data/quote.txt
@> test-data/table.sh 20 2
@> sleep 2
@> cat test-data/gettysburg.txt
@> grep printf test-data/print_args.c
@> pause 500000000 0
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)  125 cat test-data/quote.txt 
1    %3          -1        RUN   -1 test-data/table.sh 20 2 
2    %4          -1        RUN   -1 sleep 2 
3    %1           0    EXIT(0) 1511 cat test-data/gettysburg.txt 
4    %2           0    EXIT(0)   71 grep printf test-data/print_args.c 
@> output-all
@<<< Output for cat[%0] (125 bytes):
----------------------------------------
Object-oriented programming is an exceptionally bad idea which could
only have originated in California.

-- Edsger Dijkstra
----------------------------------------
@<<< Output for test-data/table.sh[%3] (-1 bytes):
----------------------------------------
test-data/table.sh[%3] : output not ready
----------------------------------------
@<<< Output for sleep[%4] (-1 bytes):
----------------------------------------
sleep[%4] : output not ready
----------------------------------------
@<<< Output for cat[%1] (1511 bytes):
----------------------------------------
Four score and seven years ago our fathers brought forth on this
continent, a new nation, conceived in Liberty, and dedicated to the
proposition that all men are created equal.

Now we are engaged in a great civil war, testing whether that nation,
or any nation so conceived and so dedicated, can long endure. We are
met on a great battle-field of that war. We have come to dedicate a
portion of that field, as a final resting place for those who here
gave their lives that that nation might live. It is altogether fitting
and proper that we should do this.

But, in a larger sense, we can not dedicate -- we can not consecrate
-- we can not hallow -- this ground. The brave men, living and dead,
who struggled here, have consecrated it, far above our poor power to
add or detract. The world will little note, nor long remember what we
say here, but it can never forget what they did here. It is for us the
living, rather, to be dedicated here to the unfinished work which they
who fought here have thus far so nobly advanced. It is rather for us
to be here dedicated to the great task remaining before us -- that
from these honored dead we take increased devotion to that cause for
which they gave the last full measure of devotion -- that we here
highly resolve that these dead shall not have died in vain -- that
this nation, under God, shall have a new birth of freedom -- and that
government of the people, by the people, for the people, shall not
perish from the earth.

Abraham Lincoln
November 19, 1863
----------------------------------------
@<<< Output for grep[%2] (71 bytes):
----------------------------------------
  printf("%d args received\n",argc);
    printf("%d: %s\n",i,argv[i]);
----------------------------------------
@> wait-all
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0)  125 cat test-data/quote.txt 
1    %3           0    EXIT(0)  760 test-data/table.sh 20 2 
2    %4           0    EXIT(0)    0 sleep 2 
3    %1           0    EXIT(0) 1511 cat test-data/gettysburg.txt 
4    %2           0    EXIT(0)   71 grep printf test-data/print_args.c 
@> exit
ALERTS:
@!!! cat[%0]: EXIT(0)
@!!! cat[%1]: EXIT(0)
@!!! grep[%2]: EXIT(0)
@!!! test-data/table.sh[%3]: EXIT(0)
@!!! sleep[%4]: EXIT(0)
ENDOUT


# Check that wait-for waits for individual jobs
((T++))
tnames[T]="wait-coord"
read  -r -d '' input[$T] <<"ENDIN"
sleep 1
sleep 3
sleep 2
wait-for 0
output-for 0
output-for 1
wait-for 2
output-for 2
output-for 1
wait-all
output-for 1
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> sleep 1
@> sleep 3
@> sleep 2
@> wait-for 0
@> output-for 0
@<<< Output for sleep[%0] (0 bytes):
----------------------------------------
----------------------------------------
@> output-for 1
@<<< Output for sleep[%1] (-1 bytes):
----------------------------------------
sleep[%1] : output not ready
----------------------------------------
@> wait-for 2
@> output-for 2
@<<< Output for sleep[%2] (0 bytes):
----------------------------------------
----------------------------------------
@> output-for 1
@<<< Output for sleep[%1] (-1 bytes):
----------------------------------------
sleep[%1] : output not ready
----------------------------------------
@> wait-all
@> output-for 1
@<<< Output for sleep[%1] (0 bytes):
----------------------------------------
----------------------------------------
@> exit
ALERTS:
@!!! sleep[%0]: EXIT(0)
@!!! sleep[%2]: EXIT(0)
@!!! sleep[%1]: EXIT(0)
ENDOUT

# Somewhat involved test with a variety of commands
((T++))
tnames[T]="stress1"
read  -r -d '' input[$T] <<"ENDIN"
test-data/table.sh 50 2
test-data/table.sh 40 0
sleep 2
list
cat test-data/print_args.c
pause 0 1
output-for 1
output-all
list
wait-all
list
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> test-data/table.sh 50 2
@> test-data/table.sh 40 0
@> sleep 2
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0          -1        RUN   -1 test-data/table.sh 50 2 
1    %1          -1        RUN   -1 test-data/table.sh 40 0 
2    %2          -1        RUN   -1 sleep 2 
@> cat test-data/print_args.c
@> pause 0 1
@> output-for 1
@<<< Output for test-data/table.sh[%1] (1520 bytes):
----------------------------------------
i^1=      1  i^2=      1  i^3=      1
i^1=      2  i^2=      4  i^3=      8
i^1=      3  i^2=      9  i^3=     27
i^1=      4  i^2=     16  i^3=     64
i^1=      5  i^2=     25  i^3=    125
i^1=      6  i^2=     36  i^3=    216
i^1=      7  i^2=     49  i^3=    343
i^1=      8  i^2=     64  i^3=    512
i^1=      9  i^2=     81  i^3=    729
i^1=     10  i^2=    100  i^3=   1000
i^1=     11  i^2=    121  i^3=   1331
i^1=     12  i^2=    144  i^3=   1728
i^1=     13  i^2=    169  i^3=   2197
i^1=     14  i^2=    196  i^3=   2744
i^1=     15  i^2=    225  i^3=   3375
i^1=     16  i^2=    256  i^3=   4096
i^1=     17  i^2=    289  i^3=   4913
i^1=     18  i^2=    324  i^3=   5832
i^1=     19  i^2=    361  i^3=   6859
i^1=     20  i^2=    400  i^3=   8000
i^1=     21  i^2=    441  i^3=   9261
i^1=     22  i^2=    484  i^3=  10648
i^1=     23  i^2=    529  i^3=  12167
i^1=     24  i^2=    576  i^3=  13824
i^1=     25  i^2=    625  i^3=  15625
i^1=     26  i^2=    676  i^3=  17576
i^1=     27  i^2=    729  i^3=  19683
i^1=     28  i^2=    784  i^3=  21952
i^1=     29  i^2=    841  i^3=  24389
i^1=     30  i^2=    900  i^3=  27000
i^1=     31  i^2=    961  i^3=  29791
i^1=     32  i^2=   1024  i^3=  32768
i^1=     33  i^2=   1089  i^3=  35937
i^1=     34  i^2=   1156  i^3=  39304
i^1=     35  i^2=   1225  i^3=  42875
i^1=     36  i^2=   1296  i^3=  46656
i^1=     37  i^2=   1369  i^3=  50653
i^1=     38  i^2=   1444  i^3=  54872
i^1=     39  i^2=   1521  i^3=  59319
i^1=     40  i^2=   1600  i^3=  64000
----------------------------------------
@> output-all
@<<< Output for test-data/table.sh[%0] (-1 bytes):
----------------------------------------
test-data/table.sh[%0] : output not ready
----------------------------------------
@<<< Output for test-data/table.sh[%1] (1520 bytes):
----------------------------------------
i^1=      1  i^2=      1  i^3=      1
i^1=      2  i^2=      4  i^3=      8
i^1=      3  i^2=      9  i^3=     27
i^1=      4  i^2=     16  i^3=     64
i^1=      5  i^2=     25  i^3=    125
i^1=      6  i^2=     36  i^3=    216
i^1=      7  i^2=     49  i^3=    343
i^1=      8  i^2=     64  i^3=    512
i^1=      9  i^2=     81  i^3=    729
i^1=     10  i^2=    100  i^3=   1000
i^1=     11  i^2=    121  i^3=   1331
i^1=     12  i^2=    144  i^3=   1728
i^1=     13  i^2=    169  i^3=   2197
i^1=     14  i^2=    196  i^3=   2744
i^1=     15  i^2=    225  i^3=   3375
i^1=     16  i^2=    256  i^3=   4096
i^1=     17  i^2=    289  i^3=   4913
i^1=     18  i^2=    324  i^3=   5832
i^1=     19  i^2=    361  i^3=   6859
i^1=     20  i^2=    400  i^3=   8000
i^1=     21  i^2=    441  i^3=   9261
i^1=     22  i^2=    484  i^3=  10648
i^1=     23  i^2=    529  i^3=  12167
i^1=     24  i^2=    576  i^3=  13824
i^1=     25  i^2=    625  i^3=  15625
i^1=     26  i^2=    676  i^3=  17576
i^1=     27  i^2=    729  i^3=  19683
i^1=     28  i^2=    784  i^3=  21952
i^1=     29  i^2=    841  i^3=  24389
i^1=     30  i^2=    900  i^3=  27000
i^1=     31  i^2=    961  i^3=  29791
i^1=     32  i^2=   1024  i^3=  32768
i^1=     33  i^2=   1089  i^3=  35937
i^1=     34  i^2=   1156  i^3=  39304
i^1=     35  i^2=   1225  i^3=  42875
i^1=     36  i^2=   1296  i^3=  46656
i^1=     37  i^2=   1369  i^3=  50653
i^1=     38  i^2=   1444  i^3=  54872
i^1=     39  i^2=   1521  i^3=  59319
i^1=     40  i^2=   1600  i^3=  64000
----------------------------------------
@<<< Output for sleep[%2] (-1 bytes):
----------------------------------------
sleep[%2] : output not ready
----------------------------------------
@<<< Output for cat[%3] (218 bytes):
----------------------------------------
// Print all the arguments in the argv array

#include <stdio.h>

int main(int argc, char *argv[]){
  printf("%d args received\n",argc);
  for(int i=0; i<argc; i++){
    printf("%d: %s\n",i,argv[i]);
  }
  return 0;
}
----------------------------------------
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0          -1        RUN   -1 test-data/table.sh 50 2 
1    %1           0    EXIT(0) 1520 test-data/table.sh 40 0 
2    %2          -1        RUN   -1 sleep 2 
3    %3           0    EXIT(0)  218 cat test-data/print_args.c 
@> wait-all
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0) 1900 test-data/table.sh 50 2 
1    %1           0    EXIT(0) 1520 test-data/table.sh 40 0 
2    %2           0    EXIT(0)    0 sleep 2 
3    %3           0    EXIT(0)  218 cat test-data/print_args.c 
@> exit
ALERTS:
@!!! test-data/table.sh[%1]: EXIT(0)
@!!! cat[%3]: EXIT(0)
@!!! test-data/table.sh[%0]: EXIT(0)
@!!! sleep[%2]: EXIT(0)
ENDOUT

# Somewhat involved test with a variety of commands
((T++))
tnames[T]="stress2"
read  -r -d '' input[$T] <<"ENDIN"
test-data/table.sh 100 1
test-data/table.sh 100 2
test-data/table.sh 50 3
grep flurbo test-data/gettysburg.txt
list
ls -1 -a -F test-data/stuff/
cat test-data/print_args.c
pause 0 1
output-all
grep -n the test-data/gettysburg.txt
grep -n the test-data/quote.txt
list
wait-all
list
output-all
exit
ENDIN

read  -r -d '' output[$T] <<"ENDOUT"
@> test-data/table.sh 100 1
@> test-data/table.sh 100 2
@> test-data/table.sh 50 3
@> grep flurbo test-data/gettysburg.txt
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0          -1        RUN   -1 test-data/table.sh 100 1 
1    %1          -1        RUN   -1 test-data/table.sh 100 2 
2    %2          -1        RUN   -1 test-data/table.sh 50 3 
3    %3          -1        RUN   -1 grep flurbo test-data/gettysburg.txt 
@> ls -1 -a -F test-data/stuff/
@> cat test-data/print_args.c
@> pause 0 1
@> output-all
@<<< Output for test-data/table.sh[%0] (-1 bytes):
----------------------------------------
test-data/table.sh[%0] : output not ready
----------------------------------------
@<<< Output for test-data/table.sh[%1] (-1 bytes):
----------------------------------------
test-data/table.sh[%1] : output not ready
----------------------------------------
@<<< Output for test-data/table.sh[%2] (-1 bytes):
----------------------------------------
test-data/table.sh[%2] : output not ready
----------------------------------------
@<<< Output for grep[%3] (0 bytes):
----------------------------------------
----------------------------------------
@<<< Output for ls[%4] (55 bytes):
----------------------------------------
./
../
empty
gettysburg.txt
quote.txt
table.sh*
util.o
----------------------------------------
@<<< Output for cat[%5] (218 bytes):
----------------------------------------
// Print all the arguments in the argv array

#include <stdio.h>

int main(int argc, char *argv[]){
  printf("%d args received\n",argc);
  for(int i=0; i<argc; i++){
    printf("%d: %s\n",i,argv[i]);
  }
  return 0;
}
----------------------------------------
@> grep -n the test-data/gettysburg.txt
@> grep -n the test-data/quote.txt
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0          -1        RUN   -1 test-data/table.sh 100 1 
1    %1          -1        RUN   -1 test-data/table.sh 100 2 
2    %2          -1        RUN   -1 test-data/table.sh 50 3 
3    %3           1    EXIT(1)    0 grep flurbo test-data/gettysburg.txt 
4    %4           0    EXIT(0)   55 ls -1 -a -F test-data/stuff/ 
5    %5           0    EXIT(0)  218 cat test-data/print_args.c 
6    %6          -1        RUN   -1 grep -n the test-data/gettysburg.txt 
7    %7          -1        RUN   -1 grep -n the test-data/quote.txt 
@> wait-all
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    %0           0    EXIT(0) 3801 test-data/table.sh 100 1 
1    %1           0    EXIT(0) 3801 test-data/table.sh 100 2 
2    %2           0    EXIT(0) 1900 test-data/table.sh 50 3 
3    %3           1    EXIT(1)    0 grep flurbo test-data/gettysburg.txt 
4    %4           0    EXIT(0)   55 ls -1 -a -F test-data/stuff/ 
5    %5           0    EXIT(0)  218 cat test-data/print_args.c 
6    %6           0    EXIT(0)  879 grep -n the test-data/gettysburg.txt 
7    %7           1    EXIT(1)    0 grep -n the test-data/quote.txt 
@> output-all
@<<< Output for test-data/table.sh[%0] (3801 bytes):
----------------------------------------
i^1=      1  i^2=      1  i^3=      1
i^1=      2  i^2=      4  i^3=      8
i^1=      3  i^2=      9  i^3=     27
i^1=      4  i^2=     16  i^3=     64
i^1=      5  i^2=     25  i^3=    125
i^1=      6  i^2=     36  i^3=    216
i^1=      7  i^2=     49  i^3=    343
i^1=      8  i^2=     64  i^3=    512
i^1=      9  i^2=     81  i^3=    729
i^1=     10  i^2=    100  i^3=   1000
i^1=     11  i^2=    121  i^3=   1331
i^1=     12  i^2=    144  i^3=   1728
i^1=     13  i^2=    169  i^3=   2197
i^1=     14  i^2=    196  i^3=   2744
i^1=     15  i^2=    225  i^3=   3375
i^1=     16  i^2=    256  i^3=   4096
i^1=     17  i^2=    289  i^3=   4913
i^1=     18  i^2=    324  i^3=   5832
i^1=     19  i^2=    361  i^3=   6859
i^1=     20  i^2=    400  i^3=   8000
i^1=     21  i^2=    441  i^3=   9261
i^1=     22  i^2=    484  i^3=  10648
i^1=     23  i^2=    529  i^3=  12167
i^1=     24  i^2=    576  i^3=  13824
i^1=     25  i^2=    625  i^3=  15625
i^1=     26  i^2=    676  i^3=  17576
i^1=     27  i^2=    729  i^3=  19683
i^1=     28  i^2=    784  i^3=  21952
i^1=     29  i^2=    841  i^3=  24389
i^1=     30  i^2=    900  i^3=  27000
i^1=     31  i^2=    961  i^3=  29791
i^1=     32  i^2=   1024  i^3=  32768
i^1=     33  i^2=   1089  i^3=  35937
i^1=     34  i^2=   1156  i^3=  39304
i^1=     35  i^2=   1225  i^3=  42875
i^1=     36  i^2=   1296  i^3=  46656
i^1=     37  i^2=   1369  i^3=  50653
i^1=     38  i^2=   1444  i^3=  54872
i^1=     39  i^2=   1521  i^3=  59319
i^1=     40  i^2=   1600  i^3=  64000
i^1=     41  i^2=   1681  i^3=  68921
i^1=     42  i^2=   1764  i^3=  74088
i^1=     43  i^2=   1849  i^3=  79507
i^1=     44  i^2=   1936  i^3=  85184
i^1=     45  i^2=   2025  i^3=  91125
i^1=     46  i^2=   2116  i^3=  97336
i^1=     47  i^2=   2209  i^3= 103823
i^1=     48  i^2=   2304  i^3= 110592
i^1=     49  i^2=   2401  i^3= 117649
i^1=     50  i^2=   2500  i^3= 125000
i^1=     51  i^2=   2601  i^3= 132651
i^1=     52  i^2=   2704  i^3= 140608
i^1=     53  i^2=   2809  i^3= 148877
i^1=     54  i^2=   2916  i^3= 157464
i^1=     55  i^2=   3025  i^3= 166375
i^1=     56  i^2=   3136  i^3= 175616
i^1=     57  i^2=   3249  i^3= 185193
i^1=     58  i^2=   3364  i^3= 195112
i^1=     59  i^2=   3481  i^3= 205379
i^1=     60  i^2=   3600  i^3= 216000
i^1=     61  i^2=   3721  i^3= 226981
i^1=     62  i^2=   3844  i^3= 238328
i^1=     63  i^2=   3969  i^3= 250047
i^1=     64  i^2=   4096  i^3= 262144
i^1=     65  i^2=   4225  i^3= 274625
i^1=     66  i^2=   4356  i^3= 287496
i^1=     67  i^2=   4489  i^3= 300763
i^1=     68  i^2=   4624  i^3= 314432
i^1=     69  i^2=   4761  i^3= 328509
i^1=     70  i^2=   4900  i^3= 343000
i^1=     71  i^2=   5041  i^3= 357911
i^1=     72  i^2=   5184  i^3= 373248
i^1=     73  i^2=   5329  i^3= 389017
i^1=     74  i^2=   5476  i^3= 405224
i^1=     75  i^2=   5625  i^3= 421875
i^1=     76  i^2=   5776  i^3= 438976
i^1=     77  i^2=   5929  i^3= 456533
i^1=     78  i^2=   6084  i^3= 474552
i^1=     79  i^2=   6241  i^3= 493039
i^1=     80  i^2=   6400  i^3= 512000
i^1=     81  i^2=   6561  i^3= 531441
i^1=     82  i^2=   6724  i^3= 551368
i^1=     83  i^2=   6889  i^3= 571787
i^1=     84  i^2=   7056  i^3= 592704
i^1=     85  i^2=   7225  i^3= 614125
i^1=     86  i^2=   7396  i^3= 636056
i^1=     87  i^2=   7569  i^3= 658503
i^1=     88  i^2=   7744  i^3= 681472
i^1=     89  i^2=   7921  i^3= 704969
i^1=     90  i^2=   8100  i^3= 729000
i^1=     91  i^2=   8281  i^3= 753571
i^1=     92  i^2=   8464  i^3= 778688
i^1=     93  i^2=   8649  i^3= 804357
i^1=     94  i^2=   8836  i^3= 830584
i^1=     95  i^2=   9025  i^3= 857375
i^1=     96  i^2=   9216  i^3= 884736
i^1=     97  i^2=   9409  i^3= 912673
i^1=     98  i^2=   9604  i^3= 941192
i^1=     99  i^2=   9801  i^3= 970299
i^1=    100  i^2=  10000  i^3= 1000000
----------------------------------------
@<<< Output for test-data/table.sh[%1] (3801 bytes):
----------------------------------------
i^1=      1  i^2=      1  i^3=      1
i^1=      2  i^2=      4  i^3=      8
i^1=      3  i^2=      9  i^3=     27
i^1=      4  i^2=     16  i^3=     64
i^1=      5  i^2=     25  i^3=    125
i^1=      6  i^2=     36  i^3=    216
i^1=      7  i^2=     49  i^3=    343
i^1=      8  i^2=     64  i^3=    512
i^1=      9  i^2=     81  i^3=    729
i^1=     10  i^2=    100  i^3=   1000
i^1=     11  i^2=    121  i^3=   1331
i^1=     12  i^2=    144  i^3=   1728
i^1=     13  i^2=    169  i^3=   2197
i^1=     14  i^2=    196  i^3=   2744
i^1=     15  i^2=    225  i^3=   3375
i^1=     16  i^2=    256  i^3=   4096
i^1=     17  i^2=    289  i^3=   4913
i^1=     18  i^2=    324  i^3=   5832
i^1=     19  i^2=    361  i^3=   6859
i^1=     20  i^2=    400  i^3=   8000
i^1=     21  i^2=    441  i^3=   9261
i^1=     22  i^2=    484  i^3=  10648
i^1=     23  i^2=    529  i^3=  12167
i^1=     24  i^2=    576  i^3=  13824
i^1=     25  i^2=    625  i^3=  15625
i^1=     26  i^2=    676  i^3=  17576
i^1=     27  i^2=    729  i^3=  19683
i^1=     28  i^2=    784  i^3=  21952
i^1=     29  i^2=    841  i^3=  24389
i^1=     30  i^2=    900  i^3=  27000
i^1=     31  i^2=    961  i^3=  29791
i^1=     32  i^2=   1024  i^3=  32768
i^1=     33  i^2=   1089  i^3=  35937
i^1=     34  i^2=   1156  i^3=  39304
i^1=     35  i^2=   1225  i^3=  42875
i^1=     36  i^2=   1296  i^3=  46656
i^1=     37  i^2=   1369  i^3=  50653
i^1=     38  i^2=   1444  i^3=  54872
i^1=     39  i^2=   1521  i^3=  59319
i^1=     40  i^2=   1600  i^3=  64000
i^1=     41  i^2=   1681  i^3=  68921
i^1=     42  i^2=   1764  i^3=  74088
i^1=     43  i^2=   1849  i^3=  79507
i^1=     44  i^2=   1936  i^3=  85184
i^1=     45  i^2=   2025  i^3=  91125
i^1=     46  i^2=   2116  i^3=  97336
i^1=     47  i^2=   2209  i^3= 103823
i^1=     48  i^2=   2304  i^3= 110592
i^1=     49  i^2=   2401  i^3= 117649
i^1=     50  i^2=   2500  i^3= 125000
i^1=     51  i^2=   2601  i^3= 132651
i^1=     52  i^2=   2704  i^3= 140608
i^1=     53  i^2=   2809  i^3= 148877
i^1=     54  i^2=   2916  i^3= 157464
i^1=     55  i^2=   3025  i^3= 166375
i^1=     56  i^2=   3136  i^3= 175616
i^1=     57  i^2=   3249  i^3= 185193
i^1=     58  i^2=   3364  i^3= 195112
i^1=     59  i^2=   3481  i^3= 205379
i^1=     60  i^2=   3600  i^3= 216000
i^1=     61  i^2=   3721  i^3= 226981
i^1=     62  i^2=   3844  i^3= 238328
i^1=     63  i^2=   3969  i^3= 250047
i^1=     64  i^2=   4096  i^3= 262144
i^1=     65  i^2=   4225  i^3= 274625
i^1=     66  i^2=   4356  i^3= 287496
i^1=     67  i^2=   4489  i^3= 300763
i^1=     68  i^2=   4624  i^3= 314432
i^1=     69  i^2=   4761  i^3= 328509
i^1=     70  i^2=   4900  i^3= 343000
i^1=     71  i^2=   5041  i^3= 357911
i^1=     72  i^2=   5184  i^3= 373248
i^1=     73  i^2=   5329  i^3= 389017
i^1=     74  i^2=   5476  i^3= 405224
i^1=     75  i^2=   5625  i^3= 421875
i^1=     76  i^2=   5776  i^3= 438976
i^1=     77  i^2=   5929  i^3= 456533
i^1=     78  i^2=   6084  i^3= 474552
i^1=     79  i^2=   6241  i^3= 493039
i^1=     80  i^2=   6400  i^3= 512000
i^1=     81  i^2=   6561  i^3= 531441
i^1=     82  i^2=   6724  i^3= 551368
i^1=     83  i^2=   6889  i^3= 571787
i^1=     84  i^2=   7056  i^3= 592704
i^1=     85  i^2=   7225  i^3= 614125
i^1=     86  i^2=   7396  i^3= 636056
i^1=     87  i^2=   7569  i^3= 658503
i^1=     88  i^2=   7744  i^3= 681472
i^1=     89  i^2=   7921  i^3= 704969
i^1=     90  i^2=   8100  i^3= 729000
i^1=     91  i^2=   8281  i^3= 753571
i^1=     92  i^2=   8464  i^3= 778688
i^1=     93  i^2=   8649  i^3= 804357
i^1=     94  i^2=   8836  i^3= 830584
i^1=     95  i^2=   9025  i^3= 857375
i^1=     96  i^2=   9216  i^3= 884736
i^1=     97  i^2=   9409  i^3= 912673
i^1=     98  i^2=   9604  i^3= 941192
i^1=     99  i^2=   9801  i^3= 970299
i^1=    100  i^2=  10000  i^3= 1000000
----------------------------------------
@<<< Output for test-data/table.sh[%2] (1900 bytes):
----------------------------------------
i^1=      1  i^2=      1  i^3=      1
i^1=      2  i^2=      4  i^3=      8
i^1=      3  i^2=      9  i^3=     27
i^1=      4  i^2=     16  i^3=     64
i^1=      5  i^2=     25  i^3=    125
i^1=      6  i^2=     36  i^3=    216
i^1=      7  i^2=     49  i^3=    343
i^1=      8  i^2=     64  i^3=    512
i^1=      9  i^2=     81  i^3=    729
i^1=     10  i^2=    100  i^3=   1000
i^1=     11  i^2=    121  i^3=   1331
i^1=     12  i^2=    144  i^3=   1728
i^1=     13  i^2=    169  i^3=   2197
i^1=     14  i^2=    196  i^3=   2744
i^1=     15  i^2=    225  i^3=   3375
i^1=     16  i^2=    256  i^3=   4096
i^1=     17  i^2=    289  i^3=   4913
i^1=     18  i^2=    324  i^3=   5832
i^1=     19  i^2=    361  i^3=   6859
i^1=     20  i^2=    400  i^3=   8000
i^1=     21  i^2=    441  i^3=   9261
i^1=     22  i^2=    484  i^3=  10648
i^1=     23  i^2=    529  i^3=  12167
i^1=     24  i^2=    576  i^3=  13824
i^1=     25  i^2=    625  i^3=  15625
i^1=     26  i^2=    676  i^3=  17576
i^1=     27  i^2=    729  i^3=  19683
i^1=     28  i^2=    784  i^3=  21952
i^1=     29  i^2=    841  i^3=  24389
i^1=     30  i^2=    900  i^3=  27000
i^1=     31  i^2=    961  i^3=  29791
i^1=     32  i^2=   1024  i^3=  32768
i^1=     33  i^2=   1089  i^3=  35937
i^1=     34  i^2=   1156  i^3=  39304
i^1=     35  i^2=   1225  i^3=  42875
i^1=     36  i^2=   1296  i^3=  46656
i^1=     37  i^2=   1369  i^3=  50653
i^1=     38  i^2=   1444  i^3=  54872
i^1=     39  i^2=   1521  i^3=  59319
i^1=     40  i^2=   1600  i^3=  64000
i^1=     41  i^2=   1681  i^3=  68921
i^1=     42  i^2=   1764  i^3=  74088
i^1=     43  i^2=   1849  i^3=  79507
i^1=     44  i^2=   1936  i^3=  85184
i^1=     45  i^2=   2025  i^3=  91125
i^1=     46  i^2=   2116  i^3=  97336
i^1=     47  i^2=   2209  i^3= 103823
i^1=     48  i^2=   2304  i^3= 110592
i^1=     49  i^2=   2401  i^3= 117649
i^1=     50  i^2=   2500  i^3= 125000
----------------------------------------
@<<< Output for grep[%3] (0 bytes):
----------------------------------------
----------------------------------------
@<<< Output for ls[%4] (55 bytes):
----------------------------------------
./
../
empty
gettysburg.txt
quote.txt
table.sh*
util.o
----------------------------------------
@<<< Output for cat[%5] (218 bytes):
----------------------------------------
// Print all the arguments in the argv array

#include <stdio.h>

int main(int argc, char *argv[]){
  printf("%d args received\n",argc);
  for(int i=0; i<argc; i++){
    printf("%d: %s\n",i,argv[i]);
  }
  return 0;
}
----------------------------------------
@<<< Output for grep[%6] (879 bytes):
----------------------------------------
1:Four score and seven years ago our fathers brought forth on this
2:continent, a new nation, conceived in Liberty, and dedicated to the
5:Now we are engaged in a great civil war, testing whether that nation,
9:gave their lives that that nation might live. It is altogether fitting
16:say here, but it can never forget what they did here. It is for us the
17:living, rather, to be dedicated here to the unfinished work which they
18:who fought here have thus far so nobly advanced. It is rather for us
19:to be here dedicated to the great task remaining before us -- that
20:from these honored dead we take increased devotion to that cause for
21:which they gave the last full measure of devotion -- that we here
22:highly resolve that these dead shall not have died in vain -- that
24:government of the people, by the people, for the people, shall not
25:perish from the earth.
----------------------------------------
@<<< Output for grep[%7] (0 bytes):
----------------------------------------
----------------------------------------
@> exit
ALERTS:
@!!! grep[%3]: EXIT(1)
@!!! ls[%4]: EXIT(0)
@!!! cat[%5]: EXIT(0)
@!!! test-data/table.sh[%0]: EXIT(0)
@!!! test-data/table.sh[%1]: EXIT(0)
@!!! test-data/table.sh[%2]: EXIT(0)
@!!! grep[%6]: EXIT(0)
@!!! grep[%7]: EXIT(1)
ENDOUT
