// Written: fmckenna

// Purpose: the typical Qt main for running a QMainWindow

#include <MainWindowWorkflowApp.h>
#include <QApplication>
#include <QFile>
#include <QThread>
#include <QObject>
#include <QDebug>

#include <AgaveCurl.h>
#include <WorkflowAppWE.h>

#include <QTime>
#include <QTextStream>
#include <GoogleAnalytics.h>
#include <QStandardPaths>
#include <QDir>
#include <SimCenterPreferences.h>
#include <QStatusBar>

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
    QCoreApplication::setApplicationVersion("3.0.a");
    // GoogleAnalytics::SetTrackingId("UA-121615795-1");
    GoogleAnalytics::StartSession();
    GoogleAnalytics::ReportStart();

    Q_INIT_RESOURCE(images1);

    //
    // set up logging of output messages for user debugging
    //

    logFilePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + QDir::separator() + QCoreApplication::applicationName();

    // make sure tool dir exists in Documentss folder
    QDir dirWork(logFilePath);
    if (!dirWork.exists())
        if (!dirWork.mkpath(logFilePath)) {
            qDebug() << QString("Could not create Working Dir: ") << logFilePath;
        }

    // full path to debug.log file
    logFilePath = logFilePath + QDir::separator() + QString("debug.log");


    //
    // window scaling
    //
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    // remove old log file
    // QFile debugFile(logFilePath);
    // debugFile.remove();

    QApplication a(argc, argv);


    QByteArray envVar = qgetenv("QTDIR");       //  check if the app is run in Qt Creator

    if (envVar.isEmpty())
        logToFile = true;

    qInstallMessageHandler(customMessageOutput);

    qDebug() << "logFile: " << logFilePath;



    //
    // create a remote interface
    //

    QString tenant("designsafe");
    QString storage("agave://designsafe.storage.default/");
    QString dirName("WE-UQ");

    AgaveCurl *theRemoteService = new AgaveCurl(tenant, storage, &dirName);


    //
    // create the main window
    //

    WorkflowAppWidget *theInputApp = new WorkflowAppWE(theRemoteService);
    MainWindowWorkflowApp w(QString("WE-UQ: Wind Engineering with Uncertainty Quantification"),theInputApp, theRemoteService);

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

    QString citeText("1) Frank McKenna, Peter Mackenzie-Helnwein, Jiawei Wan, Fei Ding, Wael Elhaddad, & Stevan Gavrilovic. (2023). NHERI-SimCenter/WE-UQ: Version 2.3.0 (v2.3.0). Zenodo. https://doi.org/10.5281/zenodo.7706617 \n\n2) Gregory G. Deierlein, Frank McKenna, Adam Zsarnóczay, Tracy Kijewski-Correa, Ahsan Kareem, Wael Elhaddad, Laura Lowes, Matt J. Schoettler, and Sanjay Govindjee (2020) A Cloud-Enabled Application Framework for Simulating Regional-Scale Impacts of Natural Hazards on the Built Environment. Frontiers in the Built Environment. 6:558706. doi: 10.3389/fbuil.2020.558706");
    
    w.setCite(citeText);

    // QString manualURL("https://www.designsafe-ci.org/data/browser/public/designsafe.storage.community//SimCenter/Software/WE_UQ/");
    QString manualURL("https://nheri-simcenter.github.io/WE-UQ-Documentation/");
    w.setDocumentationURL(manualURL);

    QString messageBoardURL("https://simcenter-messageboard.designsafe-ci.org/smf/index.php?board=5.0");
    w.setFeedbackURL(messageBoardURL);

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

#ifdef Q_OS_WIN
    QFile file(":/styleCommon/stylesheetWIN.qss");
#endif

#ifdef Q_OS_MACOS
    QFile file(":/styleCommon/stylesheetMAC.qss");
#endif

#ifdef Q_OS_LINUX
    QFile file(":/styleCommon/stylesheetMAC.qss");
#endif

    if(file.open(QFile::ReadOnly)) {
      a.setStyleSheet(file.readAll());
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
      a.setStyleSheet(a.styleSheet().append(WEUQfile.readAll()));
      WEUQfile.close();
    } else {
      qDebug() << "could not open stylesheet";
    }



/*
    QFile fileCommon(":/styleCommon/common_experimental.qss");
    QFile fileEEUQ(":/styles/stylesheet_eeuq.qss");

    if(fileCommon.open(QFile::ReadOnly) && fileEEUQ.open(QFile::ReadOnly)) {
      QString styleSheet = QLatin1String(fileCommon.readAll());
      QString styleSheetEEUQ = QLatin1String(fileEEUQ.readAll());

      //      a.setStyleSheet(styleSheet+styleSheetEEUQ);
      a.setStyleSheet(styleSheet);
      fileCommon.close();
      fileEEUQ.close();
    }
*/
    /*
    theInputApp->setStyleSheet("QComboBox {background: #E0E0E0;} \
                               QGroupBox {font-weight: bold;}\
                               QLineEdit {background-color: #E0E0E0; border: 2px solid darkgray;} \
                               QTabWidget::pane {background-color: #ECECEC; border: 1px solid rgb(244, 244, 244);}");
    */

    //a.setStyleSheet(openStyleFiles());

    int res = a.exec();

    //
    // on done with event loop, logout & stop the thread
    //

    theRemoteService->logout();
    thread->quit();

    GoogleAnalytics::EndSession();

    // done
    return res;
}
