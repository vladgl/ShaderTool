#include <QApplication>

#include <QString>
#include <QTextStream>
#include <QStyleFactory>
#include <QSurfaceFormat>
#include <QFile>
#include "MainWindow.h"


QString loadTxtFile(const QString& path);


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Shader Tool");
    a.setOrganizationName("no_org");

    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    QColor windowColor = QColor(35, 35, 35);
    QColor baseColor = QColor(40, 40, 40);
    QColor alternateBaseColor = QColor(45, 45, 45);
    QColor disabledColor = QColor(127, 127, 127);
    QColor buttonColor = QColor(30, 30, 30);



    darkPalette.setColor(QPalette::Window, windowColor);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, baseColor);
    darkPalette.setColor(QPalette::AlternateBase, alternateBaseColor);
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::black);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    darkPalette.setColor(QPalette::Button, buttonColor);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);
    qApp->setPalette(darkPalette);
    qApp->setStyleSheet(loadTxtFile(":/styles/dark.css"));


    MainWindow w;
    w.showMaximized();

    int res = a.exec();
    return res;
}

QString loadTxtFile(const QString& path)
{
    QFile file(path);
    file.open(QFile::ReadOnly);
    QTextStream tin(&file);
    QString result;

    while(!tin.atEnd())
    {
        result += tin.readLine();
    }
    return result;
}

