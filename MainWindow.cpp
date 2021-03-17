#include "MainWindow.h"
#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include <Shaders.h>

MainWindow::MainWindow(QWidget* parent) :
    QWidget(parent)
{
    this->setWindowTitle("Shader Tool");
    /*Create*/
    QTextEdit* inputVars = new QTextEdit();
    QPushButton* compileBtn = new QPushButton();
    QSplitter* mainSplitter = new QSplitter();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QVBoxLayout* panelLayout = new QVBoxLayout();
    QWidget* panelWidget = new QWidget();
    /*Setup*/    
    inputVars->setReadOnly(true);
    inputVars->setText(SHADER_INPUT);
    userCode.setText(MAIN_IMAGE_BASIC_DEF);

    inputVars->setFont(QFont("Consolas", 14));
    userCode.setFont(QFont("Consolas", 14));


    compileBtn->setText("Compile");
    mainSplitter->setOrientation(Qt::Horizontal);

    /*Connect*/
    connect(this, &MainWindow::compileSourceSignal, &oglWgt, &OglArea::compileSource);

    connect(compileBtn, &QPushButton::clicked,
    [&]()
    {
        emit compileSourceSignal(userCode.toPlainText());
    });
    /*Place*/
    panelLayout->addWidget(inputVars);
    panelLayout->addWidget(&userCode);
    panelLayout->addWidget(compileBtn);
    panelLayout->setStretch(0, 3);
    panelLayout->setStretch(1, 10);

    panelWidget->setLayout(panelLayout);
    mainSplitter->addWidget(&oglWgt);
    mainSplitter->addWidget(panelWidget);
    mainSplitter->setStretchFactor(0, 10);
    mainSplitter->setStretchFactor(1, 10);

    mainLayout->addWidget(mainSplitter);
    this->setLayout(mainLayout);

}

MainWindow::~MainWindow()
{

}
