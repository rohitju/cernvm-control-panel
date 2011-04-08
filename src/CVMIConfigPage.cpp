/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#include "CVMIConfigPage.h"

CVMIConfigPage::CVMIConfigPage(QWidget* parent)
  :QWidget(parent)
{
  QGroupBox* configGroupBox = new QGroupBox(tr("CernVM installer preferences"));
   
  QLabel*      imageDirLabel = new QLabel(tr("Image download directory:"));
  QPushButton* imageDirButton = new QPushButton(tr("Browse"));     
  imageDirEdit = new QLineEdit();
  imageDirEdit->setText(conf.getImageDir());
  imageDirEdit->setReadOnly(true);
  Q_ASSERT(connect (imageDirButton, SIGNAL(clicked()), this, SLOT(changeImageDir())));
  
  QLabel* vmDirLabel = new QLabel(tr("VM deployment directory:"));
  QPushButton* vmDirButton = new QPushButton(tr("Browse"));     
  vmDirEdit = new QLineEdit();
  vmDirEdit->setText(conf.getVMDir());
  vmDirEdit->setReadOnly(true);
  Q_ASSERT(connect (vmDirButton, SIGNAL(clicked()), this, SLOT(changeVMDir())));
 
  QLabel* hypervisorDirLabel = new QLabel(tr("Hypervisor installation directory:"));
  QPushButton* hypervisorDirButton = new QPushButton(tr("Browse"));     
  hypervisorDirEdit = new QLineEdit();
  hypervisorDirEdit->setText(conf.getHypervisorDir());
  hypervisorDirEdit->setReadOnly(true);
  Q_ASSERT(connect (hypervisorDirButton, SIGNAL(clicked()), this, SLOT(changeHypervisorDir())));

  showDevCheckBox = new QCheckBox(tr("Show development releases"));
  showDevCheckBox->setChecked (conf.getShowDevReleases().compare("true") == 0);
  Q_ASSERT(connect (showDevCheckBox, SIGNAL(toggled(bool)), this, SLOT(changeShowDevReleases(bool))));
  
  QPushButton* resetButton = new QPushButton(tr("Reset to defaults"));
  Q_ASSERT(connect(resetButton, SIGNAL(clicked()), this, SLOT(resetConfig())));
 
  QVBoxLayout* mainLayout = new QVBoxLayout;
  QGridLayout* configLayout = new QGridLayout;

  configLayout->addWidget (imageDirLabel, 0, 0, Qt::AlignTop);  
  configLayout->addWidget (imageDirEdit, 1, 0, Qt::AlignTop);
  configLayout->addWidget (imageDirButton, 1, 1, Qt::AlignTop);

  configLayout->addWidget (vmDirLabel, 2, 0);//, Qt::AlignTop);  
  configLayout->addWidget (vmDirEdit, 3, 0);
  configLayout->addWidget (vmDirButton, 3, 1);

  configLayout->addWidget (hypervisorDirLabel, 4, 0);//, Qt::AlignTop);  
  configLayout->addWidget (hypervisorDirEdit, 5, 0);
  configLayout->addWidget (hypervisorDirButton, 5, 1);

  configLayout->addWidget (showDevCheckBox, 6, 0);


  configLayout->addWidget (resetButton, 7, 1);

  configGroupBox->setLayout(configLayout);
  
  //deployButton = new QPushButton(tr("Deploy"));
  mainLayout->addWidget(configGroupBox, 0, Qt::AlignTop);
  //mainLayout->addWidget(resetButton, 0, Qt::AlignRight | Qt::AlignTop); 
  //mainLayout->addSpacing(12);

  setLayout(mainLayout); 
  //setLayout(configLayout); 
}

void CVMIConfigPage::changeShowDevReleases(bool f)
{
  if (f == true)
    conf.setShowDevReleases("true");
  else
    conf.setShowDevReleases("false");
  
    emit displayOptionChanged();
}

void CVMIConfigPage::resetConfig()
{
  conf.reset();

  imageDirEdit->setText(conf.getImageDir());
  imageDirEdit->setCursorPosition(0);

  vmDirEdit->setText(conf.getVMDir());
  vmDirEdit->setCursorPosition(0);
  
  hypervisorDirEdit->setText(conf.getHypervisorDir());
  vmDirEdit->setCursorPosition(0); 

  showDevCheckBox->setChecked (conf.getShowDevReleases().compare("true") == 0);
  emit displayOptionChanged();
  emit configChanged();
}

void CVMIConfigPage::changeImageDir()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Choose VM image download directory"),
                                                  QDir::homePath(),
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); 
  if (dir.compare("") == 0)
    return;  

  imageDirEdit->setText (dir);
  imageDirEdit->setCursorPosition(0);
  conf.setImageDir(dir);
  emit configChanged();
}

void CVMIConfigPage::changeVMDir()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Choose VM deployment directory"),
                                                  QDir::homePath(),
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); 
  if (dir.compare("") == 0)
    return;  

  vmDirEdit->setText (dir);
  vmDirEdit->setCursorPosition(0);
  conf.setVMDir(dir);
  emit configChanged();
}

void CVMIConfigPage::changeHypervisorDir()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Choose hypervisor installation directory"),
                                                  QDir::homePath(),
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); 
  if (dir.compare("") == 0)
    return;  

  hypervisorDirEdit->setText (dir);
  hypervisorDirEdit->setCursorPosition(0);
  conf.setHypervisorDir(dir);
  emit configChanged();
}

