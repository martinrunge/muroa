TEMPLATE = app
TARGET = muroagui
QT += core \
    gui \
    xml \
    network
HEADERS += CCollectionView.h \
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
SOURCES += CCollectionView.cpp \
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
FORMS += MuroaGui.ui
RESOURCES += 
