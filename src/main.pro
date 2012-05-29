TEMPLATE = app
TARGET = ../snowshoe

QT += widgets quick core-private gui-private
LIBS += -Lcore/ -lsnowshoe

linux-g++-maemo {
    DEFINES += SNOWSHOE_MEEGO_HARMATTAN
}

SOURCES += \
    mobile/BrowserWindowMobile.cpp \
    main.cpp

HEADERS += \
    mobile/BrowserWindowMobile.h

RESOURCES += \
    mobile/snowshoe-mobile.qrc

MOC_DIR = .moc/
RCC_DIR = .rcc/
OBJECTS_DIR = .obj/

OTHER_FILES += \
    mobile/qml/Main.qml \
    mobile/qml/main-harmattan.qml

# Extra files and install rules for N9.
# TODO support desktop install paths

desktop.path = /usr/share/applications/
icon.files = icons/snowshoe80.png

linux-g++-maemo {
    target.path = /usr/share/snowshoe/

    launcher.files = mobile/snowshoe_launcher
    launcher.path = /usr/bin

    desktop.files = mobile/snowshoe_harmattan.desktop
    icon.path = /usr/share/themes/base/meegotouch/icons/

    # powervr ini file
    powervr.files = mobile/snowshoe.ini
    powervr.path = /etc/powervr.d/

    INSTALLS += launcher powervr

} else {
    target.path = /usr/bin
    desktop.files = desktop/snowshoe.desktop
    icon.path = /usr/share/pixmaps/
}

INSTALLS += target desktop icon
