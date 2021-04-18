#include "util.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_ISEEK 19
#define read 0
#define write 1
#define read_and_write 2
#define eof -1

extern int system_call();

int isCapital(char c){
  return 'A' <= c && c <= 'Z';
}

char toLower(char c){
  char x = c >= 'A' && c <= 'Z' ? c + 32 : c;
    return x;
}

int encoder(int inputFile ,int  outputFile, int dmode){
  char  c[1] = " "  ;
  char  debugBuffer[2] = "  ";
  int counter = 0;
  int status;
  char* countChar;
  while (1){
    status = system_call(SYS_READ , inputFile, c,1);
    if(status == 0 ){
      system_call(SYS_CLOSE, inputFile);
      system_call(SYS_CLOSE, outputFile);
      return 0;
    }
    if (c[0] != '\n'){
      if(dmode){
        counter += isCapital(c[0]);
        debugBuffer[0] = c[0];
        debugBuffer[1] = '\t'; 
        system_call(SYS_WRITE, STDERR , debugBuffer, 2);
    }
      c[0] = toLower(c[0]);
      if (dmode){
        debugBuffer[0] = c[0];
        debugBuffer[1] = '\n';
        system_call(SYS_WRITE, STDERR , debugBuffer, 2);
    }
    system_call(SYS_WRITE, outputFile , c, 1);
    }else{
      if (dmode){
      system_call(SYS_WRITE, STDERR , "the number of letters: ", 24);
      countChar = itoa(counter);
      system_call(SYS_WRITE, STDERR, countChar ,strlen(countChar));
      system_call(SYS_WRITE, STDERR , "\n", 1);
      counter = 0;
      }
      system_call(SYS_WRITE, outputFile , c, 1);
    }
  }
  return 0;
}

int main (int argc , char* argv[], char* envp[])
{
  int dmode = 0  ,i = 1 ;
  int  input = STDIN;
  int output = STDOUT;
  for (i=1 ; i< argc ; i++){
    if (!strcmp( argv[i] , "-D")){
      dmode = 1;
    }
    else if(!strncmp(argv[i],"-i" ,2)){
      input = system_call(SYS_OPEN,argv[i]+2,read);
      if (output == -1)
      {
        return 0x55;
      }
    }else if(!strncmp(argv[i],"-o" ,2)){
      output = system_call(SYS_OPEN,argv[i]+2,write);
      if (output == -1)
      {
        return 0x55;
      }
      
    }else {
      system_call(SYS_WRITE,STDOUT,"Hello, World!\n", 14);
      return 0x55;
    }
  }
  return encoder(input ,output , dmode);
}
