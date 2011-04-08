/****************************************************************************
** Copyright (C) 2010 CERN 
** All rights reserved.
** Contact: CernVM project (cernvm-talk@cern.ch)
** Author: Artem Harutyunyan (Artem.Harutyunyan@cern.ch)
****************************************************************************/

#ifndef CVMIINSTALLPAGE_H
#define CVMIINSTALLPAGE_H

#include <QtGui>
#include <QtNetwork>

#include "CVMIData.h" 

class CVMIInstallPage : public QWidget
{
  Q_OBJECT

  public:
    CVMIInstallPage(QWidget* parent = 0); 
    void start (CVMIRelease vmRelease, CVMIInstanceConfig vmInstance);
    CVMIExitStatus checkConfig(); 

  signals:
    void done();
    //void configProblem();

  private slots:
    void httpFinished ();
    void httpReadyRead ();
    void httpUpdateProgress (qint64 bytesRead, qint64 bytesTotal);
    void unzipDone (int exitCode, QProcess::ExitStatus exitStatus);
    void untarDone (int exitCode, QProcess::ExitStatus exitStatus);
    void vmReady ();
    void vmError (QString msg);
    void donePressed ();
    

  private:
    void startDownload();
    void httpStart();
    void unzip();
    void untar();
    bool cleanup (QString dir);
    void setImagePaths();    

    QLabel* label;
    QProgressBar* progressBar;
    QPushButton* doneButton;
 
    CVMIRelease           currentRelease;
    CVMIInstanceConfig    currentInstanceConfig;
    QUrl                  imageURL;
    QString               confImageStorageDir;
    QDir                  dir;
    QString               installDir;
    QFile*                compressedImage;
    QNetworkReply*        httpReply;
    QNetworkAccessManager qnam;
    QString               tmpUncompressedImage; 
    QString               tmpUncompressedImageDir;   
    QProcess              process;
    QString               confWorkDir;
    QString               imagePath;
};

#endif// CVMIINSTALLPAGE_H
