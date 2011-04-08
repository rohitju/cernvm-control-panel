/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#ifndef CVMICONTROLVM_H
#define CVMICONTROLVM_H

#include <QString>
#include <QObject>

#include "CVMIData.h"

class CVMIControlVM : public QObject
{
  Q_OBJECT 
  public:
    CVMIControlVM (QString* path, CVMIInstanceConfig vmConfig) : 
      uncompressedImagePath (path),
      vmConfigData(vmConfig)
      {}
    CVMIControlVM(){}
   
    virtual ~CVMIControlVM() {}
    virtual bool checkHypervisor() = 0;
    virtual void createVM() = 0;
    virtual void configureVM() = 0;   

    QString* getImagePath() 
      { return uncompressedImagePath; }    
  
  signals:
    void vmReady();
    void error(QString msg);
    void warning(QString msg);

  private:
    QString* uncompressedImagePath;
    CVMIInstanceConfig vmConfigData;
};
 
#endif // CVMICONTROLVM_H



