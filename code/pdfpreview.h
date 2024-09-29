#ifndef PDFPREVIEW_H
#define PDFPREVIEW_H

#include <QWidget>
#include <poppler-qt5.h>
#include <QList>
#include <QImage>
#include <QDebug>
#include <QLabel>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QImage>


namespace Ui {
class PdfPreview;
}

class PdfPreview : public QWidget
{
    Q_OBJECT

public:
    explicit PdfPreview(QWidget *parent = 0);
    ~PdfPreview();

    QList<QImage> myImgList;
    int PageNum;
    int currentPg;
    bool isPrinted;

    QString filepath;

    QTimer *rstTimer;
    QTimer *tPrint;

    int renderPdf(QString path);

    void showPages(QImage image);

    QNetworkAccessManager *printmanager;
    QString BaseUrl;
    QString pdfNum;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void conPrinBtn();

signals:
    void finish();

private:
    Ui::PdfPreview *ui;
};

#endif // PDFPREVIEW_H
