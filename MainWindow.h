#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QString>
#include <QSplitter>
#include <QVBoxLayout>
#include "OglArea.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
signals:
    void compileSourceSignal(QString const& src);
public slots:
    void on_SourceCompiled();
    void on_SourceFailedToCompile(QString const& error_str);
    void on_UserCodeTextChanged();
private slots:
    void on_SaveClicked();
    void on_SaveAsClicked();
    void on_OpenClicked();
    void on_ResetClicked();
private:

    bool writeFile(QString const& path, QString const& text);
    bool readFile(QString const& path, QString &text);

    OglArea* oglWgt;
    QTextEdit* userCode;
    QTextEdit* errorScreen;

    QSplitter* mainSplitter;
    QVBoxLayout* mainLayout;
    QWidget* panelWidget;

    QString pathToFile = "";

    // These two methods are operating with settings
    QString getLastSavePath();
    void setLastSavePath(QString const& path);
};
