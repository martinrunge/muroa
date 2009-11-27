TEMPLATE = app
TARGET = muroaserver
QT += core \
    gui \
    xml \
    network
HEADERS += CDiff.h \
    CSession.h \
    CCollectionItem.h \
    CCollection.h \
    cconnection.h \
    ccontenthandler.h \
    cnetwork.h \
    cstatemachine.h \
    muroaserver.h
SOURCES += CDiff.cpp \
    CSession.cpp \
    CCollectionItem.cpp \
    cconnection.cpp \
    ccontenthandler.cpp \
    cnetwork.cpp \
    cstatemachine.cpp \
    main.cpp \
    muroaserver.cpp
FORMS += muroaserver.ui
LIBS += -lxdiff
