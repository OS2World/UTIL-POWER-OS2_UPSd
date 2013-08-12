/* OS/2 UPS Daemon
 * For Trust 400/650 Energy Protector
 *
 * Copyright (C) 1997 by M.W. Pieters Software en Advies
 *                    Zoetermeer, the Netherlands
 * Version 0.1
 * Copying policy: GNU General Public License version 2.0 or higher
 */

#include <string.h>
#include <process.h>
#include <setjmp.h>
#include <signal.h>
#include "upsd.h"
#include "upsctrl.h"

#define COUNT_INIT 6000		// about 10 minutes

#define SIG_COUNT_STOP SIGUSR1
#define SIG_SHUTD_STOP SIGUSR2

static int countdownRunning = 0;
static int shutdownRunning = 0;
static int counter = COUNT_INIT;
static int doShutdown = 0;

/* Temporary until commandline parameters are handled
 * 1: COM1, 2: COM2, etc.
 */
#define UPS_DEVICE 2

// -----------------
// Shutdown routines
// -----------------

void shutdown(HFILE com)
{
  HAB		hAnchorBlock;
  ULONG		ulRetCode;
  ULONG		ulSendResult;
  HMQ		hMsgQWindow;
  QMSG		rMsgQWin;
  QMSG		rMsgDESK;
  int 		x;

  if ((hAnchorBlock = WinInitialize(0)) == NULLHANDLE)
    return;
  if ((hMsgQWindow = WinCreateMsgQueue(hAnchorBlock, 0)) == NULLHANDLE)
    return;
  rMsgDESK.hwnd = HWND_DESKTOP;
  rMsgDESK.msg = 0;
  rMsgDESK.mp1 = MPVOID;
  rMsgDESK.mp2 = MPVOID;
  rMsgDESK.time = 0;
  rMsgDESK.reserved = 0;
  ulSendResult = WinBroadcastMsg(HWND_DESKTOP,
				 WM_SYSCOMMAND,
				 MPFROMSHORT(SC_CLOSE),
				 MPFROM2SHORT(CMDSRC_MENU,FALSE),
				 (ULONG) BMSG_FRAMEONLY);

  for (int i=0; i < 60; i++)	// One minute for a clean stop of all programs
  {
    x = CheckState(com);
    if (x != state) state = x;
    if (state == 0) { counter = COUNT_INIT; return; };
    if (state == 2) break;
    DosSleep(1000);
  }

  DosBeep(4000, 100);
  DosBeep(3000, 100);
  DosBeep(2000, 100);
  DosBeep(1000, 100);
  DosShutdown(0);
  UPSPowerOff(com);
  WinDestroyMsgQueue(hMsgQWindow);
  WinTerminate(hAnchorBlock);
  return;
}

// -----------------
// Threaded routines
// -----------------

void countdownThread(void * pcom)
{
  HFILE * xcom = (HFILE *) pcom;
  countdownRunning = 1;

  while(state != 0)
  {
    DosSleep(10);
    if (--counter <= 0) break;
  }
  if (state != 0)
  // do a shutdown...
    doShutdown = 1;
  while (state != 0) DosSleep(100);
  countdownRunning = 0;

  _endthread();
}

// --->PowerFail<---

void Powerfail(int event, HFILE com)
{
  ULONG reserved = 0;
  switch(event)
  {
    case 0: 	/* Power is back */
      state = ST_OK;
      doShutdown = 0;
      counter = COUNT_INIT;
      break;
    case 1:	/* Power is failing, UPS active */
      {
	state = ST_POWERFAIL;
	if (countdownRunning == 0)
	  _beginthread(countdownThread, 4096, (void*)&com);
      }
      break;
    case 2:	/* Power is failing, battery low */
    default:
      {
	state = ST_POWERLOW;
	doShutdown = 2;
      }
      break;
  }
}

// --->HELP<---

void help()
{
  printf("Usage:\n\n");
  printf("UPSD.EXE <port>\n");
  printf("    <port> can be one of COM1..COM4\n\n");
}

typedef struct
{
  WORD writeTimeout;
  WORD readTimeout;
  BYTE flags1;
  BYTE flags2;
  BYTE flags3;
  BYTE errReplChr;
  BYTE brkReplChr;
  BYTE xon;
  BYTE xoff;
} DCBparam;

/* Main procedure */
void main(int argc, char * argv[])
{
  int newState = 0;
  HFILE com;
  ULONG action;
  DCBparam DCB;

  if (argc != 2)
  {
    fprintf(stderr, "Incorrect number of parameters!\n");
    help();
    exit(1);
  }

  if ((strcmpi(argv[1], "COM1")<0) ||
      (strcmpi(argv[1], "COM4")>0))
  {
    fprintf(stderr, "Serial port name required!\n");
    help();
    exit(2);
  }

  unsigned long datasize = sizeof(DCB);
  if (DosOpen(argv[1], &com, &action,
	       0L,	// logical size
	       0L,	// file attributes
	       0x01,	// action on existence
	       0x0012,	// openbits
	       0L)) exit(1);

  RaiseDTR(com);
  if (!CheckDSR(com))
    DosBeep(1000, 1000L);

  DosDevIOCtl(com, 0x01, 0x73, NULL, 0, NULL, &DCB, datasize, &datasize);
  DCB.writeTimeout = 1000;
  DCB.readTimeout = 100;
  DCB.flags1 = 0x00;
  DCB.flags3 = (DCB.flags3 & 0xF9) + 0x04;
  SetBitRate(com, 110);

  while (CheckDSR(com))
  {
    newState = CheckState(com);
    if (newState != state)
      Powerfail(newState, com);
    switch(state)
    {
      case 1:
	DosBeep(500, 10L);
	break;
      case 2:
	DosBeep(4000, 10L);
	break;
      default:
	break;
    }
    switch (doShutdown)
    {
      case 1: shutdown(com); break;
      case 2:
	{
	  DosShutdown(0);
	  UPSPowerOff(com);
	};
	break;
      default: break;
    }
    DosSleep(1000);
  }
  DosClose(com);
  exit(-1);
}
