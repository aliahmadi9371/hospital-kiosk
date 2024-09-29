
#include "mainwidget.h"
#include "ui_mainwidget.h"


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    ui->label->setHidden(true);
    ui->label_2->setHidden(true);
    isPrint= false;

    netManager = new QNetworkAccessManager(this);
    dlmanager = new QNetworkAccessManager(this);

    tNoNumber = new QTimer(this);
    tFailedResp = new QTimer(this);
    srvTimer = new QTimer(this);
    srvBackTimer = new QTimer(this);

    tNoNumber->setSingleShot(true);
    tFailedResp->setSingleShot(true);
    srvTimer->setSingleShot(true);
    srvBackTimer->setSingleShot(true);




    connect(netManager, SIGNAL(finished(QNetworkReply*)), this,SLOT(replyFinished(QNetworkReply*)),Qt::QueuedConnection);
    connect(dlmanager, SIGNAL(finished(QNetworkReply*)), this,SLOT(replyDlFinished(QNetworkReply*)),Qt::QueuedConnection);

    connect(tNoNumber, SIGNAL(timeout()), this,SLOT(resetMainUi()),Qt::QueuedConnection);
    connect(tFailedResp, SIGNAL(timeout()), this,SLOT(resetMainUi()),Qt::QueuedConnection);
    connect(srvTimer, SIGNAL(timeout()), this,SLOT(noSrvResp()),Qt::QueuedConnection);
    connect(srvBackTimer, SIGNAL(timeout()), this,SLOT(noBtSrvBack()),Qt::QueuedConnection);

    QSettings setting(QString("/home/pi/MainApp/Setting/config.ini"),QSettings::IniFormat);
    baseUrl = setting.value("urlinfo/baseurl","https://dacs.ir/pdf-lab/inquiry?code=").toString();
    baseLocation = setting.value("location/basepdflocation","/home/pi/MainApp/PDFs/").toString();

    resetMainUi();
    MP = false; //Main page
}

MainWidget::~MainWidget()
{
    netManager->deleteLater();
    dlmanager->deleteLater();
    tNoNumber->deleteLater();
    tFailedResp->deleteLater();
    srvBackTimer->deleteLater();
    srvBackTimer->deleteLater();

    delete ui;
}

void MainWidget::on_pushButton_clicked()
{
    getKey(1);
}

void MainWidget::on_pushButton_2_clicked()
{
    getKey(2);
}

void MainWidget::on_pushButton_3_clicked()
{
    getKey(3);
}

void MainWidget::on_pushButton_9_clicked()
{
    getKey(4);
}

void MainWidget::on_pushButton_8_clicked()
{
    getKey(5);
}

void MainWidget::on_pushButton_4_clicked()
{
    getKey(6);
}

void MainWidget::on_pushButton_7_clicked()
{
    getKey(7);
}

void MainWidget::on_pushButton_6_clicked()
{
    getKey(8);
}

void MainWidget::on_pushButton_5_clicked()
{
    getKey(9);
}

void MainWidget::on_pushButton_10_clicked()
{
    getKey(0);
}

void MainWidget::getKey(int num)
{
    resetMainUi();
    ui->lineEdit->insert(QString::number(num));
}

void MainWidget::on_pushButton_11_clicked()//clear button
{
    backToMain();
}

void MainWidget::backToMain()
{
    resetMainUi();
    ui->lineEdit->clear();
    MP = false;
}

void MainWidget::resetMainUi()
{
    tNoNumber->stop();
    tFailedResp->stop();
    srvBackTimer->stop();
    ui->label->setHidden(true);
    ui->label_2->clear();
    ui->label_2->setHidden(true);
}

void MainWidget::noSrvResp()
{
    delete netManager;
    netManager = new QNetworkAccessManager(this);
    connect(netManager, SIGNAL(finished(QNetworkReply*)), this,SLOT(replyFinished(QNetworkReply*)),Qt::QueuedConnection);

    ui->label_2->clear();
    ui->lineEdit->clear();
    ui->label_2->setText("پاسخی از شبکه دریافت نشد");
    ui->label_2->setHidden(false);
    ConnectAllBtn();
    srvBackTimer->start(3000);
}

void MainWidget::noBtSrvBack()
{
    backToMain();
}

void MainWidget::on_pushButton_12_clicked()
{
    if(ui->lineEdit->text()==""){
        ui->label->setHidden(false);//please enter number
        ui->label_2->clear();
        ui->label_2->setHidden(true);
        tNoNumber->start(5000);
           return;
    }
    DisconnectAllBtn();
    cuurentPdfName = ui->lineEdit->text();
    ui->lineEdit->clear();
    ui->lineEdit->setText("لطفا صبر کنید.");
    QApplication::processEvents();

    qDebug() << cuurentPdfName;
    getPdf(cuurentPdfName);
}

void MainWidget::getPdf(QString pdfNum)
{
    currentNum = pdfNum;
    QUrl getUrl = QUrl(baseUrl + pdfNum);
    QNetworkRequest request(getUrl);
    srvTimer->start(20000);
    netManager->get(request);
    return;
}

void MainWidget::replyFinished(QNetworkReply *reply)
{
    qDebug() << "finished!";
    QByteArray response_data = reply->readAll();
    qDebug()<< "read all reply data";
    QJsonParseError error_json;
    QJsonDocument jsonresp = QJsonDocument::fromJson(response_data, &error_json);
    QJsonObject repobj = jsonresp.object();
    if(repobj.value("status") == "200"){
        srvTimer->stop();
        isPrint = repobj.value("printed").toBool();
        qDebug() << "printed: " << repobj.value("printed");
        resetMainUi();
        doDlFile(repobj.value("message").toString());
        return;
    }
    else if(repobj.value("status") == "400"){
        qDebug() << repobj.value("message").toString();
        resetMainUi();
        srvTimer->stop();
        ui->lineEdit->clear();
        ui->label_2->setText(repobj.value("message").toString());
        ui->label_2->setHidden(false);
        tFailedResp->start(5000);
        ConnectAllBtn();
        return;
    }
    qDebug() << "error response";
    reply->deleteLater();
    return;
}

void MainWidget::doDlFile(QString dlUrl)
{
    QUrl getUrl = QUrl(dlUrl);
    QNetworkRequest request(getUrl);
    dlmanager->get(request);
    return;
}

void MainWidget::replyDlFinished(QNetworkReply *reply)
{
    qDebug() << " Download finished!";
    QString filePath = baseLocation + cuurentPdfName + ".pdf";
    qDebug() << filePath;
    QFileInfo check_file(filePath);
    if(check_file.exists() && check_file.isFile()){
        qDebug() << "exsisted file!";
        goToPreview(filePath);
        return;
    }
    qDebug() << "file is not exsisted! - we save it";
    QFile myFile(filePath);
    if(myFile.isOpen())
        myFile.close();
    if(!myFile.open(QIODevice::WriteOnly)){
        qDebug() << "can not open or create file";
        ConnectAllBtn();
        backToMain();
        return;
    }
    myFile.write(reply->readAll());
    myFile.close();
    reply->deleteLater();
    goToPreview(filePath);
    return;
}

void MainWidget::goToPreview(QString path)
{
    resetMainUi();
    ui->lineEdit->clear();

    qDebug() << "pdf preview";

    PdfPreview *preview = new PdfPreview(this);
    MP = true;
    ConnectAllBtn();
    preview->isPrinted = isPrint;
    preview->pdfNum = currentNum;
    preview->showFullScreen();
    preview->renderPdf(path);
}

void MainWidget::DisconnectAllBtn()
{
    disconnect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_clicked()));
    disconnect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_2_clicked()));
    disconnect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_3_clicked()));
    disconnect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_4_clicked()));
    disconnect(ui->pushButton_5,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
    disconnect(ui->pushButton_6,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_6_clicked()));
    disconnect(ui->pushButton_7,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_7_clicked()));
    disconnect(ui->pushButton_8,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_8_clicked()));
    disconnect(ui->pushButton_9,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_9_clicked()));
    disconnect(ui->pushButton_10,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_10_clicked()));
    disconnect(ui->pushButton_11,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_11_clicked()));
    disconnect(ui->pushButton_12,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_12_clicked()));
}

void MainWidget::ConnectAllBtn()
{
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_clicked()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_2_clicked()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_3_clicked()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_4_clicked()));
    connect(ui->pushButton_5,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
    connect(ui->pushButton_6,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_6_clicked()));
    connect(ui->pushButton_7,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_7_clicked()));
    connect(ui->pushButton_8,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_8_clicked()));
    connect(ui->pushButton_9,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_9_clicked()));
    connect(ui->pushButton_10,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_10_clicked()));
    connect(ui->pushButton_11,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_11_clicked()));
    connect(ui->pushButton_12,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_12_clicked()));
}
