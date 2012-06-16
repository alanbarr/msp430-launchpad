/* Copyright (C) Alan Barr 2011 */
/* Builds with mspgcc */

#ifndef COMMAND_FUNCTIONS_H_
#define COMMAND_FUNCTIONS_H_

#define COMMANDLISTSIZE 8

typedef struct sCommandStruct {char * commandString;
							   void (*function)(void);
							  } tCommandStruct;

extern const tCommandStruct commandList[COMMANDLISTSIZE];

void led1On(void);
void led2On(void);
void led1Off(void);
void led2Off(void);
void led1Toggle(void);
void led2Toggle(void);
void cliHelp(void);
void reset(void);
#endif /*COMMAND_FUNCTIONS_H_*/
