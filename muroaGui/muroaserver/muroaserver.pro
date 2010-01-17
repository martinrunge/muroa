TEMPLATE = app
TARGET = muroaserver
QT += core \
    gui \
    xml \
    network
HEADERS += CCollection.h \
    CCollectionItem.h \
    CDiff.h \
    CItemBase.h \
    CPlaylistItem.h \
    CSession.h \
    cconnection.h \
    ccontenthandler.h \
    cnetwork.h \
    cstatemachine.h \
    muroaserver.h \
    CItemBase.h \
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
SOURCES += CCollectionItem.cpp \
    CDiff.cpp \
    CItemBase.cpp \
    CPlaylistItem.cpp \
    CSession.cpp \
    cconnection.cpp \
    ccontenthandler.cpp \
    cnetwork.cpp \
    cstatemachine.cpp \
    main.cpp \
    muroaserver.cpp
FORMS += muroaserver.ui
LIBS += -lxdiff
