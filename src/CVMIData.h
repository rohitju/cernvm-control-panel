/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#ifndef CVMIDATA_H
#define CVMIDATA_H

#include <QString>

struct CVMIRelease
{
  quint16 id;
  QString url;
  QString name;
  QString arch;
  QString edition; 
};

struct CVMIInstanceConfig
{
  QString name;
  QString ramSize;
  QString cpuCount;
};

struct CVMIExitStatus
{
  bool    isOK;
  QString msg;
  qint16  code;
};


#endif // CVMIDATA_H

