// Written: fmckenna

// Purpose: the typical Qt main for running a QMainWindow

#include <MainWindowWorkflowApp.h>
#include <QApplication>
#include <QFile>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTime>
#include <QTextStream>
#include <GoogleAnalytics.h>
#include <QStandardPaths>
#include <QDir>
#include <QStatusBar>
#include <QWebEngineView>

#include <TapisV3.h>
#include <WorkflowAppWE.h>
#include <SimCenterPreferences.h>
#include <stdlib.h>
#include <Utils/FileOperations.h>

#ifdef ENDLN
#undef ENDLN
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
#define ENDLN endl
#else
#define ENDLN Qt::endl
#endif

 // customMessgaeOutput code from web:
 // https://stackoverflow.com/questions/4954140/how-to-redirect-qdebug-qwarning-qcritical-etc-output

static QString logFilePath;
static bool logToFile = false;

void customMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QHash<QtMsgType, QString> msgLevelHash({{QtDebugMsg, "Debug"}, {QtInfoMsg, "Info"}, {QtWarningMsg, "Warning"}, {QtCriticalMsg, "Critical"}, {QtFatalMsg, "Fatal"}});
    QByteArray localMsg = msg.toLocal8Bit();
    QTime time = QTime::currentTime();
    QString formattedTime = time.toString("hh:mm:ss.zzz");
    QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
    QString logLevelName = msgLevelHash[type];
    QByteArray logLevelMsg = logLevelName.toLocal8Bit();

    if (logToFile) {
        QString txt = QString("%1 %2: %3 (%4)").arg(formattedTime, logLevelName, msg,  context.file);
        QFile outFile(logFilePath);
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << ENDLN;
        outFile.close();
    } else {
        fprintf(stderr, "%s %s: %s (%s:%u, %s)\n", formattedTimeMsg.constData(), logLevelMsg.constData(), localMsg.constData(), context.file, context.line, context.function);
        fflush(stderr);
    }

    if (type == QtFatalMsg)
        abort();
}


int main(int argc, char *argv[])
{

    //
    // Setting Core Application Name, Organization, Version and Google Analytics Tracking Id
    //

    QCoreApplication::setApplicationName("WE-UQ");
    QCoreApplication::setOrganizationName("SimCenter");
    QCoreApplication::setApplicationVersion("4.2.3");

    Q_INIT_RESOURCE(images1);

    //
    // set up logging of output messages for user debugging
    //

    // full path to debug.log file    
    logFilePath = SCUtils::getAppWorkDir();
    logFilePath = logFilePath + QDir::separator() + QString("debug.log");

    //
    // window scaling
    //
    
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // remove old log file
    QFile debugFile(logFilePath);
    if (debugFile.exists())
      debugFile.remove();

    QApplication mainApp(argc, argv);

    QByteArray envVar = qgetenv("QTDIR");       //  check if the app is run in Qt Creator

    if (envVar.isEmpty())
        logToFile = true;

    qInstallMessageHandler(customMessageOutput);

    qDebug() << "logFile: " << logFilePath;



    //
    // create a remote interface
    //

    QString tenant("designsafe");
    // QString storage("agave://designsafe.storage.default/");
    QString storage("designsafe.storage.default/");        
    QString dirName("WE-UQ");

    // AgaveCurl *theRemoteService = new AgaveCurl(tenant, storage, &dirName);
    TapisV3 *theRemoteService = new TapisV3(tenant, storage, &dirName);        

    //
    // create the main window
    //

    WorkflowAppWidget *theInputApp = new WorkflowAppWE(theRemoteService);
    MainWindowWorkflowApp w(QString("WE-UQ: Wind Engineering with Uncertainty Quantification"), theInputApp, theRemoteService);

    /*
    QFile aboutTXT(":/Resources/docs/textAboutWEUQ.html");
    aboutTXT.open(QIODevice::ReadOnly);
    QString textAboutWE = aboutTXT.readAll();
    aboutTXT.close();
    w.setAbout(textAboutWE);
    */

    QString aboutTitle = "About this SimCenter WE-UQApplication"; // this is the title displayed in the on About dialog
    QString aboutSource = ":/Resources/docs/textAboutWEUQ.html";  // this is an HTML file stored under resources
    w.setAbout(aboutTitle,aboutSource);

    QString version = QString("Version ") + QCoreApplication::applicationVersion();
    w.setVersion(version);

    QString citeText("1)Frank McKenna, Abiy Melaku, Fei Ding, Jiawei Wan, Peter Mackenzie-Helnwein, Michael Gardner, Sang-ri Yi, Aakash Bangalore Satish, & Wael Elhaddad. (2025). NHERI-SimCenter/WE-UQ: Version 4.2.0 (v4.2.0). Zenodo. https://doi.org/10.5281/zenodo.14895011 \n\n2) Gregory G. Deierlein, Frank McKenna, Adam Zsarnóczay, Tracy Kijewski-Correa, Ahsan Kareem, Wael Elhaddad, Laura Lowes, Matthew J. Schoettler, and Sanjay Govindjee (2020) A Cloud-Enabled Application Framework for Simulating Regional-Scale Impacts of Natural Hazards on the Built Environment. Frontiers in the Built Environment. 6:558706. doi: 10.3389/fbuil.2020.558706");
    
    w.setCite(citeText);

    // QString manualURL("https://www.designsafe-ci.org/data/browser/public/designsafe.storage.community//SimCenter/Software/WE_UQ/");
    QString manualURL("https://nheri-simcenter.github.io/WE-UQ-Documentation/");
    w.setDocumentationURL(manualURL);

    //    QString messageBoardURL("https://simcenter-messageboard.designsafe-ci.org/smf/index.php?board=5.0");
    QString messageBoardURL("https://github.com/orgs/NHERI-SimCenter/discussions/categories/we-uq");      w.setFeedbackURL(messageBoardURL);

    //
    // move remote interface to a thread
    //

    QThread *thread = new QThread();
    theRemoteService->moveToThread(thread);

    QWidget::connect(thread, SIGNAL(finished()), theRemoteService, SLOT(deleteLater()));
    QWidget::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();

    //
    // show the main window, set styles & start the event loop
    //

    w.show();
    w.statusBar()->showMessage("Ready", 5000);

//    pqApplicationCore appCore(argc, argv);





#ifdef Q_OS_WIN
    QFile file(":/styleCommon/stylesheetWIN.qss");
#endif

#ifdef Q_OS_MACOS
    QFile file(":/styleCommon/stylesheetMAC.qss");
#endif

#ifdef Q_OS_LINUX
    QFile file(":/styleCommon/stylesheetLinux.qss");
#endif

    if(file.open(QFile::ReadOnly)) {
      mainApp.setStyleSheet(file.readAll());
      file.close();
    } else {
      qDebug() << "could not open stylesheet";
    }

    /* add app-specific style options */

#ifdef Q_OS_WIN
    QFile WEUQfile(":/styleSheets/WEUQWin.qss");
#endif

#ifdef Q_OS_MACOS
    QFile WEUQfile(":/styleSheets/WEUQMac.qss");
#endif

#ifdef Q_OS_LINUX
    QFile WEUQfile(":/styleSheets/WEUQLinux.qss");
#endif

    if(WEUQfile.open(QFile::ReadOnly)) {
      mainApp.setStyleSheet(mainApp.styleSheet().append(WEUQfile.readAll()));
      WEUQfile.close();
    } else {
      qDebug() << "could not open stylesheet";
    }

    //Setting Google Analytics Tracking Information
    
#ifdef _SC_RELEASE

    qDebug() << "Running a Release Version of WE-UQ";
    GoogleAnalytics::SetMeasurementId("G-SQHRGYDZ0H");
    GoogleAnalytics::SetAPISecret("SCg4ry-WRee780Oen2WBUA");
    GoogleAnalytics::CreateSessionId();
    GoogleAnalytics::StartSession();

    // Opening a QWebEngineView and using github to get app geographic usage
    QWebEngineView view;
    view.setUrl(QUrl("https://nheri-simcenter.github.io/WE-UQ/GA4.html"));
    view.resize(1024, 750);
    view.show();
    view.hide();
    
#endif

#ifdef _ANALYTICS

    qDebug() << "compiled with _ANALYTICS";    
    GoogleAnalytics::SetMeasurementId("G-SQHRGYDZ0H");
    GoogleAnalytics::SetAPISecret("SCg4ry-WRee780Oen2WBUA");
    GoogleAnalytics::CreateSessionId();
    GoogleAnalytics::StartSession();
    
#endif    
    
    //
    // RUN the GUI
    //
    
    int res = mainApp.exec();


#ifdef _GA_AFTER

    qDebug() << "compiled with: _GA_AFTER";
    // Opening a QWebEngineView and using github to get app geographic usage
    QWebEngineView view;
    view.setUrl(QUrl("https://nheri-simcenter.github.io/WE-UQ/GA4.html"));
    view.resize(1024, 750);
    view.show();
    view.hide();
    
#endif
    
    //
    // on done with event loop, logout & stop the thread
    //

    theRemoteService->logout();
    thread->quit();

    GoogleAnalytics::EndSession();

    // done
    return res;
}
