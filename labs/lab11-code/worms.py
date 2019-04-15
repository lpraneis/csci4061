#!/bin/python

# prototype curses implementation of worms in threads

import curses
from threading import Thread, RLock
from time import sleep
import random
import time
import sys
import os

height = 10
width  = 10

random.seed()

# set up the display for ncurses
screen = curses.initscr()
worm_box = curses.newwin(height+1, width+1)                               # generate borderd Box
worm_box.box()                                                            
screen.refresh()
worm_box.refresh()
curses.start_color()
curses.use_default_colors()                                               # use terminal background

# locks for each grid point in the worm box; RLock allows "recursive"
# locks, same thread can relock it which simplifies logic
mutex = [[RLock() for j in range(20)] for i in range(20)]


# function run by worms
def worm(my_id,row,col,color,delay):
    curses.init_pair(my_id, color, -1);
    moves = [(+1,0),(-1,0),(0,+1),(0,-1)]                                 # move up/down/left/right

    while (1):                                                            # main thread work loop which will draw on screen forever
        while (1):                                                        # try different moves until an in-bound move is found
            (dr,dc) = random.choice(moves);                               # pick a random delta
            (new_row, new_col) = (row+dr, col+dc)                         # calculate new ro
            if new_row > 0 and new_row < height and new_col > 0 and new_col < width:
                break                                                     # break from selection loop if in bounds

        # new_row, new_col now defined; attempt to acquire a mutex at
        # new position; if False is returned, mutex already locked by
        # another thread so loop back around to try a different move
        if mutex[new_row][new_col].acquire(blocking=False):               # Acquire a point on the grid before printing * on it
            screen.addstr(row, col, "*", curses.color_pair(my_id))        # write * to last position
            (row,col) = (new_row, new_col)                                # update to new position
            screen.addstr(row, col, str(my_id), curses.color_pair(my_id)) # write id at the "head"
            my_str = "row: %2d col: %2d" % (row,col)                      # write position to screen
            screen.addstr(height+my_id,0,my_str,curses.color_pair(my_id))
            sleep(delay)                                                  # sleep for a bit


# list of threads with different properties
worm_threads = [
  Thread(target=worm, kwargs={"my_id":1, "color":curses.COLOR_RED,    'delay':0.25, "row":1,        "col":1,      }),
  Thread(target=worm, kwargs={"my_id":2, "color":curses.COLOR_GREEN,  'delay':0.50, "row":1,        "col":width-1 }),
  Thread(target=worm, kwargs={"my_id":3, "color":curses.COLOR_BLUE,   'delay':0.75, "row":height-1, "col":width-1 }),
  Thread(target=worm, kwargs={"my_id":4, "color":curses.COLOR_YELLOW, 'delay':0.10, "row":height-1, "col":1       }),
]

try:
    for wt in worm_threads:
        wt.start()

    while (1):
        screen.clearok(1)
        screen.refresh()
        sleep(0.05)

except KeyboardInterrupt:
    screen.clear()
    curses.endwin()
    print("Interrupt received... Exiting!")
    os._exit(0)          # sys.exit() does not work for multiple threads
    
