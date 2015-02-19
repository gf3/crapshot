#include "crapshot.h"

#include <QPrinter>
#include <QTimer>


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

CrapShot::CrapShot(): QObject(), mDelay(5000), mSawInitialLayout(false), mSawLoadFinished(false) {
  connect(&mPage, &QWebPage::loadFinished, this, &CrapShot::loadFinished);
  connect(mPage.mainFrame(), &QWebFrame::initialLayoutCompleted, this, &CrapShot::initialLayoutCompleted);
}


//-----------------------------------------------------------------------------
// Private
//-----------------------------------------------------------------------------

QString readFile(const QString& filename) {
  QFile file(filename);

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    return stream.readAll();
  }

  return "";
}


//-----------------------------------------------------------------------------
// Slots
//-----------------------------------------------------------------------------

void CrapShot::delayFinished() {
  render();
}

void CrapShot::initialLayoutCompleted() {
  mSawInitialLayout = true;

  if (mSawInitialLayout && mSawLoadFinished) {
    delayedRender();
  }
}

void CrapShot::loadFinished(bool ok) {
  if (!ok) {
    std::cerr << "Failed loading " << qPrintable(mPage.mainFrame()->url().toString()) << std::endl;
    emit finished(1);
    return;
  }

  mSawLoadFinished = true;

  if (mSawInitialLayout && mSawLoadFinished) {
    delayedRender();
  }
}


//-----------------------------------------------------------------------------
// Instance Methods
//-----------------------------------------------------------------------------

void CrapShot::delayedRender() {
  mPage.mainFrame()->evaluateJavaScript(readFile(":/js/nuke.js"));

  QTimer::singleShot(mDelay, this, SLOT(delayFinished()));

  return;
}

void CrapShot::loadHTML(const QString &html, const QString &filename) {
  mFilename = filename;
  setupPage();
  mPage.mainFrame()->setHtml(html);
}

void CrapShot::loadURL(const QUrl &url, const QString &filename) {
  mFilename = filename;
  setupPage();
  mPage.mainFrame()->load(url);
}

void CrapShot::render() {
  mPage.setViewportSize(mPage.mainFrame()->contentsSize());

  QPrinter printer(QPrinter::HighResolution);
  printer.setFullPage(true);
  printer.setOutputFileName(mFilename);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setPageSize(QPrinter::A4);

  mPage.mainFrame()->print(&printer);
  emit finished(0);
}

void CrapShot::setupPage() {
  mPage.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
  mPage.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  mPage.settings()->setAttribute(QWebSettings::AutoLoadImages, true);
  mPage.settings()->setAttribute(QWebSettings::FrameFlatteningEnabled, true);
  mPage.settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
  mPage.settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
  mPage.settings()->setAttribute(QWebSettings::PrintElementBackgrounds, true);
  mPage.setViewportSize(QSize(750, 1334));
}
