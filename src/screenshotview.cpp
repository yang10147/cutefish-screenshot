#include "screenshotview.h"

#include <QClipboard>
#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QImage>
#include <QProcess>

#include <QGuiApplication>
#include <QQmlContext>
#include <QScreen>
#include <QPixmap>
#include <QStandardPaths>
#include <QDateTime>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusUnixFileDescriptor>

#include <unistd.h>
#include <fcntl.h>

ScreenshotView::ScreenshotView(QQuickView *parent)
    : QQuickView(parent)
{
    rootContext()->setContextProperty("view", this);
    setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setScreen(qGuiApp->primaryScreen());
    setResizeMode(QQuickView::SizeRootObjectToView);
    setSource(QUrl("qrc:/qml/main.qml"));
    setGeometry(screen()->geometry());
}

void ScreenshotView::start()
{
    grabViaKWin();
}

void ScreenshotView::grabViaKWin()
{
    int pipeFds[2];
    if (pipe2(pipeFds, O_CLOEXEC) != 0) {
        qWarning() << "Failed to create pipe";
        quit();
        return;
    }

    int readFd  = pipeFds[0];
    int writeFd = pipeFds[1];

    QDBusInterface kwin("org.kde.KWin.ScreenShot2",
                        "/org/kde/KWin/ScreenShot2",
                        "org.kde.KWin.ScreenShot2",
                        QDBusConnection::sessionBus());

    if (!kwin.isValid()) {
        qWarning() << "KWin.ScreenShot2 not available";
        ::close(readFd);
        ::close(writeFd);
        quit();
        return;
    }

    QVariantMap options;
    options["native-resolution"] = true;

    // kind=0: 交互式（点击选窗口）
    uint kind = 0;

    QDBusUnixFileDescriptor dbusWriteFd(writeFd);

    // 启动读线程（阻塞读，等 KWin 写完关闭写端）
    PipeReader *reader = new PipeReader(readFd, this);

    QDBusPendingCall call = kwin.asyncCall(
        "CaptureInteractive",
        kind,
        options,
        QVariant::fromValue(dbusWriteFd)
    );

    // 我们这边关掉写端（KWin 持有写端的副本，会在截图完成后关闭）
    ::close(writeFd);

    // 启动读线程
    reader->start();

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, this,
            [this, reader](QDBusPendingCallWatcher *w) {
        w->deleteLater();

        QDBusPendingReply<QVariantMap> reply = *w;
        if (reply.isError()) {
            qWarning() << "KWin ScreenShot2 error:" << reply.error().message();
            reader->wait();
            reader->deleteLater();
            quit();
            return;
        }

        // 等读线程读完所有数据
        reader->wait();
        QByteArray pngData = reader->data();
        reader->deleteLater();

        if (pngData.isEmpty()) {
            qWarning() << "KWin returned empty screenshot data";
            quit();
            return;
        }

        QFile::remove("/tmp/cutefish-screenshot.png");
        QFile tmp("/tmp/cutefish-screenshot.png");
        if (tmp.open(QIODevice::WriteOnly)) {
            tmp.write(pngData);
            tmp.close();
        }

        setVisible(true);
        setKeyboardGrabEnabled(true);
        emit refresh();
    });
}

void ScreenshotView::delay(int value)
{
    QEventLoop waitLoop;
    QTimer::singleShot(value, &waitLoop, &QEventLoop::quit);
    waitLoop.exec();
    start();
}

void ScreenshotView::quit()
{
    qGuiApp->quit();
}

void ScreenshotView::saveFile(QRect rect)
{
    setVisible(false);

    QString picPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString saveDir = picPath + "/屏幕截图";
    QDir().mkpath(saveDir);

    QString fileName = QString("%1/屏幕截图_%2.png")
                              .arg(saveDir)
                              .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    QImage image("/tmp/cutefish-screenshot.png");
    QImage cropped = image.copy(rect);
    bool saved = cropped.save(fileName);

    if (saved) {
        QDBusInterface iface("org.freedesktop.Notifications",
                             "/org/freedesktop/Notifications",
                             "org.freedesktop.Notifications",
                             QDBusConnection::sessionBus());
        if (iface.isValid()) {
            QList<QVariant> args;
            args << "cutefish-screenshot" << (uint)0 << "cutefish-screenshot"
                 << "" << tr("The picture has been saved to %1").arg(fileName)
                 << QStringList() << QVariantMap() << (int)10;
            iface.asyncCallWithArgumentList("Notify", args);
        }
    }

    removeTmpFile();
    this->quit();
}

void ScreenshotView::copyToClipboard(QRect rect)
{
    setVisible(false);

    QImage image("/tmp/cutefish-screenshot.png");
    QImage cropped = image.copy(rect);
    QString tmpPath = "/tmp/cutefish-screenshot-clip.png";
    cropped.save(tmpPath);

    QProcess *proc = new QProcess();
    proc->setStandardInputFile(tmpPath);
    proc->start("wl-copy", {"--type", "image/png"});
    QObject::connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     proc, &QProcess::deleteLater);

    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QList<QVariant> args;
        args << "cutefish-screenshot" << (uint)0 << "cutefish-screenshot"
             << "" << tr("The picture has been saved to the clipboard")
             << QStringList() << QVariantMap() << (int)10;
        iface.asyncCallWithArgumentList("Notify", args);
    }

    removeTmpFile();
    QTimer::singleShot(500, qGuiApp, &QGuiApplication::quit);
}

void ScreenshotView::removeTmpFile()
{
    QFile("/tmp/cutefish-screenshot.png").remove();
}
