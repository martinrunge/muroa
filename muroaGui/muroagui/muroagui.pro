TEMPLATE = app
TARGET = muroagui
QT += core \
    gui \
    xml \
    network
HEADERS += cstatemachine.h \
    cconnection.h \
    muroagui.h
SOURCES += cstatemachine.cpp \
    cconnection.cpp \
    main.cpp \
    muroagui.cpp
FORMS += muroagui.ui
RESOURCES += 
