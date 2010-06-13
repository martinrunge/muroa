TEMPLATE = app
TARGET = muroaserver
QT += core \
    gui \
    xml \
    network \
    dbus
HEADERS += mediaprocessing/CStream.h \
    CDnsSdAvahi.h \
    CDnsSdAvahiViaQtDBus.h \
    IDnsSd.h \
    CConnection.h \
    CContentHandler.h \
    CMuroaServer.h \
    CNetwork.h \
    CStateMachine.h \
    CCollection.h \
    CCollectionItem.h \
    CDiff.h \
    CItemBase.h \
    CPlaylistItem.h \
    CSession.h \
    CItemBase.h \
    CPlaylistItem.h \
    CDiff.h \
    CSession.h \
    CCollectionItem.h \
    CCollection.h
SOURCES += mediaprocessing/CStream.cpp \
    CDnsSdAvahi.cpp \
    CDnsSdAvahiViaQtDBus.cpp \
    IDnsSd.cpp \
    CConnection.cpp \
    CCollectionItem.cpp \
    CDiff.cpp \
    CItemBase.cpp \
    CPlaylistItem.cpp \
    CSession.cpp \
    main.cpp \
    CStateMachine.cpp \
    CContentHandler.cpp \
    CMuroaServer.cpp \
    CNetwork.cpp
FORMS += MuroaServer.ui
LIBS += -lxdiff \
    -lavahi-client \
    -lavahi-qt4
