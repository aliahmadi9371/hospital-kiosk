#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QPrinter>
#include <QPrintDialog>
#include <QtPrintSupport>
#include <QFont>
#include <QString>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPrinterInfo>

#include "qdateconvertor.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QTimer *timer;
    QTimer *qtimer;
    QSqlDatabase sqlDb;
    QLabel *lblWaiting;



private slots:

    void queueQuery();

    void connectall();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::Widget *ui;

//    int createNewNum();
//    int creteNewQueue(int service);
//    void insertToDb(int num, int service);

    void queueProc(int servicetype, QString serviceName);
    void print(QString serve,int nownum, int lastnums);
    void diconnectall();

    void initDataBase();
};

#endif // WIDGET_H
