#include "MainWindow.h"
#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <Shaders.h>
#include <QToolBar>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QSettings>
#include <QFileInfo>

/// __Layout__
///
/// MainWindow---->mainLayout---->toolBar
///                     |
///                     |
///                     v
///               mainSplitter---->oglWgt||errorScreen
///                           |
///                           ---->panelWidget---->panelLayout---->inputVars
///                                                           |
///                                                           ---->userCode
///                                                           |
///                                                           ---->compileBtn

const QString fileFormats{"GLSL Shaders(*.sh *fsh *vsh *.vert *tesc *tese *geom *frag *comp);;"
                          "Text Files (*.txt)"};
const QString programName{"Shader Tool"};

MainWindow::MainWindow(QWidget* parent) :
    QWidget(parent)
{
    this->setWindowTitle(programName);
    /*Create*/
    userCode = new QTextEdit();
    errorScreen = new QTextEdit();
    oglWgt = new OglArea();
    mainSplitter = new QSplitter();
    mainLayout = new QVBoxLayout();
    panelWidget = new QWidget();

    QTextEdit* inputVars = new QTextEdit();
    QPushButton* compileBtn = new QPushButton();
    QVBoxLayout* panelLayout = new QVBoxLayout();
    QToolBar* toolBar = new QToolBar();
    QAction* saveAction = new QAction("Save");
    QAction* saveAsAction = new QAction("Save As");
    QAction* openAction = new QAction("Open");
    QAction* resetAction = new QAction("Reset");
    /*Setup*/    
    inputVars->setReadOnly(true);
    errorScreen->setReadOnly(true);
    inputVars->setText(SHADER_INPUT);
    userCode->setText(MAIN_IMAGE_BASIC_DEF);

    inputVars->setFont(QFont("Consolas", 9));
    userCode->setFont(QFont("Consolas", 14));


    compileBtn->setText("Compile");
    mainSplitter->setOrientation(Qt::Horizontal);

    saveAction->setIcon(QIcon(":/icn/icons/icon_save.png"));
    saveAsAction->setIcon(QIcon(":/icn/icons/icon_save_as.png"));
    openAction->setIcon(QIcon(":/icn/icons/icon_open.png"));
    resetAction->setIcon(QIcon(":/icn/icons/icon_reset.png"));


    /*Connect*/
    connect(this, &MainWindow::compileSourceSignal, oglWgt, &OglArea::compileSource);
    connect(oglWgt, &OglArea::sourceCompiled, this, &MainWindow::on_SourceCompiled);
    connect(oglWgt, &OglArea::sourceFailedToCompile, this, &MainWindow::on_SourceFailedToCompile);

    connect(userCode, &QTextEdit::textChanged, this, &MainWindow::on_UserCodeTextChanged);
    connect(compileBtn, &QPushButton::clicked,
    [&]()
    {
        emit compileSourceSignal(userCode->toPlainText());
    });

    connect(saveAction, &QAction::triggered, this, &MainWindow::on_SaveClicked);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::on_SaveAsClicked);
    connect(openAction, &QAction::triggered, this, &MainWindow::on_OpenClicked);
    connect(resetAction, &QAction::triggered, this, &MainWindow::on_ResetClicked);

    /*Place*/
    panelLayout->addWidget(inputVars);
    panelLayout->addWidget(userCode);
    panelLayout->addWidget(compileBtn);
    panelLayout->setStretch(0, 3);
    panelLayout->setStretch(1, 10);

    panelWidget->setLayout(panelLayout);
    mainSplitter->addWidget(errorScreen);
    mainSplitter->addWidget(oglWgt);
    mainSplitter->addWidget(panelWidget);

    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(saveAsAction);
    toolBar->addAction(resetAction);

    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(mainSplitter);
    this->setLayout(mainLayout);

    mainSplitter->setSizes({700, 700, 300});

    errorScreen->hide();
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_SourceCompiled()
{
    if(oglWgt->isHidden())
    {
        auto sizes = mainSplitter->sizes();
        std::swap(sizes[0], sizes[1]);
        errorScreen->hide();
        oglWgt->show();
        mainSplitter->setSizes(sizes);
    }
}

void MainWindow::on_SourceFailedToCompile(QString const& error_str)
{
    if(errorScreen->isHidden())
    {
        auto sizes = mainSplitter->sizes();
        std::swap(sizes[0], sizes[1]);
        oglWgt->hide();
        errorScreen->setText(error_str);
        errorScreen->show();
        mainSplitter->setSizes(sizes);
    }
}

void MainWindow::on_UserCodeTextChanged()
{

}

void MainWindow::on_SaveClicked()
{
    // If for some reason there is no this file anymore, or it is first save button click
    if(!QFile::exists(pathToFile))
    {
        pathToFile =
                QFileDialog::getSaveFileName(this,
                                             tr("Open File"),
                                             getLastSavePath(),
                                             fileFormats);
        // user have not selected any file
        if(pathToFile.isEmpty())
        {
            setWindowTitle(programName);
            return;
        }
        setWindowTitle(pathToFile);
        setLastSavePath(pathToFile);
    }

    writeFile(pathToFile, userCode->toPlainText());
}

void MainWindow::on_SaveAsClicked()
{
    QString path =
            QFileDialog::getSaveFileName(this,
                                         tr("Save File"),
                                         getLastSavePath(),
                                         fileFormats);

    // user have not selected any file
    if(path.isEmpty())
    {
        return;
    }
    if(writeFile(path, userCode->toPlainText()))
    {
        pathToFile = path;
        setWindowTitle(pathToFile);
        setLastSavePath(pathToFile);
    }
}

void MainWindow::on_OpenClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), getLastSavePath(), fileFormats);
    // user have not selected any file
    if(path.isEmpty()) return;
    QString code = "";
    if(readFile(path, code))
    {
        pathToFile = path;
        userCode->setText(code);
        setWindowTitle(pathToFile);
        setLastSavePath(pathToFile);
    }
}

void MainWindow::on_ResetClicked()
{
    pathToFile = "";
    setWindowTitle(programName);
}

bool MainWindow::writeFile(QString const& path, QString const& text)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Warning", "Cant open file \"" + path + "\"");
        return false;
    }

    QTextStream out(&file);
    out << text;

    return true;
}

bool MainWindow::readFile(QString const& path, QString &text)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Warning", "Cant open file \"" + path + "\"");
        return false;
    }

    QTextStream out(&file);
    text = out.readAll();
    return true;
}

QString MainWindow::getLastSavePath()
{
    QSettings settings;
    QVariant val = settings.value("paths/last_path");
    QString res = ".";
    if(val.isValid())
    {
        res = val.toString();
    }
    return res;
}
void MainWindow::setLastSavePath(QString const& path)
{
    QSettings settings;
    settings.setValue("paths/last_path", QFileInfo(path).absolutePath());
}
