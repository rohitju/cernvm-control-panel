/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#ifndef CVMICONTROLVBOX_H
#define CVMICONTROLVBOX_H

#include "CVMIControlVM.h"

#include <QObject>
#include <QString>
#include <QProcess>

class CVMIControlVBox : public CVMIControlVM
{
  Q_OBJECT

  public:
    CVMIControlVBox (QString* path, CVMIInstanceConfig vmConfig);
    CVMIControlVBox();

    ~CVMIControlVBox() {}

    bool checkHypervisor();
    void createVM();
    void configureVM();
  
  signals:
    void vmReady();
    void error(QString msg);
    void warning(QString msg);

  private slots:
    void processFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStdout();

  private:
    void startProcess();    
    void initPaths();

    QString            executable;
    QStringList        args;
    QProcess           process;
    unsigned char      step;
    QString            vboxManageExecutable;
    QString            vboxGuestAdditionsIso;
    QString            vmImageFullPath;
    //QString       vmName;
    bool               canFail;
    CVMIInstanceConfig vmConfigData;
};
 

#endif// CVMICONTROLVBOX_H

