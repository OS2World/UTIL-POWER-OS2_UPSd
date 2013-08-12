#include "upsctrl.h"

void UPSPowerOff(HFILE com)
{
  BYTE ch = 0xFF;
  unsigned long datasize = sizeof(ch);

  while (1)
  {
    DosDevIOCtl(com, 0x01, 0x44, &ch, datasize, &datasize, NULL, 0, NULL);
    DosSleep(10);
  }
}

void SetBitRate(HFILE com, WORD rate)
{
  WORD param = rate;
  unsigned long paramsize = sizeof(param);

  DosDevIOCtl(com, 0x01, 0x42, &param, paramsize, &paramsize, NULL, 0, NULL);
}

void RaiseDTR(HFILE com)
{
  struct {
  BYTE setON;
  BYTE setOFF;
  } param;
  WORD data;
  unsigned long paramsize = sizeof(param);
  unsigned long datasize = sizeof(data);

  param.setON = 0x01;
  param.setOFF = 0xFF;
  DosDevIOCtl(com, 0x01, 0x46, &param, paramsize, &paramsize, &data, datasize, &datasize);
}

void LowerDTR(HFILE com)
{
  struct {
  BYTE setON;
  BYTE setOFF;
  } param;
  WORD data;
  unsigned long paramsize = sizeof(param);
  unsigned long datasize = sizeof(data);

  param.setON = 0x00;
  param.setOFF = 0xFE;
  DosDevIOCtl(com, 0x01, 0x46, &param, paramsize, &paramsize, &data, datasize, &datasize);
}

int CheckDSR(HFILE com)
{
  BYTE data;
  unsigned long datasize = sizeof(data);

  data = 0;
  DosDevIOCtl(com, 0x01, 0x67, NULL, 0, NULL, &data, datasize, &datasize);
  return ((data & 32) != 0);
}

int CheckState(HFILE com)
{
  BYTE data;
  unsigned long datasize = sizeof(data);
  int ret = ST_OK;

  data = 0;
  DosDevIOCtl(com, 0x01, 0x67, NULL, 0, NULL, &data, datasize, &datasize);
  if (data & PRT_DSR)
  {
    if (!(data & PRT_CTS)) return ST_POWERLOW;
    if (!(data & PRT_DCD)) return ST_POWERFAIL;
  }
  return ret;
}
