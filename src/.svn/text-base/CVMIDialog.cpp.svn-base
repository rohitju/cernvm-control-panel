/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#include "CVMIDialog.h"
#include "CVMIConfigPage.h"
#include "CVMIAboutPage.h"

CVMIDialog::CVMIDialog(QWidget* parent) : QDialog(parent) // : QDialog (0, Qt::WindowMinMaxButtonsHint | Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowCloseButtonHint|Qt::Dialog) 
{

  // Select page
  selectPage = new CVMISelectPage;
  Q_ASSERT(connect(selectPage, SIGNAL(vmSelected(CVMIRelease)), this, SLOT(showVMCreate(CVMIRelease))));
  selectPage->setReady(false);

  // Create page
  createPage = new CVMICreatePage;
  Q_ASSERT(connect(createPage, SIGNAL(back()), this, SLOT(showVMSelect()))); 
  Q_ASSERT(connect(createPage, SIGNAL(createVM(CVMIInstanceConfig)), this, SLOT(showVMInstall(CVMIInstanceConfig))));

  // Install page
  installPage = new CVMIInstallPage;
  Q_ASSERT(connect(installPage, SIGNAL(done()), this, SLOT(showVMSelect())));

  // Config page
  CVMIConfigPage* configPage = new CVMIConfigPage;
  Q_ASSERT(connect(configPage, SIGNAL(displayOptionChanged()), selectPage, SLOT(displayOptionChanged())));
  Q_ASSERT(connect(configPage, SIGNAL(configChanged()), this, SLOT(verifyConfig())));  
   
  // About page
  CVMIAboutPage* aboutPage = new CVMIAboutPage;

  // Stacked Widget 
  contentWidget = new QStackedWidget;
  contentWidget->addWidget (selectPage);
  contentWidget->addWidget (createPage);  
  contentWidget->addWidget (installPage);

  // Tabs 
  QTabWidget *tabWidget = new QTabWidget;
  tabWidget->insertTab(0, contentWidget, tr("Installer"));
  tabWidget->insertTab(1, configPage, tr("Preferences"));
  tabWidget->insertTab(2, aboutPage, tr("About"));

   // Window decoration
  setWindowTitle(tr("CernVM installation tool"));  
  setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint | Qt::CustomizeWindowHint);

  // Layout
  QHBoxLayout *bodyLayout = new QHBoxLayout;
  bodyLayout->addWidget(tabWidget);
  setLayout(bodyLayout);

  // Application icon
  this->setWindowIcon(QIcon(":images/application_icon.png"));
  verifyConfig();
}

void CVMIDialog::verifyConfig()
{
  CVMIExitStatus status;
  status = installPage->checkConfig();  

  if (status.isOK == true)
    selectPage->setReady(true);
  else
  {
    selectPage->setReady(false);
    QMessageBox::critical(0, tr("Configuration error"), tr("Error: ") + status.msg);
  }
}

void CVMIDialog::showVMSelect()
  {  contentWidget->setCurrentIndex(0);  }

void CVMIDialog::showVMCreate(CVMIRelease vmRelease)//QString vmName, QString vmURL)
{
  selectedVMRelease = vmRelease;

  QString vmName = vmRelease.name;
  QString vmURL = vmRelease.url;
  QString vmArch = vmRelease.arch;

  vmName.replace(QRegExp ("\\s+")," ");

  createPage->setTitle (vmName);

  vmName.replace(QRegExp ("\\(.+\\)"),"");
  vmName.replace(QRegExp ("\\s+"), "-");
  vmName.append (vmArch);
  //name.append ("-" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
  createPage->setName(vmName);

  contentWidget->setCurrentIndex(1);
}

void CVMIDialog::showVMInstall(CVMIInstanceConfig vmConfig)
{
  installPage->start(selectedVMRelease, vmConfig);
  contentWidget->setCurrentIndex(2);
}

/*
void CVMIDialog::createIcons()
{

  QListWidgetItem* deployButton = new QListWidgetItem(buttonsWidget);
  deployButton->setIcon(QIcon(":images/download_icon.png"));
  deployButton->setText(tr("Deploy CernVM"));
  deployButton->setTextAlignment(Qt::AlignCenter);
  deployButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QListWidgetItem* configButton = new QListWidgetItem(buttonsWidget);
  configButton->setIcon(QIcon(":images/configure_icon.png"));
  configButton->setText(tr("Preferences"));
  configButton->setTextAlignment(Qt::AlignCenter);
  configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  Q_ASSERT( connect (buttonsWidget, SIGNAL(currentItemChanged (QListWidgetItem*, QListWidgetItem*)),
                     this, SLOT(changePage(QListWidgetItem*, QListWidgetItem*))));
             
  this->setWindowIcon(QIcon(":images/application_icon.png"));

}
*/

/*
void CVMIDialog::changePage(QListWidgetItem* current, QListWidgetItem* previous)
{
  if (!current)
    current = previous;

  contentWidget->setCurrentIndex(buttonsWidget->row(current));      
}
*/

