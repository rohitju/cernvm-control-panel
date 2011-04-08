/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#include "CVMICreatePage.h"

CVMICreatePage::CVMICreatePage(QWidget* parent) : QWidget(parent) 
{
  const unsigned int defaultRam = 256;
  const unsigned int defaultCPU = 1;

  instanceGroupBox = new QGroupBox(tr("Virtual Machine configuration"));

  // RAM Slider
  QLabel* ramLabel = new QLabel (tr("Memory: "));
  ramSlider = new QSlider (Qt::Horizontal);
  ramSlider->setRange (1, 32);
  ramSlider->setValue (1);
  ramValue = new QLabel (QString::number(defaultRam) + " Mb");

  Q_ASSERT(connect(ramSlider, SIGNAL(valueChanged(int)), this, SLOT(ramSliderChanged(int)))); 

  // CPU slider
  QLabel* cpuLabel = new QLabel (tr("Processor(s): "));
  cpuSlider = new QSlider (Qt::Horizontal);
  cpuSlider->setRange (1, 8);
  cpuSlider->setValue (defaultCPU); 
  cpuValue = new QLabel (QString::number(defaultCPU));
  
  Q_ASSERT(connect(cpuSlider, SIGNAL(valueChanged(int)), this, SLOT(cpuSliderChanged(int))));

  // Name box 
  QLabel* nameLabel = new QLabel(tr("Name: "));
  vmName = new QLineEdit();

  // Buttons 
  QPushButton* proceedButton = new QPushButton(tr("Proceed"));
  Q_ASSERT(connect(proceedButton, SIGNAL(clicked()), this, SLOT(proceedPressed())));

  QPushButton* backButton = new QPushButton(tr("Back"));
  Q_ASSERT(connect(backButton, SIGNAL(clicked()), this, SLOT(backPressed())));  

  // Layout
  QGridLayout* instanceLayout = new QGridLayout;
  instanceLayout->addWidget(ramLabel, 0, 0, 1, 1, Qt::AlignRight);
  instanceLayout->addWidget(ramSlider, 0, 1);
  instanceLayout->addWidget(ramValue, 0, 2);

  instanceLayout->addWidget(cpuLabel, 1, 0, 1, 1, Qt::AlignRight);
  instanceLayout->addWidget(cpuSlider, 1, 1);
  instanceLayout->addWidget(cpuValue, 1, 2);

  instanceLayout->addWidget(nameLabel, 2, 0, 1, 1, Qt::AlignRight);
  instanceLayout->addWidget(vmName, 2, 1);

  instanceLayout->addWidget(proceedButton, 3, 2, 1, 1, Qt::AlignLeft);
  instanceLayout->addWidget(backButton, 3, 0);//, 1, 1, Qt::AlignLeft);

  instanceGroupBox->setLayout(instanceLayout);

  QGridLayout* mainLayout = new QGridLayout;
  mainLayout->addWidget(instanceGroupBox, 0, 0, 1, 3, Qt::AlignTop); 
 
  setLayout(mainLayout); 
}

void CVMICreatePage::proceedPressed()
  {  emit createVM(getVMConfig());  }

void CVMICreatePage::backPressed()
  {  emit back(); }

CVMIInstanceConfig CVMICreatePage::getVMConfig()
{
  CVMIInstanceConfig c;
  c.name = vmName->text();
  c.ramSize = QString::number(256 * ramSlider->value());
  c.cpuCount = QString::number(cpuSlider->value());

  return c;
}

void CVMICreatePage::setTitle (QString name)
  { instanceGroupBox->setTitle(name); }

void CVMICreatePage::setName (QString name)  
{ 
  vmName->setText(name);
  vmName->setCursorPosition(0); 
}

QString CVMICreatePage::getName()
  { return vmName->text(); }

QString CVMICreatePage::getRamSize()
  { return QString::number(ramSlider->value()); }

QString CVMICreatePage::getCPUCount()
  { return QString::number(cpuSlider->value()); }

void CVMICreatePage::ramSliderChanged (int value)
{ 
  ramValue->setText(QString::number(256 * value) + " Mb"); 
}

void CVMICreatePage::cpuSliderChanged (int value)
  { cpuValue->setText(QString::number(value)); }

