/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#ifndef CVMISELECTPAGE_H
#define CVMISELECTPAGE_H

#include <QtGui>
#include <QtNetwork>

#include "CVMIData.h"

class CVMISelectPage : public QWidget
{
  Q_OBJECT

  public:
    CVMISelectPage(QWidget* parent = 0);
    void populateList();
    void setReady(bool b);

  signals:
    void vmSelected (CVMIRelease vmRelease);
    
  private:
    // UI elements
    QCheckBox* desktopCheckBox;
    QCheckBox* basicCheckBox;
    QCheckBox* arch64CheckBox;
    QCheckBox* arch32CheckBox;

    QPushButton* deployButton;
    
    //QListWidget* availableReleases;
    QTableWidget* availableReleasesTbl;

    bool desktopChecked;
    bool basicChecked;
    bool arch64Checked;  
    bool arch32Checked;
    bool testChecked;

    // Class data
    QNetworkReply* httpReply;
    QNetworkAccessManager qnam;

    bool populateInProgress;
    QString downloadListURL; 
    QString availableImagesList;
    QList<CVMIRelease>    releaseList; 
    quint16 currentListElem;

  private slots: 
    void arch64Toggled (bool checked); 
    void arch32Toggled (bool checked); 
    void basicToggled (bool checked); 
    void desktopToggled (bool checked); 

    void displayOptionChanged();

    //void rowChanged(int);    
    void tblRowChanged(QTableWidgetItem* item);

    void populateFinished();
    void populateReadyRead();

    void deployClicked();
};
#endif // CVMISELECTPAGE_H
