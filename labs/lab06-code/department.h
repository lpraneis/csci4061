// department.h: header file which defines the structures required for department listings.  

#ifndef DEPARTMENT_H
#define DEPARTMENT_H 1

#include <unistd.h>

// struct at the beginning of the department "directory" file which
// has contacts for all members of the department.
typedef struct {
  unsigned char ident[4]; // identifying bytes should be {0xDE,'D','I','R'}
  int num_depts;          // number of departments in dept_offset_t array
} file_header_t;

// struct describing a single department and where its contacts are
// located in the file
typedef struct {
  char dept_code[16];  // short identifier code for department
  size_t offset;       // byte offset in file where contact_t array starts
  int num_contacts;    // number of contact_t's for this department
} dept_offset_t;

// struct describing a single department contact
typedef struct {
  char name[128];      // name of contact person
  char email[128];     // email address 
} contact_t;

#endif
