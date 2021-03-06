#include "crapshot.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  if (argc != 3) {
    std::cerr << "Capture a web page and save it to a PDF" << std::endl << std::endl;
    std::cerr << "  " << argv[0] << " <url> <outputfile>" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Notes:" << std::endl;
    std::cerr << "  'url' is the URL of the web page to be captured" << std::endl;
    std::cerr << "  'outputfile' is the name of the file to be generated" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Example: " << std::endl;
    std::cerr << "  " << argv[0] << " universe.com lol.pdf" << std::endl;
    std::cerr << std::endl;

    return 1;
  }

  QApplication a(argc, argv, true);

  QString source = QString::fromUtf8(argv[1]);
  QString filename = QString::fromUtf8(argv[2]);

  CrapShot shot;
  QObject::connect(&shot, &CrapShot::finished, QApplication::instance(), &QApplication::quit);

  if (source == "-") {
    QTextStream in(stdin);
    QString html = in.readAll();
    shot.loadHTML(html, filename);
  }
  else {
    QUrl url = QUrl::fromUserInput(source);
    shot.loadURL(url, filename);
  }

  return a.exec();
}
