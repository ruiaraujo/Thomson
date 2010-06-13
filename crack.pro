TEMPLATE = app
TARGET = crack
QT += core \
    gui
HEADERS += Crack.h \
    BruteForce.h \
    unknown.h
SOURCES += Crack.cpp \
    BruteForce.cpp \
    main.cpp
FORMS += crack.ui
RESOURCES += 
LIBS += -lssl \
    -lgomp \
    -fopenmp
QMAKE_CXXFLAGS += -fopenmp \
    -O3
