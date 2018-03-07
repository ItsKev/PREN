/*
 * Shell.h
 *
 *  Created on: Mar 6, 2018
 *      Author: burak
 */

#ifndef SHELL_H_
#define SHELL_H_

void Shell_Init(void);

uint8_t ShellParseCommand(uint8_t *cmdBuf, size_t cmdBufSize, CLS1_ConstStdIOType *io);

#endif /* SHELL_H_ */
