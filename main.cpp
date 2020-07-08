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
        ts << txt << endl;
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
    QCoreApplication::setApplicationVersion("2.1.0");
    //    GoogleAnalytics::SetTrackingId("UA-121615795-1");
    GoogleAnalytics::StartSession();
    GoogleAnalytics::ReportStart();

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
    QFile debugFile(logFilePath);
    debugFile.remove();
  QApplication a(argc, argv);
  Q_INIT_RESOURCE(images1);

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

    QFile aboutTXT(":/Resources/docs/textAboutWEUQ.html");
    aboutTXT.open(QIODevice::ReadOnly);
    QString textAboutWE = aboutTXT.readAll();
    aboutTXT.close();
    w.setAbout(textAboutWE);

    QString version("Version 2.1.0");
    w.setVersion(version);

    QString citeText("Frank McKenna, Peter Mackenzie-Helnwein, Wael Elhaddad, Michael Gardner, Jiawei Wan, & Dae Kun Kwon. (2019, September 30). NHERI-SimCenter/WE-UQ: Version 2.0.0 (Version v2.0.0). Zenodo. http://doi.org/10.5281/zenodo.3464692");

    w.setCite(citeText);

    QString manualURL("https://www.designsafe-ci.org/data/browser/public/designsafe.storage.community//SimCenter/Software/WE_UQ/");
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
