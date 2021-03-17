QT += gui widgets opengl
TEMPLATE = app
CONFIG += c++11


SOURCES += \
        MainWindow.cpp \
        OglArea.cpp \
        main.cpp

HEADERS += \
    MainWindow.h \
    OglArea.h \
    Shaders.h

RESOURCES += \
    resources.qrc
