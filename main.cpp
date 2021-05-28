#include "Settings.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    qSetMessagePattern("[%{time yyyy-MM-dd HH:mm:ss.zzz}][%{type}][%{function} %{line}]%{message}");
    QApplication a(argc, argv);

#if 0
    QDir dir(QString("%1/fonts").arg(a.applicationDirPath()));
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);
    for (int i = 0; i < fileInfoList.size(); ++i) {
        QFileInfo info = fileInfoList.at(i);
        int fontid = QFontDatabase::addApplicationFont(info.absoluteFilePath());
        if (fontid < 0) {
            continue;
        }
        QStringList families = QFontDatabase::applicationFontFamilies(fontid);
        if (families.isEmpty()) {
            continue;
        }
        QFont font(families.first(), 10);
        a.setFont(font);
        qDebug() << "Set font:" << font;
        break;
    }
#endif

    a.setFont(gSettings.appFont());

    MainWindow w;
    w.show();
    return a.exec();
}
