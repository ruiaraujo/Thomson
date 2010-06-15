TEMPLATE = app
TARGET = thomson
QT += core \
    gui
HEADERS += Crack.h \
    BruteForce.h \
    unknown.h
SOURCES += Crack.cpp \
    BruteForce.cpp \
    main.cpp
FORMS += crack.ui
RESOURCES += icon.qrc
LIBS += -lcrypto \
    -lgomp
QMAKE_CXXFLAGS += -fopenmp \
    -O3
