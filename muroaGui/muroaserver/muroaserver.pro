TEMPLATE = app
TARGET = muroaserver
QT += core \
    gui \
    xml \
    network
HEADERS += cconnection.h \
    ccontenthandler.h \
    cnetwork.h \
    cstatemachine.h \
    muroaserver.h
SOURCES += cconnection.cpp \
    ccontenthandler.cpp \
    cnetwork.cpp \
    cstatemachine.cpp \
    main.cpp \
    muroaserver.cpp
FORMS += muroaserver.ui
RESOURCES += 
