#!/usr/bin/awk -f

/: sem_open /{
  sem_err=1
}

/ERROR SUMMARY/{
  if(sem_err==1){
    nerrors=$4
    ncontexts=$7
    $4 = nerrors-1
    $7 = ncontexts-1
    print "NOTE: FILTERED sem_open BUG"
  }
}

{print}
