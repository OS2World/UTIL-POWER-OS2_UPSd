#ifndef UPSD_H
#define UPSD_H

#define INCL_PM
#define INCL_DOSDEVICES
#define INCL_DOSPROCESS
#define INCL_WINMESSAGEMGR
#define INCL_WINWINDOWMGR
#define INLC_WINWORKPLACE

#include <os2.h>
#include <pmwp.h>
#include <stdio.h>
#include <stdlib.h>
#include <bsedev.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>

#define WORD unsigned short int

#define ST_OK		(BYTE) 0;
#define ST_POWERFAIL 	(BYTE) 1;
#define ST_POWERLOW	(BYTE) 2;

static int fd = 0;
static int state = ST_OK;

const BYTE PRT_CTS = 	0x10;
const BYTE PRT_DSR = 	0x20;
const BYTE PRT_RI  = 	0x40;
const BYTE PRT_DCD = 	0x80;

#endif