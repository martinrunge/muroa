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
    cstatemachine.h \
    cconnection.h \
    muroagui.h \
    CCollectionModel.h
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
    cstatemachine.cpp \
    cconnection.cpp \
    main.cpp \
    muroagui.cpp \
    CCollectionModel.cpp
FORMS += muroagui.ui
RESOURCES += 
