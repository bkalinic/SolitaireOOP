#include "src/guiHeader/MainWindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDir>

int main(int argc, char* argv[]) {
    qDebug() << "Pokrecem Solitaire GUI...";

    qDebug() << "=== RESOURCE DEBUG ===";

    qDebug() << "Registered resources:";
    QStringList resourceFiles = QDir(":/").entryList();
    for (const QString& file : resourceFiles) {
        qDebug() << "  :/" << file;
    }

    QString qrcPath = "resources/cards.qrc";
    if (QFile::exists(qrcPath)) {
        qDebug() << "cards.qrc postoji na:" << qrcPath;

        QFile file(qrcPath);
        if (file.open(QIODevice::ReadOnly)) {
            qDebug() << "Sadrzaj cards.qrc:";
            qDebug() << file.readAll().left(500);
            file.close();
        }
    }
    else {
        qDebug() << "cards.qrc NE POSTOJI na:" << qrcPath;
    }

    QApplication app(argc, argv);

    app.setApplicationName("Solitaire OOP");
    app.setOrganizationName("SOSS");
    app.setWindowIcon(QIcon(":/cards/back.png"));

    MainWindow window;
    window.show();

    qDebug() << "Prikazujem prozor...";

    return app.exec();
}