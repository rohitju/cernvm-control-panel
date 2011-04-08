/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#include "CVMIAboutPage.h"

CVMIAboutPage::CVMIAboutPage(QWidget* parent) : QWidget(parent) 
{
  QGroupBox* groupBox = new QGroupBox(tr("About CernVM Installer"));
  QGridLayout* groupBoxLayout = new QGridLayout;
  
  // Logo
  QLabel* logo = new QLabel;
  logo->setPixmap(QPixmap(":images/application_icon.png"));
  groupBoxLayout->addWidget(logo, 0, 0, 4, 1, Qt::AlignTop);

  // Label
  QLabel* artem   = new QLabel(tr("Version: 0.2.0.alpha\n\nAuthors:\n  Artem Harutyunyan\n  Predrag Buncic"));
  //\n\nContact: cernvm-talk@cern.ch\nhttp://cernvm.cern.ch"));
  artem->setOpenExternalLinks(true);

  QLabel* email = new QLabel(tr("Contact: <a href='mailto:cernvm-talk@cern.ch?subject=CernVM Installer'>cernvm-talk@cern.ch</a>"));
  Q_ASSERT(connect(email, SIGNAL(linkActivated(const QString&)), this, SLOT(emailClicked(const QString&))));

  QLabel* www = new QLabel(tr("Web page: <a href=http://cernvm.cern.ch>http://cernvm.cern.ch</a>")); 
  Q_ASSERT(connect(www, SIGNAL(linkActivated(const QString&)), this, SLOT(wwwClicked(const QString&))));

  QLabel* copyright = new QLabel(tr("<p>&copy; CERN 2010-") + QDateTime::currentDateTime().toString("yyyy") + " (PH/SFT/CernVM) </p>");

  groupBoxLayout->addWidget(artem, 0, 1, Qt::AlignTop|Qt::AlignLeft);
  groupBoxLayout->addWidget(email, 1, 1);  
  groupBoxLayout->addWidget(www, 2, 1);  
  groupBoxLayout->addWidget(copyright, 3, 1);  

  groupBox->setLayout(groupBoxLayout);  

  QGridLayout* mainLayout = new QGridLayout;
  mainLayout->addWidget(groupBox, 0, 0, Qt::AlignTop);
  setLayout(mainLayout);
}

void CVMIAboutPage::emailClicked(const QString& link)
{
  qDebug() << "_about_ emailClicked "<< link;
  QDesktopServices::openUrl(QUrl(link));
}

void CVMIAboutPage::wwwClicked(const QString& link)
{
  qDebug() << "_about_ wwwClicked " <<link;
  QDesktopServices::openUrl(QUrl(link));
}

