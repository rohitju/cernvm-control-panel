/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#ifndef CVMICREATEPAGE_H
#define CVMICREATEPAGE_H

#include <QtGui>
//#include <QDialog>
//#include <QSlider>

#include "CVMIData.h" 

class CVMICreatePage : public QWidget
{
  Q_OBJECT

  public:
    CVMICreatePage(QWidget* parent = 0);

    QString             getName();
    QString             getRamSize();
    QString             getCPUCount();
    CVMIInstanceConfig  getVMConfig();

    void setName(QString name);
    void setTitle(QString name);

  signals:
    void createVM (CVMIInstanceConfig c);
    void back();

  private slots:
    void ramSliderChanged(int value);  
    void cpuSliderChanged(int value);
    void proceedPressed(); 
    void backPressed();

  private:
    QGroupBox* instanceGroupBox;
    QSlider*   ramSlider;
    QSlider*   cpuSlider;
    QLabel*    ramValue;
    QLabel*    cpuValue;
    QLineEdit* vmName;
};

#endif// CVMICREATEPAGE_H

