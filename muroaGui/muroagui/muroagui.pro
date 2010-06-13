TEMPLATE = app
TARGET = muroagui
QT += core \
    gui \
    xml \
    network \
    dbus
HEADERS += CSessionCommand.h \
    CDnsSdServiceBrowserAvahi.h \
    CDnsSdBase.h \
    CServiceDesc.h \
    CServiceBrowser.h \
    CDnsSdAvahiViaQtDBus.h \
    CCollectionCommand.h \
    CNextlistCommand.h \
    CCollectionView.h \
    CCommandBase.h \
    CPlaylistCommand.h \
    CModelDiff.h \
    CPlaylistView.h \
    CModelBase.h \
    CPlaylistModel.h \
    CPlaylistItem.h \
    CItemBase.h \
    CCollectionItem.h \
    CCollection.h \
    CStateMachine.h \
    CConnection.h \
    CMuroaGui.h \
    CCollectionModel.h \
    CDiffBuilder.h
SOURCES += CSessionCommand.cpp \
    CDnsSdServiceBrowserAvahi.cpp \
    CDnsSdBase.cpp \
    CServiceDesc.cpp \
    CServiceBrowser.cpp \
    CDnsSdAvahiViaQtDBus.cpp \
    CCollectionCommand.cpp \
    CNextlistCommand.cpp \
    CCollectionView.cpp \
    CCommandBase.cpp \
    CPlaylistCommand.cpp \
    CModelDiff.cpp \
    CPlaylistView.cpp \
    CModelBase.cpp \
    CPlaylistModel.cpp \
    CPlaylistItem.cpp \
    CItemBase.cpp \
    CCollectionItem.cpp \
    CStateMachine.cpp \
    CConnection.cpp \
    main.cpp \
    CMuroaGui.cpp \
    CCollectionModel.cpp \
    CDiffBuilder.cpp
FORMS += ServiceBrowser.ui \
    ServiceBrowser.ui \
    MuroaGui.ui
RESOURCES += MuroaGui.qrc
LIBS += -lavahi-client \
    -lavahi-qt4
