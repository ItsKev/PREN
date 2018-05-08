/*
 * Shell.h
 *
 *  Created on: Mar 6, 2018
 *      Author: burak
 */

#ifndef SHELL_H_
#define SHELL_H_

#define STRING_CONNECTOR()	(CLS1_SendStr((unsigned char*)";", CLS1_GetStdio()->stdOut))
#define STRING_S()			(CLS1_SendStr((unsigned char*)"s ", CLS1_GetStdio()->stdOut))
#define STRING_SUCCES()		(CLS1_SendStr((unsigned char*)"success\n", CLS1_GetStdio()->stdOut))
#define STRING_FAILURE()	(CLS1_SendStr((unsigned char*)"failure\n", CLS1_GetStdio()->stdOut))
#define STRING_PROMPT()		(CLS1_SendStr((unsigned char*)"CMD>> ", CLS1_GetStdio()->stdOut))

void Shell_Init(void);

uint8_t ShellParseCommand(uint8_t *cmdBuf, size_t cmdBufSize, CLS1_ConstStdIOType *io);

#endif /* SHELL_H_ */
