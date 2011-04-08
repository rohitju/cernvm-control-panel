#ifndef CVMIMACHINES_H
#define CVMIMACHINES_H

#include <QtGui>

class CVMIMachines : public QWidget
{
    Q_OBJECT

    public:
      CVMIMachines(QWidget* parent = 0);

    private:

      QTableWidget *availableMachinesTbl;
      QPushButton *suspend;
      QPushButton *start;
      QPushButton *stop;
      QPushButton *resume;

  private slots:
      void startvm(bool checked);
      void stopvm(bool checked);

};
#endif // CVMIMACHINES_H
