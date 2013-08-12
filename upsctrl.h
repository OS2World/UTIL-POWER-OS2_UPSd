#ifndef UPSCTRL_H
#define UPSCTRL_H

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
#include "upsd.h"

void UPSPowerOff(HFILE com);
void SetBitRate(HFILE com, WORD rate);
void RaiseDTR(HFILE com);
void LowerDTR(HFILE com);
int CheckDSR(HFILE com);
int CheckState(HFILE com);

#endif
