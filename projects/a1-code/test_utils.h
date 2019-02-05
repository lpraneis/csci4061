void fail(char *msg, int errstr);
// Fail with given message; if errstr is nonzero, print a message
// associated with the current errno

int assert_int_equals(char *msg, int expect, int actual);
// Utility to check if two ints are equal; if unequal print errors and
// return 1

int assert_int_positive(char *msg, int actual);
// Utility to check if two ints are equal; if unequal print errors and
// return 1

int assert_str_equals(char *msg, char *expect, char *actual);
// Utility to check if two strings are equal; if unequal print errors
// and return 1

int assert_strn_equals(char *msg, char *expect, char *actual);
// Utility to check if two strings are equal; if unequal print errors
// and return 1; prints expect and actual on their own lines for
// multiline strings

int assert_null(char *msg, void *actual);
// Utility to check if pointer is null; if not print errors
// and return 1

int assert_unique_pointers(char *msg, void *x, void *y);
// Utility to check if pointers are unique or point at the same
// location; if not print errors and return 1

// Temporarily direct stdout to a pipe which is captured for later
// use.
void catch_stdout();

// Restore stdout redirected via catch_stdout(); fill internal buffer
// to allow captured output to be retrieved
void restore_stdout();

// Get a pointer to the output captured from stdout. Uses an internal
// buffer so overwritten each call to catch/restore, do not free().
char *captured_stdout();
