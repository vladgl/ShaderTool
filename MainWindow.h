#pragma once
#include <QWidget>
#include <QTextEdit>
#include "OglArea.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
signals:
    void compileSourceSignal(QString const& src);
private:

    QTextEdit userCode;
    OglArea oglWgt;
};
