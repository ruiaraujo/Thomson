TEMPLATE = app
TARGET = thomson
QT += core \
    gui
HEADERS += TextMode.h \
    GraphicMode.h \
    BruteForce.h \
    unknown.h
SOURCES += TextMode.cpp \
    GraphicMode.cpp \
    BruteForce.cpp \
    main.cpp
FORMS += crack.ui
RESOURCES += icon.qrc
LIBS += -lcrypto \
    -lgomp
QMAKE_CXXFLAGS += -fopenmp \
    -O3
