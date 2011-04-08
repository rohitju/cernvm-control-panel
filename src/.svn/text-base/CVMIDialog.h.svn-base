/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#ifndef CVMIDIALOG_H
#define CVMIDIALOG_H

#include <QtGui>
#include <QDialog>

#include "CVMIInstallPage.h"
#include "CVMICreatePage.h"
#include "CVMISelectPage.h"
#include "CVMIData.h"

class CVMIDialog : public QDialog
{
  Q_OBJECT

  public:
    CVMIDialog(QWidget* parent = 0);
    ~CVMIDialog(){}

  public slots:
//  void changePage (QListWidgetItem *current, QListWidgetItem *previous);
    void showVMCreate(CVMIRelease vmRelease);//QString vmName, QString vmURL);
    void showVMSelect();
    void showVMInstall(CVMIInstanceConfig vmConfig);    
    void verifyConfig();    
 
private: 
//  QListWidget*    buttonsWidget;
    QStackedWidget*  contentWidget;
    CVMICreatePage*  createPage;
    CVMIInstallPage* installPage;
    CVMISelectPage*  selectPage;
    CVMIRelease      selectedVMRelease;
//  QTextEdit*      messageWindow; 
};

#endif// CVMIDIALOG_H

