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
    setVisible(false);  // 初始隐藏，截图完成后再显示，避免蒙版被截入
}

void ScreenshotView::start()
{
    // 延迟 200ms 再截图，确保窗口（蒙版）已完全隐藏后才触发 KWin 截图
    QTimer::singleShot(50, this, &ScreenshotView::grabViaKWin);
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

    QDBusInterface kwin("org.kde.KWin",
                        "/org/kde/KWin/ScreenShot2",
                        "org.kde.KWin.ScreenShot2",
                        QDBusConnection::sessionBus());

    if (!kwin.isValid()) {
        qWarning() << "KWin.ScreenShot2 not available:" << kwin.lastError().message();
        ::close(readFd);
        ::close(writeFd);
        quit();
        return;
    }

    PipeReader *reader = new PipeReader(readFd, this);
    reader->start();

    QVariantMap options;
    QDBusUnixFileDescriptor dbusWriteFd(writeFd);

    // kind=2: 框选区域截图（交互式）
    // CaptureInteractive(in u kind, in a{sv} options, in h pipe, out a{sv} results)
    // CaptureActiveScreen: 截全屏，QML层负责框选裁剪
    // 签名: CaptureActiveScreen(in a{sv} options, in h pipe, out a{sv} results)
    QDBusPendingCall call = kwin.asyncCall(
        "CaptureActiveScreen",
        QVariant::fromValue(options),
        QVariant::fromValue(dbusWriteFd)
    );

    ::close(writeFd);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this,
            [this, reader](QDBusPendingCallWatcher *w) {
        w->deleteLater();

        QDBusPendingReply<QVariantMap> reply = *w;
        if (reply.isError()) {
            qWarning() << "CaptureInteractive error:" << reply.error().message();
            reader->wait(3000);
            reader->deleteLater();
            quit();
            return;
        }

        qDebug() << "KWin reply ok, waiting for pipe data...";
        reader->wait(10000);
        QByteArray pngData = reader->data();
        reader->deleteLater();

        qDebug() << "Pipe data size:" << pngData.size();

        if (pngData.isEmpty()) {
            qWarning() << "Empty data, user cancelled or error";
            quit();
            return;
        }

        // KWin 返回的是原始 BGRA 像素数据，需要转换成 PNG
        // 根据数据大小推算分辨率（屏幕分辨率 * devicePixelRatio）
        QScreen *screen = qGuiApp->primaryScreen();
        QSize screenSize = screen->geometry().size() * screen->devicePixelRatio();
        int imgW = screenSize.width();
        int h = screenSize.height();
        int expectedSize = imgW * h * 4;

        qDebug() << "Screen size:" << imgW << "x" << h
                 << "expected bytes:" << expectedSize
                 << "actual bytes:" << pngData.size();

        // 如果尺寸不匹配，尝试从数据大小反推
        if (pngData.size() != expectedSize) {
            // 尝试用原始分辨率（不乘 devicePixelRatio）
            QSize rawSize = screen->geometry().size();
            if (pngData.size() == rawSize.width() * rawSize.height() * 4) {
                imgW = rawSize.width();
                h = rawSize.height();
                qDebug() << "Using raw size:" << w << "x" << h;
            } else {
                // 强行推算高度
                h = pngData.size() / (imgW * 4);
                qDebug() << "Calculated h:" << h;
            }
        }

        // BGRA -> ARGB32 转换
        QImage image(reinterpret_cast<const uchar*>(pngData.constData()),
                     imgW, h, imgW * 4, QImage::Format_ARGB32);
        // KWin 返回的是 BGRA，Qt 的 Format_ARGB32 在小端机器上就是 BGRA 内存布局
        // 所以直接用 Format_ARGB32 即可，不需要额外转换

        QFile::remove("/tmp/cutefish-screenshot.png");
        bool saved = image.save("/tmp/cutefish-screenshot.png", "PNG");
        qDebug() << "Saved PNG:" << saved << "size:" << image.size();

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
