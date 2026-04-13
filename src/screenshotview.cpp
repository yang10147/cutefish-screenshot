/*
 * Copyright (C) 2021 CutefishOS Team.
 *
 * Author:     Reion Wong <reionwong@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "screenshotview.h"

#include <QClipboard>
#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QUrl>
#include <QImage>
#include <QProcess>
#include <QBuffer>

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

ScreenshotView::ScreenshotView(QQuickView *parent)
    : QQuickView(parent)
{
    rootContext()->setContextProperty("view", this);

    setFlags(Qt::FramelessWindowHint);
    setScreen(qGuiApp->primaryScreen());
    setResizeMode(QQuickView::SizeRootObjectToView);
    setSource(QUrl("qrc:/qml/main.qml"));
    setGeometry(screen()->geometry());
}

void ScreenshotView::start()
{
    grabViaPortal();
}

void ScreenshotView::grabViaPortal()
{
    QDBusInterface portal("org.freedesktop.portal.Desktop",
                          "/org/freedesktop/portal/desktop",
                          "org.freedesktop.portal.Screenshot",
                          QDBusConnection::sessionBus());

    if (!portal.isValid()) {
        qWarning() << "org.freedesktop.portal.Screenshot not available";
        quit();
        return;
    }

    QVariantMap options;
    options["interactive"] = false;
    options["modal"] = false;

    QDBusPendingCall call = portal.asyncCall("Screenshot", QString(""), options);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, this,
            [this](QDBusPendingCallWatcher *w) {
        w->deleteLater();

        QDBusPendingReply<QDBusObjectPath> reply = *w;
        if (reply.isError()) {
            qWarning() << "Screenshot portal call error:" << reply.error().message();
            quit();
            return;
        }

        QString requestPath = reply.value().path();

        QDBusConnection::sessionBus().connect(
            "org.freedesktop.portal.Desktop",
            requestPath,
            "org.freedesktop.portal.Request",
            "Response",
            this,
            SLOT(onPortalResponse(uint,QVariantMap))
        );
    });
}

void ScreenshotView::onPortalResponse(uint response, const QVariantMap &results)
{
    if (response != 0) {
        quit();
        return;
    }

    QString uri = results.value("uri").toString();
    if (uri.isEmpty()) {
        qWarning() << "Portal returned empty URI";
        quit();
        return;
    }

    QString localPath = QUrl(uri).toLocalFile();

    QFile::remove("/tmp/cutefish-screenshot.png");
    if (localPath != "/tmp/cutefish-screenshot.png") {
        QFile::copy(localPath, "/tmp/cutefish-screenshot.png");
        QFile::remove(localPath);
    }

    setVisible(true);
    setKeyboardGrabEnabled(true);
    emit refresh();
}

void ScreenshotView::delay(int value)
{
    QEventLoop waitLoop;
    QTimer::singleShot(value * 1000, &waitLoop, &QEventLoop::quit);
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

    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QString("%1/Screenshot_%2.png")
                              .arg(desktopPath)
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
            args << "cutefish-screenshot";
            args << ((unsigned int) 0);
            args << "cutefish-screenshot";
            args << "";
            args << tr("The picture has been saved to %1").arg(fileName);
            args << QStringList();
            args << QVariantMap();
            args << (int) 10;
            iface.asyncCallWithArgumentList("Notify", args);
        }
    }

    removeTmpFile();
    this->quit();
}

void ScreenshotView::copyToClipboard(QRect rect)
{
    setVisible(false);

    // 裁剪图片保存到临时文件
    QImage image("/tmp/cutefish-screenshot.png");
    QImage cropped = image.copy(rect);
    QString tmpPath = "/tmp/cutefish-screenshot-clip.png";
    cropped.save(tmpPath);

    // 用 wl-copy 写入 Wayland 剪贴板
    // wl-copy 会保持后台运行直到剪贴板被其他程序覆盖，不会因为本进程退出而丢失
    QProcess *proc = new QProcess();
    proc->setStandardInputFile(tmpPath);
    proc->start("wl-copy", {"--type", "image/png"});

    // 不等待 wl-copy 退出，让它在后台保持剪贴板内容
    // 连接 finished 信号只是为了清理 QProcess 对象
    QObject::connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     proc, &QProcess::deleteLater);

    // 发送通知
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QList<QVariant> args;
        args << "cutefish-screenshot";
        args << ((unsigned int) 0);
        args << "cutefish-screenshot";
        args << "";
        args << tr("The picture has been saved to the clipboard");
        args << QStringList();
        args << QVariantMap();
        args << (int) 10;
        iface.asyncCallWithArgumentList("Notify", args);
    }

    removeTmpFile();

    // 稍等 wl-copy 启动完成再退出
    QTimer::singleShot(500, qGuiApp, &QGuiApplication::quit);
}

void ScreenshotView::removeTmpFile()
{
    QFile("/tmp/cutefish-screenshot.png").remove();
}
