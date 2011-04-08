/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/


#include "CVMISelectPage.h"
#include "CVMIConfig.h"

CVMISelectPage::CVMISelectPage(QWidget* parent) : QWidget(parent),
populateInProgress(false),
downloadListURL("http://rbuilder.cern.ch/downloads/buildlist.py?plain=y&hypervisor=virtualbox"),
availableImagesList("")
{
  // Filter group box 
  QGroupBox* filterGroupBox  = new QGroupBox(tr("Releases to display")); 

  QLabel* archLabel = new QLabel(tr("Architecture:"));
  arch64CheckBox = new QCheckBox(tr("64 bit (x86_64)"));
  arch64CheckBox->setChecked(true);
  arch64Checked = true;  
  Q_ASSERT(connect (arch64CheckBox, SIGNAL(toggled(bool)), this, SLOT(arch64Toggled(bool))));

  arch32CheckBox = new QCheckBox(tr("32 bit (x86)"));
  arch32CheckBox->setChecked(false);
  arch32Checked = false;
  Q_ASSERT(connect (arch32CheckBox, SIGNAL(toggled(bool)), this, SLOT(arch32Toggled(bool))));

  QLabel* editionLabel = new QLabel(tr("Edition:"));
  basicCheckBox = new QCheckBox(tr("Basic"));
  basicCheckBox->setChecked(true);
  basicChecked = true;
  Q_ASSERT(connect (basicCheckBox, SIGNAL(toggled(bool)), this, SLOT(basicToggled(bool))));

  desktopCheckBox = new QCheckBox(tr("Desktop"));
  desktopCheckBox->setChecked(true);
  desktopChecked = true;   
  Q_ASSERT(connect (desktopCheckBox, SIGNAL(toggled(bool)), this, SLOT(desktopToggled(bool))));
  
  QGridLayout* filterLayout = new QGridLayout;

  filterLayout->addWidget(editionLabel, 0, 0);
  filterLayout->addWidget(basicCheckBox, 1, 0); 
  filterLayout->addWidget(desktopCheckBox, 2, 0); 

  filterLayout->addWidget(archLabel, 0 ,1);
  filterLayout->addWidget(arch64CheckBox, 1, 1); 
  filterLayout->addWidget(arch32CheckBox, 2, 1); 

  filterGroupBox->setLayout(filterLayout);
  
  // Release group box 
  QGroupBox* releaseGroupBox = new QGroupBox(tr("Available releases")); 
  //availableReleases = new QListWidget;

  availableReleasesTbl = new QTableWidget;
  availableReleasesTbl->setColumnCount(3);
  availableReleasesTbl->setHorizontalHeaderLabels(QStringList() << "Edition" << "Version" << "Architecture"); 
  availableReleasesTbl->verticalHeader()->setVisible(false);//Labels(QStringList() << ""); 
  availableReleasesTbl->setShowGrid(false); 
  availableReleasesTbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
  availableReleasesTbl->setSelectionBehavior(QAbstractItemView::SelectRows);
  Q_ASSERT(connect(availableReleasesTbl,SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(tblRowChanged(QTableWidgetItem *))));
  
  QVBoxLayout* releaseListLayout = new QVBoxLayout;
  //releaseListLayout->addWidget(availableReleases);
  releaseListLayout->addWidget(availableReleasesTbl);

  releaseGroupBox->setLayout(releaseListLayout);
  //Q_ASSERT(connect (availableReleases, SIGNAL(currentRowChanged(int)), this, SLOT(rowChanged(int))));  

  // Deploy button
  deployButton = new QPushButton(tr("Deploy"));
  Q_ASSERT(connect (deployButton, SIGNAL(clicked()), this, SLOT(deployClicked())));

  // Page layout
  QGridLayout* pageLayout = new QGridLayout;
  pageLayout->addWidget(filterGroupBox, 0, 0, 1, 3, Qt::AlignTop);
  pageLayout->addWidget(releaseGroupBox, 1, 0, 1, 3);
  pageLayout->addWidget(deployButton, 2, 2);

  setLayout(pageLayout);

  populateList(); 
}

void CVMISelectPage::setReady(bool b)
{
  if (b == false)
    deployButton->setEnabled(false);
  else
    deployButton->setEnabled(true); 
}

void CVMISelectPage::deployClicked()
  {  emit vmSelected(releaseList[currentListElem]); }

//void CVMISelectPage::rowChanged(int currentRowId)
//  { currentListElem = currentRowId; }

void CVMISelectPage::tblRowChanged(QTableWidgetItem* item)
{ currentListElem = item->row(); }

void CVMISelectPage::populateList()
{
  if (populateInProgress == true)
  {
    qDebug() << "_populate_ () skipping";
    return;
  }  

  populateInProgress = true;

  qDebug() << "_populate_ ()";
  httpReply = qnam.get(QNetworkRequest(downloadListURL));

  Q_ASSERT(connect (httpReply, SIGNAL(finished()), this, SLOT(populateFinished())));
  Q_ASSERT(connect (httpReply, SIGNAL(readyRead()), this, SLOT(populateReadyRead())));

}

void CVMISelectPage::populateReadyRead()
{
  qDebug() << "_populate_ ReadyRead";
  availableImagesList.append(httpReply->readAll());
}

void CVMISelectPage::populateFinished()
{

  qDebug() << "_populate_ Finished";
  populateInProgress = false;
  CVMIRelease listEntry;
  unsigned int id = 0;

  if (httpReply->error() != QNetworkReply::NoError )
  {
    // Error 
    QMessageBox::critical(0, tr("HTTP"), tr("Failed to populate available images list: %1").arg(httpReply->errorString())); 
    return;
  }

  QStringList values;
  QString name;  
  QString edition;

  QTableWidgetItem* editionItem;
  QTableWidgetItem* versionItem;
  QTableWidgetItem* archItem;
  

  releaseList.clear();
  //availableReleases->clear();   
  availableReleasesTbl->clear();
  availableReleasesTbl->setHorizontalHeaderLabels(QStringList() << "Edition" << "Version" << "Architecture"); 
  availableReleasesTbl->setRowCount(0);

  CVMIConfig conf;
  bool testChecked = (conf.getShowDevReleases().compare("true") == 0);

  Q_FOREACH (QString elem, availableImagesList.split('\n'))
  {
    values = elem.split(';');
    if (values.size() < 10)
      continue;

    name = values[6].replace(QRegExp ("\\(.+\\)"),"");
    if (! name.contains("Desktop"))
      name += "Basic";

    if ( arch64Checked == false && values[3].contains("64") )
      continue;
    
    if ( arch32Checked == false && values[3].contains("32") )
      continue;

    if ( basicChecked == false && name.contains("Basic") )
      continue;

    if ( desktopChecked == false && name.contains("Desktop") )
      continue;  

    if ( testChecked == false && ( values[2].contains("0") || name.contains("BOINC") ) )
      continue;

    if (name.contains("BOINC"))
      edition = "BOINC_";
    else if (name.contains("Desktop"))
      edition = "Desktop_";
    else
      edition = "Basic_";        

    listEntry.id = id;
    listEntry.url = values[4];
    listEntry.name =  name + " " + values[1] + " (" + values[3] + " bit)"; // Name + version + arch
    listEntry.edition = edition;
      
    if (values[3].compare("32") == 0)
      listEntry.arch = "x86";
    else
      listEntry.arch = "x86_64";
    
    releaseList.append(listEntry);

    //item = new QListWidgetItem (listEntry.name, availableReleases);
    //item->setToolTip(listEntry.url);

    availableReleasesTbl->setRowCount(availableReleasesTbl->rowCount() + 1);
    editionItem = new QTableWidgetItem(name);
    availableReleasesTbl->setItem (id, 0, editionItem);   
    
    versionItem = new QTableWidgetItem(values[1]);
    versionItem->setTextAlignment(Qt::AlignHCenter);
    availableReleasesTbl->setItem (id, 1, versionItem);   
       
    archItem = new QTableWidgetItem (listEntry.arch);
    archItem->setTextAlignment(Qt::AlignHCenter);
    availableReleasesTbl->setItem (id, 2, archItem);   
  
    availableReleasesTbl->setRowHeight(id, 18);
    ++id;
  }

  availableReleasesTbl->resizeColumnToContents(0);
  availableReleasesTbl->horizontalHeader()->setStretchLastSection(true);

  //availableReleases->setCurrentItem(0);
  currentListElem = 0;
  //availableReleases->item(0)->setSelected(true);

  availableImagesList = "";
  httpReply->deleteLater();
}

void CVMISelectPage::arch64Toggled(bool checked)
{ 
  arch64Checked = checked;

  // Make sure we never end up with an empty list 
  if (arch64Checked == false && arch32Checked == false)
  {
    arch32CheckBox->setChecked(true);
    arch32Checked = true;
  } 

  populateList(); 
}
void CVMISelectPage::arch32Toggled(bool checked)
{ 
  arch32Checked = checked; 

  // Make sure we never end up with an empty list
  if (arch32Checked == false && arch64Checked == false)
  {
    arch64CheckBox->setChecked(true);
    arch64Checked = true;
  }

  populateList(); 
}

void CVMISelectPage::basicToggled(bool checked)
{ 
  basicChecked = checked; 
  
  // Make sure we never end up with an empty list 
  if (basicChecked == false && desktopChecked == false)
  {
    desktopCheckBox->setChecked(true);
    desktopChecked = true;
  }  
  populateList(); 
}

void CVMISelectPage::desktopToggled(bool checked)
{ 
  desktopChecked = checked; 

  // Make sure we never end up with an empty list 
  if (desktopChecked == false && basicChecked == false)
  {
    basicCheckBox->setChecked(true);
    basicChecked = true;
  }

  populateList(); 
}

void CVMISelectPage::displayOptionChanged()
{
  populateList();
}


