TEMPLATE = app
TARGET = muroaserver
QT += core \
    gui \
    xml \
    network
HEADERS += CItemBase.h \
    CPlaylistItem.h \
    CDiff.h \
    CSession.h \
    CCollectionItem.h \
    CCollection.h \
    cconnection.h \
    ccontenthandler.h \
    cnetwork.h \
    cstatemachine.h \
    muroaserver.h
SOURCES += CItemBase.cpp \
    CPlaylistItem.cpp \
    CDiff.cpp \
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
