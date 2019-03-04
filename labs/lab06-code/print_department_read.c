/* print_department_read.c: uses read() and lseek().

   Read a binary file which has the below structure and print contacts
   (name/email) belonging to a single department.

   The structure of the binary file is as follows.

   1. file_header_t struct
      - inital identifying bytes which should be {0xDE,'D','I','R'}
      - size of following array of dept_offset_t array
   2. array of dept_offset_t structs
      - dept_code, offset, and num_contacts
      - length of this array is given in initial file_header_t
      - contains field offset which is bytes at which contacts for an
        associated department start
   3. array of contact_t structs
      - name and email fields
      - array is arranged by department
      - array of dept_offset_t structs indicate where each department starts
        and how many contacts are in it

   A verbose plain text example of this would be something along the
   following. Note that offsets are given by line while in the binary
   file they are given by byte offset.

   1. DEDIR 3 departments
   2. CS, starts at line 5, 8 contacts
   3. EE, starts at line 13, 6 contacts
   4. IT, starts at line 19, 5 contacts
   5. Arindam Banerjee, baner029@umn.edu
   6. Daniel Boley, boley@umn.edu
   7. Abhishek Chandra, chandra@umn.edu
   8. David Hung-Chang Du, du@umn.edu
   9. Maria Gini, gini@umn.edu
   10. Stephen Guy, sjguy@umn.edu
   11. Tian He, tianhe@umn.edu
   12. Mats Heimdahl, heimdahl@umn.edu
   13. Mehmet Akcakaya, akcakaya@umn.edu
   14. Massoud Amin, amin@umn.edu
   15. Raj Aravalli, aravalli@umn.edu
   16. Kia Bazargan, kia@umn.edu
   17. Itshak Bergel, ibergel@umn.edu
   18. Stephen Campbell, scampbell@umn.edu
   19. Joseph Axberg, jaxberg@umn.edu
   20. Carl Follstad, follstad@umn.edu
   21. Valarie Griep, griep003@umn.edu
   22. Richard Howey, howe0156@umn.edu
   23. Scott Kerlin, kerli012@umn.edu
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "department.h"

int main(int argc, char *argv[]){
  if(argc < 3){                              // check for 2 command line arguments
    printf("usage: %s <file.dat> <department>\n",argv[0]);
    printf("  department is one of\n");
    printf("  CS : computer science department\n");
    printf("  EE : electrical engineering department\n");
    printf("  IT : information technology department\n");
    return 1;
  }
  char *filename  = argv[1];
  char *dept_code = argv[2];

  int fd = open(filename, O_RDONLY);         // open file to get file descriptor

  ////////////////////////////////////////////////////////////////////////////////
  // CHECK the file_header_t struct for integrity, size of department array
  file_header_t header;
  read(fd, &header, sizeof(file_header_t *)); // binary header struct is first thing in the file
  int ident_matches =                         // check the first bytes to ensure correct file format
    header.ident[0] == 0xDE &&
    header.ident[1] == 'D'  &&
    header.ident[2] == 'I'  &&
    header.ident[3] == 'R';

  if(!ident_matches){
    printf("'%s' does not appear to be a binary department directory file\n",
           filename);
    exit(1);
  }

  int num_depts = header.num_depts; // header was ok, retrieve the number of departments

  ////////////////////////////////////////////////////////////////////////////////
  // SEARCH the array of department offsets for the department named
  // on the command line

  // fd is positioned after the file header structure, subsequent
  // reads will read in dept_offset_t structures.

  size_t offset = -1;
  int num_contacts = -1;
  for(int i=0; i<num_depts; i++){        // iterate over the array checking for desired department_code
    dept_offset_t dept;
    read(fd, &dept, sizeof(dept_offset_t));
    printf("Dept Name: %s Offset: %lu\n",
           dept.dept_code, dept.offset);

    if( strcmp(dept_code, dept.dept_code)==0 ){
      offset = dept.offset;       // found correct department
      num_contacts = dept.num_contacts;
    }
  }
  
  if(offset == -1){                      // check if department not found
    printf("Department code '%s' not found\n", dept_code);
    exit(1);
  }

  ////////////////////////////////////////////////////////////////////////////////
  // PRINT out all personnel in the specified department
  // - offset set to byte position from beginning of file for desired department
  // - num_contacts set to the number of contacts for that department

  off_t ret = lseek(fd, offset, SEEK_SET);  // seek to file offset for department found above
  if(ret == -1){
    perror("lseek() failed to seek to department position");
    exit(1);
  }

  printf("\n%d Contacts for %s department\n",num_contacts,dept_code);

  for(int i=0; i<num_contacts; i++){         // iterate over array and print contacts
    contact_t contact;
    read(fd, &contact, sizeof(contact_t));
    printf("%s <%s>\n", contact.name, contact.email);
  }

  close(fd);
  return 0;
}
