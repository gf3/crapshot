#ifndef CRAPSHOT_H
#define CRAPSHOT_H

#include <iostream>

#include <QApplication>
#include <QtWebKit>
#include <QtWebKitWidgets>

class CrapShot : public QObject {
  Q_OBJECT

public:
  CrapShot();
  void load(const QUrl &url, const QString &filename);

signals:
  void finished(int code);

private slots:
  void initialLayoutCompleted();
  void loadFinished(bool ok);
  void delayFinished();

private:
  QWebPage mPage;
  QString mFilename;
  float mDelay;
  bool mSawInitialLayout;
  bool mSawLoadFinished;

  void delayedRender();
  void render();
};

#endif // CRAPSHOT_H
