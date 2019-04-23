#include "blather.h"
#include <termios.h>

static struct termios oldt, newt; // structs to change the terminal input mode

// Set non-canonical mode so that each character of input is available
// immediately
void simpio_noncanonical_terminal_mode(){
  // Turn off output buffering
  setvbuf(stdout, NULL, _IONBF, 0); 

  // tcgetattr gets the parameters of the current terminal
  // STDIN_FILENO will tell tcgetattr that it should write the
  // settings of stdin to oldt
  tcgetattr( STDIN_FILENO, &oldt);

  // now the settings will be copied
  newt = oldt;

  // ICANON normally takes care that one line at a time will be
  // processed that means it will return if it sees a "\n" or an EOF
  // or an EOL
  newt.c_lflag &= ~(ICANON | ECHO); 
  //  newt.c_lflag &= ~(ICANON);          

  // Those new settings will be set to STDIN TCSANOW tells tcsetattr
  // to change attributes immediately.
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);

}

// Reset terminal mode to what is was prior to
// simpio_set_noncanonical_mode()
void simpio_reset_terminal_mode(){
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

// Reset the simpio_t object to have 0 for position and flags. Usually
// done after completing input and processing it.
void simpio_reset(simpio_t *simpio){
  simpio->pos = 0;
  simpio->buf[0] = '\0';
  simpio->line_ready = 0;
  simpio->end_of_input = 0;
  simpio->infile  = stdin;
  simpio->outfile = stdout;
}

// Set the prompt for the simpio handle. Maximum length for the prompt
// is MAXNAME.
void simpio_set_prompt(simpio_t *simpio, char *prompt){
  strncpy(simpio->prompt, prompt, MAXNAME);
}

// Assumes things are in non-canonical terminal mode otherwise results
// may vary.  Read a character from the input associated with
// simpio. Fields are adjusted to reflect the state of input after
// reading the character. Typically:
// 
// - simpio->pos will increase
// - simpio->buf will get one more character
// - simpio->line_ready may be set to 1 if a line is completed 
// - for backspaces, buf and pos decrease
// 
// This funciton is used in a loop to read input until line_ready is 1. 
void simpio_get_char(simpio_t *simpio){
  int c = fgetc(simpio->infile);
  if(0){}
  else if(c == '\n' && simpio->pos > 0){
    simpio->buf[simpio->pos] = '\0';
    simpio->line_ready = 1;
  }    
  else if((c == '\b' || c == DEL || c == '\n') && simpio->pos == 0){
    // ignore enter, backspace without input
  }    
  else if(c == EOT && simpio->pos > 0){
    simpio->buf[simpio->pos] = '\0';
    simpio->line_ready = 1;
  }    
  else if((c == '\b' || c == DEL) && simpio->pos > 0){ // backspace or delete
    simpio->pos = simpio->pos-1;
    simpio->buf[simpio->pos] = '\0';
    int fd = fileno(simpio->outfile);
    write(fd, "\b \b", 3);                              // erase last character
  }
  else if(c != EOF && c != EOT && simpio->pos < MAXLINE-1){ // normal chars get added
    simpio->buf[simpio->pos] = c;
    simpio->pos++;
    simpio->buf[simpio->pos] = '\0';
    fputc(c,simpio->outfile);
  }
  if(c == EOF || c == EOT){     // check for end of input
    simpio->end_of_input = 1;
  }
}

// Print like printf but move the input prompt ahead and preserve the
// input that has been typed so far along with the prompt.
void iprintf(simpio_t *simpio, char *fmt, ...){
  char output[MAXLINE*2];       // buffer for message
  int max = MAXLINE*2;
  int off = 0;
  off += snprintf(output+off, max-off, "\33[2K\r");           // erase line
  va_list myargs;
  va_start(myargs, fmt);
  off += vsnprintf(output+off,MAXLINE,fmt,myargs);            // add on the new message
  va_end(myargs);
  off += snprintf(output+off, max-off, "%s", simpio->prompt); // add prompt back
  off += snprintf(output+off, max-off, "%s", simpio->buf);    // current typed input
  
  int fd = fileno(simpio->outfile);
  write(fd, output, off);

  // fprintf(input->outfile, "%s", input->prompt);
  // simpio_print(input);
}
