#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QSettings>
#include <QFileInfo>
#include <QApplication>

#include "pdfpreview.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void getKey(int num);



private:
    Ui::MainWidget *ui;

    QTimer *tFailedResp;
    QTimer *tNoNumber;
    QTimer *srvTimer;
    QTimer *srvBackTimer;

    QNetworkAccessManager *netManager;
    QNetworkAccessManager *dlmanager;
    QString baseUrl;
    QString baseLocation;
    QString cuurentPdfName;
    QString currentMsg;

    bool isPrint;
    QString currentNum;



    void goToPreview(QString path);

    void ShowPdfPreview(QList<QImage> imglist, int Pnum);

    void getPdf(QString pdfNum);
    void doDlFile(QString dlUrl);

    bool MP;

    void ConnectAllBtn();
    void DisconnectAllBtn();

    //PdfPreview preview;
public slots:
    void backToMain();


private slots:
    void replyFinished(QNetworkReply *reply);
    void replyDlFinished(QNetworkReply *reply);

    void resetMainUi();
    void noSrvResp();
    void noBtSrvBack();

    void on_pushButton_12_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
};

#endif // MAINWIDGET_H
