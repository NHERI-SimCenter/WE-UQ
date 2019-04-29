// Written: fmckenna

// Purpose: the typical Qt main for running a QMainWindow

#include <MainWindowWorkflowApp.h>
#include <QApplication>
#include <QFile>
#include <QThread>
#include <QObject>

#include <AgaveCurl.h>
#include <WorkflowAppCWE.h>

#include <QTime>
#include <QTextStream>
#include <GoogleAnalytics.h>

 // customMessgaeOutput code from web:
 // https://stackoverflow.com/questions/4954140/how-to-redirect-qdebug-qwarning-qcritical-etc-output

const QString logFilePath = "debug.log";
bool logToFile = false;

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
  //Setting Core Application Name, Organization, Version and Google Analytics Tracking Id
  //

  QCoreApplication::setApplicationName("CWE-UQ");
  QCoreApplication::setOrganizationName("SimCenter");
  QCoreApplication::setApplicationVersion("0.1.0");
  // turn off  GoogleAnalytics::SetTrackingId("UA-121615795-1");
  GoogleAnalytics::StartSession();
  GoogleAnalytics::ReportStart();

  //
  // set up logging of output messages for user debugging
  //

  // remove old log file
  QFile debugFile("debug.log");
  debugFile.remove();

  QByteArray envVar = qgetenv("QTDIR");       //  check if the app is run in Qt Creator
  
  if (envVar.isEmpty())
    logToFile = true;
  
  qInstallMessageHandler(customMessageOutput);

  // window scaling for mac
  //QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QApplication a(argc, argv);

  //
  // create a remote interface
  //

  QString tenant("designsafe");
  QString storage("agave://designsafe.storage.default/");
  QString dirName("CWE-UQ");

  AgaveCurl *theRemoteService = new AgaveCurl(tenant, storage, &dirName);


  //
  // create the main window
  //
  WorkflowAppWidget *theInputApp = new WorkflowAppCWE(theRemoteService);
  MainWindowWorkflowApp w(QString("CWE-UQ:  Computational Wind Engineering Application"),theInputApp, theRemoteService);

  QString textAboutCWE = "\
          <p> \
          This is the Performance Based Engineering (CWE) Application.\
          <p> \
          The CWE Application (CWE App) is an open-source software \
          (https://github.com/NHERI-SimCenterCWE) that provides researchers a tool to \
          assess the performance of a building to wind loading. The application \
          focuses on quantifying building performance through decision variables. Given \
          that the properties of the buildings and the wind events are not known \
          exactly, and that the simulation software and the user make simplifying \
          assumptions in the numerical modeling of the structure, the estimate response of\
           the structure already exhibits significant variability. Such response can be \
           estimated using our CWE-UQ Application (https://simcenter.designsafe-ci.org/\
              research-tools/cwe-uq-application/).\
          <p> \
          Depending on the type of structural system, the fidelity of the numerical model \
          and the number of EDP samples required, the response history simulations can be \
          computationally prohibitively expensive. To overcome this impediment, the user \
          has the option to perform the response simulations on the Stampede2 \
          supercomputer. Stampede2 is located at the Texas Advanced Computing Center and \
          made available to the user through NHERI DesignSafe-CI, the cyberinfrastructure \
          provider for the distributed NSF funded Natural Hazards in Engineering Research \
          Infrastructure (NHERI) facility.\
          <p> \
          The computations are performed in a workflow application. That is, the numerical\
           simulations are actually performed by a sequence of different applications. The\
            CWE-UQ backend software runs these various applications for the user, taking the \
            outputs from some programs and providing them as inputs to others. The design \
            of the CWE-UQ App is such that researchers are able to modify the backend \
            application to utilize their own application in the workflow computations. \
            This will ensure researchers are not limited to using the default applications\
             we provide and will be enthused to provide their own applications for others \
             to use. \
          <p>\
          This is Version 0.1.0 of the tool and as such is limited in scope. Researchers are\
           encouraged to comment on what additional features and applications they would \
           like to see in the CWE-UQ App. If you want a feature, chances are many of your \
           colleagues would also benefit from it.\
          <p>";

     w.setAbout(textAboutCWE);

  QString version("0.1.0");
  w.setVersion(version);

  //
  // move remote interface to a thread
  //

  QThread *thread = new QThread();
  theRemoteService->moveToThread(thread);

  QWidget::connect(thread, SIGNAL(finished()), theRemoteService, SLOT(deleteLater()));
  QWidget::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

  thread->start();

  //
  // show the main window & start the event loop
  //

  w.show();

  QFile file(":/styleCommon/style.qss");
  if(file.open(QFile::ReadOnly)) {
    QString styleSheet = QLatin1String(file.readAll());
    a.setStyleSheet(styleSheet);
  }


  theInputApp->setStyleSheet("QComboBox {background: #FFFFFF;} \
QGroupBox {font-weight: bold;}\
QLineEdit {background-color: #FFFFFF; border: 2px solid darkgray;} \
QTabWidget::pane {background-color: #ECECEC; border: 1px solid rgb(244, 244, 244);}");

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
