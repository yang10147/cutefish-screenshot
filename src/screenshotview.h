#ifndef SCREENSHOTVIEW_H
#define SCREENSHOTVIEW_H

#include <QQuickView>
#include <QVariantMap>
#include <QThread>
#include <unistd.h>

// 在独立线程里阻塞读 pipe，避免主线程卡死
class PipeReader : public QThread
{
    Q_OBJECT
public:
    explicit PipeReader(int fd, QObject *parent = nullptr)
        : QThread(parent), m_fd(fd) {}

    QByteArray data() const { return m_data; }

protected:
    void run() override {
        char buf[65536];
        ssize_t n;
        while ((n = ::read(m_fd, buf, sizeof(buf))) > 0)
            m_data.append(buf, n);
        ::close(m_fd);
    }

private:
    int m_fd;
    QByteArray m_data;
};

class ScreenshotView : public QQuickView
{
    Q_OBJECT

public:
    explicit ScreenshotView(QQuickView *parent = nullptr);

    void start();
    void delay(int value);

    Q_INVOKABLE void quit();
    Q_INVOKABLE void saveFile(QRect rect);
    Q_INVOKABLE void copyToClipboard(QRect rect);

    void removeTmpFile();

private:
    void grabViaKWin();

signals:
    void refresh();
};

#endif // SCREENSHOTVIEW_H
