# hospital-kiosk
Hospital Kiosk  - Embedded C++ &amp; Linux platform - 


Raspberry pi 3
Qt 5.7.1
modules: core gui network

printer: Canon_LBP151 (CUPS Canon_LBP 4sx)
tocuh: EGalxs( xinput_callibration)

print in terminal: lp -d(in terminal) 
	(in Qt in terminal)
	QProcess terminal;
    	QString tmlString = "lp -d \"Canon_LBP151\" " + filepath;
    	qDebug() << tmlString;
    	terminal.start("sh", QStringList() << "-c" << tmlString);
    	terminal.waitForFinished();
    	QApplication::processEvents();

pdf rendering: poppler-qt5
_____________________________
add to .pro
win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/arm-linux-gnueabihf/release$
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/arm-linux-gnueabihf/debu$
else:unix: LIBS += -L/usr/lib/arm-linux-gnueabihf/ -lpoppler-qt5

INCLUDEPATH += /usr/lib/arm-linux-gnueabihf
INCLUDEPATH += /usr/include/poppler/qt5
DEPENDPATH += /usr/lib/arm-linux-gnueabihf

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/arm-linux-gnueabihf/release$
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/arm-linux-gnueabihf/debu$
else:unix: LIBS += -L/usr/lib/arm-linux-gnueabihf/ -lpoppler-qt5

INCLUDEPATH += /usr/lib/arm-linux-gnueabihf
DEPENDPATH += /usr/lib/arm-linux-gnueabihf

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/lib/arm-linux-gnuea$
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/lib/arm-linux-gn$
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/lib/arm$
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/lib/arm-l$
else:unix: PRE_TARGETDEPS += /usr/lib/arm-linux-gnueabihf/libpoppler-qt5.a

INCLUDEPATH += /usr/local/share/fonts

_______________________________________________
	#include <poppler-qt5.h>
	#include <QList>
	#include <QImage>
	#include <QDebug>
	#include <QLabel>
	#include <QFile>
	#include <QTimer>
	#include <QProcess>
	#include <QApplication>

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

___________________________________________________________________________

add B Nazanin font
cp *.ttf ~/.fonts/
fc-cache -v -f

push button transparency
border:0px;
background:0px;

label transparenc;y
background:0px;
background-image:url(<path>);

