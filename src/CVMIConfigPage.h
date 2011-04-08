/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#ifndef CVMICONFIGPAGE_H
#define CVMICONFIGPAGE_H

#include <QWidget>
#include <QtGui>
#include "CVMIConfig.h"

class CVMIConfigPage : public QWidget
{
  Q_OBJECT

signals:
  void displayOptionChanged();
  void configChanged();  

public:
  CVMIConfigPage(QWidget* parent = 0);

private slots:
  void changeImageDir();
  void changeVMDir();
  void changeHypervisorDir();
  void changeShowDevReleases(bool);
  void resetConfig();
 
private:
  QLineEdit* imageDirEdit;
  QLineEdit* vmDirEdit;   
  QLineEdit* hypervisorDirEdit;
  QCheckBox* showDevCheckBox;   
  CVMIConfig conf;
};

#endif // CVMICONFIGPAGE_H

