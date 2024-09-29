#include "pdfpreview.h"
#include "ui_pdfpreview.h"

PdfPreview::PdfPreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PdfPreview)
{
    ui->setupUi(this);
    PageNum = 0;
    currentPg = -1;
    filepath = "";

    rstTimer = new QTimer(this);
    tPrint = new QTimer(this);

    connect(rstTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_clicked()),Qt::QueuedConnection);
    connect(tPrint,SIGNAL(timeout()),this,SLOT(conPrinBtn()),Qt::QueuedConnection);

    rstTimer->setSingleShot(true);
    rstTimer->start(25000);

    tPrint->setSingleShot(true);

    printmanager = new QNetworkAccessManager(this);

    QImage image("/home/pi/MainApp/Res/MainUiComponents/A.png");
    image = image.scaled(160,160,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    ui->label_3->setPixmap(QPixmap::fromImage(image));
    ui->label_3->setHidden(true);

    QSettings setting(QString("/home/pi/MainApp/Setting/config.ini"),QSettings::IniFormat);
    BaseUrl = setting.value("urlinfo/printurl","httpd://dacs.ir/pdf-lab/print?code=").toString();

}

PdfPreview::~PdfPreview()
{
    rstTimer->deleteLater();
    printmanager->deleteLater();
    delete ui;
}

int PdfPreview::renderPdf(QString path)
{
    using namespace Poppler;

    filepath = path;

    Document* document = Document::load(path);

    //document->setRenderBackend(Document::SplashBackend);
    //document->setRenderHint(Document::OverprintPreview);
    //document->setRenderHint (Document::Antialiasing);
    document->setRenderHint(Document::TextAntialiasing);

    int pNum = document->numPages();
    qDebug() << "page number = " << pNum;



    for (int i=0; i<pNum; i++)
    {
        myImgList.append(document->page(i)->renderToImage());

    }
    PageNum = pNum;
    currentPg = 0;
    showPages(myImgList.at(currentPg));
    return pNum;
}

void PdfPreview::showPages(QImage image)
{
    ui->label->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    ui->label->setText(QString::number(currentPg + 1) + "/" + QString::number(PageNum));

    qDebug() << image.size();

    QIcon icon;
    QSize size(400,465);
    if (!image.isNull())
      icon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
     QPixmap pixmap = icon.pixmap(size, QIcon::Normal, QIcon::On);

    qDebug() << pixmap.size();

    ui->label_2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->label_2->setPixmap(pixmap);
}

void PdfPreview::on_pushButton_clicked()
{


    QFile::remove(filepath);
    filepath = "";
    currentPg = -1;
    PageNum = -1;
    ui->label_3->setHidden(true);
    emit this->finish();
    rstTimer->stop();
    this->setHidden(true);
}

void PdfPreview::on_pushButton_5_clicked()
{
    rstTimer->stop();
    rstTimer->start(15000);
    if(currentPg == 0)
        return;
    else{
        currentPg = currentPg - 1;
        showPages(myImgList.at(currentPg));
    }
}

void PdfPreview::on_pushButton_3_clicked()
{
    rstTimer->stop();
    rstTimer->start(15000);
    if(currentPg == (PageNum - 1))
        return;
    else{
        currentPg = currentPg + 1;
        showPages(myImgList.at(currentPg));
    }
}

void PdfPreview::on_pushButton_2_clicked()
{
    if(!isPrinted){
        rstTimer->stop();
        rstTimer->start(15000);
        disconnect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_2_clicked()));
        tPrint->start(10000);
        QProcess terminal;
        QString tmlString = "lp -d \"Canon_LBP151\" " + filepath;
        qDebug() << tmlString;
        terminal.start("sh", QStringList() << "-c" << tmlString);
        terminal.waitForFinished();
        QApplication::processEvents();

        QUrl getUrl = QUrl(BaseUrl + pdfNum);
        QNetworkRequest request(getUrl);
        printmanager->get(request);
    }
    else{
        qDebug() << "pdf is printed before";
        ui->label_3->setHidden(false);
    }
}

void PdfPreview::conPrinBtn()
{
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_2_clicked()));
}
