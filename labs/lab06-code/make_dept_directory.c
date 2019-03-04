/* Create a binary file which has the following structure.

   file_header_t struct
   - identifying bytes
   - size of following array of dept_offset_t array
   array of dept_offset_t structs
   - dept_code, offset, and num_contacts
   - length of this array is given in initial file_header_t
   - contains field offset which is bytes at which contacts for an
     associated department start
   array of contact_t structs
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
#include <unistd.h>
#include <fcntl.h>
#include "department.h"

contact_t cs_dept[] = {
  {"Arindam Banerjee", "baner029@umn.edu"},
  {"Daniel Boley","boley@umn.edu"},
  {"Abhishek Chandra","chandra@umn.edu"},
  {"David Hung-Chang Du","du@umn.edu"},
  {"Maria Gini","gini@umn.edu"},
  {"Stephen Guy","sjguy@umn.edu"},
  {"Tian He","tianhe@umn.edu"},
  {"Mats Heimdahl","heimdahl@umn.edu"},
};
  
contact_t ee_dept[] = {
  {"Mehmet Akcakaya","akcakaya@umn.edu"},
  {"Massoud Amin","amin@umn.edu"},
  {"Raj Aravalli","aravalli@umn.edu"},
  {"Kia Bazargan","kia@umn.edu"},
  {"Itshak Bergel","ibergel@umn.edu"},
  {"Stephen Campbell","scampbell@umn.edu"},
};
  
contact_t it_dept[] = {
  {"Joseph Axberg","jaxberg@umn.edu"},
  {"Carl Follstad","follstad@umn.edu"},
  {"Valarie Griep", "griep003@umn.edu"},
  {"Richard Howey", "howe0156@umn.edu"},
  {"Scott Kerlin", "kerli012@umn.edu"},
};


// calculate fill in offsets later, use sizeof() to get total size in
// bytes of arrays that are compile-time constants, divide to get
// number of elements.
dept_offset_t offsets[] = {
  {.dept_code="CS", .offset=-1, .num_contacts=sizeof(cs_dept)/sizeof(contact_t)}, 
  {.dept_code="EE", .offset=-1, .num_contacts=sizeof(ee_dept)/sizeof(contact_t)},
  {.dept_code="IT", .offset=-1, .num_contacts=sizeof(it_dept)/sizeof(contact_t)},
};

file_header_t file_header = {
  .ident={0xDE,'D','I','R'},
  .num_depts=3,
};

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s <file.dat>\n",argv[0]);
    return 1;
  }

  // Compute offsets into the file through use sizeof() to get size in
  // bytes of compile time constant arrays/structs.
  offsets[0].offset = sizeof(file_header)+sizeof(offsets);
  offsets[1].offset = offsets[0].offset + sizeof(cs_dept);
  offsets[2].offset = offsets[1].offset + sizeof(ee_dept);

  int fd = open(argv[1],O_WRONLY, S_IRUSR | S_IWUSR);
  
  write(fd, &file_header, sizeof(file_header)); 
  write(fd, &offsets,     sizeof(offsets)); // sizeof() returns size in bytes of entire fixed array
  write(fd, &cs_dept,     sizeof(cs_dept));
  write(fd, &ee_dept,     sizeof(ee_dept));
  write(fd, &it_dept,     sizeof(it_dept));
  
  close(fd);
  return 0;
}
