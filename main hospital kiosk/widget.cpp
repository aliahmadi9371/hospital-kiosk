#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
/*  1	MRI
    2	سیتی اسکن
    3	سونوگرافی
    4	سنجش تراکم استخوان
    5	ماموگرافی
    6	سایر خدمات
    7	آزمایشگاه و پاتولوژی
    8	جوابدهی تصویربرداری
    9	جواب دهی آزمایشگاه
    10	رادیولوژي
    11  پذیرش */

    ui->setupUi(this);

//    QString inipath ="C:\\KioskResource\\data.ini" ;
//    QFile inifile(inipath);

//    //qDebug() << inifile.exists(inipath);
//    QSettings dbSetting(inipath,QSettings::IniFormat);

//    QString gDbHost = dbSetting.value("db/host", "192.168.1.100").toString();
//    QString gDbName = dbSetting.value("db/name", "queue_management").toString();
//    QString gDbPass = dbSetting.value("db/pass", "123456").toString();
//    QString gDbUser = dbSetting.value("db/user", "root").toString();

//    qDebug() << gDbHost;
//    qDebug() << gDbName;
//    qDebug() << gDbPass;
//    qDebug() << gDbUser;

//    qDebug() << QSqlDatabase::isDriverAvailable("QMYSQL");
//    QSqlDatabase dbCon = QSqlDatabase::addDatabase("QMYSQL");
//    dbCon.setHostName(gDbHost);
//    dbCon.setUserName(gDbUser);
//    dbCon.setPassword(gDbPass);
//    dbCon.setDatabaseName(gDbName);
//    dbCon.open();

//    qDebug() << dbCon.isOpen();

    initDataBase();

    ui->srvStatus->hide();

    timer = new QTimer(this);
    timer->setSingleShot(true);

    lblWaiting = new QLabel(this);
    QRect RlblWaiting(0,200,800,200);
    lblWaiting->setGeometry(RlblWaiting);
    QFont font("B Nazanin",50);
    lblWaiting->setFont(font);
    QPalette pal = lblWaiting->palette();
    pal.setColor(QPalette::Background,QColor(Qt::white));
    lblWaiting->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lblWaiting->setHidden(true);
    lblWaiting->setAutoFillBackground(true);
    lblWaiting->setPalette(pal);
    lblWaiting->setStyleSheet("QLable {background-color : white}");
    lblWaiting->setText("لطفا شماره خود را بردارید");

    qtimer = new QTimer(this);
    qtimer->start(5000);

    connect(timer,SIGNAL(timeout()),this,SLOT(connectall()));
    connect(qtimer,SIGNAL(timeout()),this,SLOT(queueQuery()));
}

Widget::~Widget()
{
    timer->deleteLater();
    lblWaiting->deleteLater();
    delete ui;
}

 void Widget::diconnectall()
{
     disconnect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
     disconnect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
     disconnect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
     disconnect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
     disconnect(ui->pushButton_5,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
     disconnect(ui->pushButton_6,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
     disconnect(ui->pushButton_7,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
     disconnect(ui->pushButton_10,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()));
}

 void Widget::connectall()
{
     connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()),Qt::QueuedConnection);
     connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()),Qt::QueuedConnection);
     connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()),Qt::QueuedConnection);
     connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()),Qt::QueuedConnection);
     connect(ui->pushButton_5,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()),Qt::QueuedConnection);
     connect(ui->pushButton_6,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()),Qt::QueuedConnection);
     connect(ui->pushButton_7,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()),Qt::QueuedConnection);
     connect(ui->pushButton_10,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_5_clicked()),Qt::QueuedConnection);

     lblWaiting->setHidden(true);
}

 void Widget::queueProc(int servicetype,QString serviceName)
 {
     QSqlQuery query;
     QString strquery;
     strquery = " CALL `GetNewNumber1`(%1); ";
     strquery = strquery.arg(QString::number(servicetype));
     qDebug()<< strquery;

     int nember = 0;
     int waiting = 0;
     if (query.exec(strquery)){
         if (query.next()){
             nember = query.value(0).toInt();
             waiting = query.value(1).toInt();
         }else{
             qDebug() << "not return";
             qDebug() << query.lastError().text();
             qDebug() << query.isValid();

              lblWaiting->setHidden(true);
             return ;
         }
     }else{
         qDebug() << query.lastError().text();
          lblWaiting->setHidden(true);
         return;
     }
     print(serviceName,nember,waiting);
 }
// int Widget::createNewNum()
// {
//     QSqlQuery query;
//     query.prepare("select IFNULL( max(numberInDay),0)+1 as num from queue where date(queue.datetime) = date(now());");
//     if (query.exec()){
//             query.first();
//             return query.value(0).toInt();
//         }
//     return 0;
// }

// int Widget::creteNewQueue(int service)
// {
//    if(service == 7)
//    {
//        QSqlQuery query;
//        query.prepare("select count(*) from queue where `status` = 1 and date(datetime) = date(now()) and serviceType = 7");
//        if (query.exec()){
//                query.first();
//                return query.value(0).toInt();
//            }
//    }
//    else
//    {
//        QSqlQuery query;
//        query.prepare("select count(*) from queue where `status` = 1 and date(datetime) = date(now()) and serviceType <> 7");
//        if (query.exec()){
//                query.first();
//                return query.value(0).toInt();
//            }
//    }
//    return 0;
// }

// void Widget::insertToDb(int num, int service)
// {
//     QSqlQuery query;
//     query.prepare("insert into queue (`status`,numberInDay,step,numInStep,serviceType,datetime) VALUES (1,:numInD ,1,1, :ServiceType,now())");
//     query.bindValue(":numInD", num);
//     query.bindValue(":ServiceType", service);
//     qDebug() << query.boundValue(":numInD");
//     qDebug() << query.boundValue(":ServiceType");
//    qDebug() << query.lastQuery();
//     if(query.exec());
//     else
//        qDebug() << query.lastError().text();
//     return;
// }

 void Widget::print(QString serve,int nownum, int lastnums)
 {
     qDebug() << "print!";

     QPrinter printer(QPrinter::HighResolution);
     printer.setOrientation(QPrinter::Portrait);
     printer.setOutputFormat(QPrinter::NativeFormat);
     printer.setPageMargins(2,2,2,2,QPrinter::Millimeter);

     const QRect Rmain = QRect(0,0,550,350);
     const QRect Rlogo = QRect(317,0,233,77);
     const QRectF Rtitle = QRectF(0,90,545,73);
     const QRectF Rdate = QRectF(0,0,200,123);
     const QRectF Rnumber = QRectF(275,150,265,200);
     const QRectF Rserve = QRectF(0,140,225,200);

     QPainter painter;
     painter.begin(&printer);

     QPen pen;
     pen.setStyle(Qt::DashLine);
     painter.drawRect(Rmain.adjusted(0,0,-pen.width(),-pen.width()));



     QFont f1("B Nazanin",10);
     painter.setFont(f1);
     QDateConvertor mdate;
     QStringList today = mdate.Today();
     QString month = today[3];
     QString nday = today[2];
     QString day = today[4];
     QString year = today[0];
     QString date1 = day + " " + nday + " " + month;
     QString date2 = QString("سال")  + " " + year;
     QString Time = QString("ساعت") + " " + QDateTime::currentDateTime().toString("hh:mm:ss");
     QString datetime = date1 + "\n" + date2 + "\n" + Time;
     painter.drawText(Rdate, Qt::AlignCenter | Qt::AlignTop,datetime);


     QImage img("C:\\KioskResource\\map.png");
     painter.drawImage(Rlogo,img);

     QFont f("B Nazanin",10);
     painter.setFont(f);
     painter.drawText(Rtitle,Qt::AlignRight,serve);



     QString num1 = "شماره";
     QString num2 ;
     num2 = QString::number(nownum);
     num1 = num1 + "\n" +num2;
     QFont f2("B Nazanin",20,QFont::Bold);
     painter.setFont(f2);
     painter.drawText(Rnumber,Qt::AlignRight,num1);

     QFont f3("B Nazanin",10,QFont::Bold);
     painter.setFont(f3);
     QString nnum1 = "تعداد نفرات در انتظار";
     QString nnum2 ;
     nnum2 = QString::number(lastnums);
     nnum1 = nnum1 + "\n" + nnum2;
     painter.drawText(Rserve,Qt::AlignCenter,nnum1);



     painter.end();

 }

 void Widget::on_pushButton_5_clicked() // 3
 {

     lblWaiting->setHidden(false);
     diconnectall();
     QApplication::processEvents();

     timer->start(2500);
     queueProc(3,"سونوگرافی");
 }

void Widget::on_pushButton_2_clicked() // 2
{
    lblWaiting->setHidden(false);
    diconnectall();
    QApplication::processEvents();

    timer->start(2500);
    queueProc(2,"سی تی اسکن");
}

void Widget::on_pushButton_3_clicked() // 1
{
    lblWaiting->setHidden(false);
    diconnectall();
    QApplication::processEvents();

    timer->start(2500);
    queueProc(1,"MRI");
}

void Widget::on_pushButton_4_clicked() // 7
{
    lblWaiting->setHidden(false);
    diconnectall();
    QApplication::processEvents();

    timer->start(2500);
    queueProc(7,"آزمایشگاه و پاتوبیولوژی");
}

void Widget::on_pushButton_clicked() // 5
{
    lblWaiting->setHidden(false);
    diconnectall();
    QApplication::processEvents();

    timer->start(2500);
    queueProc(5,"ماموگرافی");
}

void Widget::on_pushButton_6_clicked() // 4
{
    lblWaiting->setHidden(false);
    diconnectall();
    QApplication::processEvents();

    timer->start(2500);
    queueProc(4,"سنجش تراکم استخوان");
}

void Widget::on_pushButton_7_clicked() // 10
{
    lblWaiting->setHidden(false);
    diconnectall();
    QApplication::processEvents();

    timer->start(2500);
    queueProc(10,"رادیولوژی");
}

void Widget::on_pushButton_10_clicked() // 6
{
    lblWaiting->setHidden(false);
    diconnectall();
    QApplication::processEvents();

    timer->start(2500);
    queueProc(6,"سایر");
}


void Widget::queueQuery()
{
    QSqlQuery query;
    query.prepare("select * from ServiceType limit 1");
    if(query.exec()){
              ui->srvStatus->hide();
    }
    else{
          ui->srvStatus->show();
        qDebug() << query.lastError().text();
        initDataBase();
    }

    return;
}

void Widget::initDataBase()
{
    QString inipath ="C:\\KioskResource\\data.ini" ;
    QFile inifile(inipath);

    qDebug() << inifile.exists(inipath);
    QSettings dbSetting(inipath,QSettings::IniFormat);

    QString gDbHost = dbSetting.value("db/host", "192.168.1.100").toString();
    QString gDbName = dbSetting.value("db/name", "queue_management").toString();
    QString gDbPass = dbSetting.value("db/pass", "123456").toString();
    QString gDbUser = dbSetting.value("db/user", "root").toString();

    qDebug() << gDbHost;
    qDebug() << gDbName;
    qDebug() << gDbPass;
    qDebug() << gDbUser;

    qDebug() << QSqlDatabase::isDriverAvailable("QMYSQL");
    QSqlDatabase dbCon = QSqlDatabase::addDatabase("QMYSQL");
    dbCon.setHostName(gDbHost);
    dbCon.setUserName(gDbUser);
    dbCon.setPassword(gDbPass);
    dbCon.setDatabaseName(gDbName);
    dbCon.open();

    qDebug() << dbCon.isOpen();
}
