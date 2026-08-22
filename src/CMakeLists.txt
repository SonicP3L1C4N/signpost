# SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

# One target rather than a library plus a shell over it: the QML types are
# registered by the target that owns the QML module, so splitting them out
# leaves QML unable to see them. The tests compile the sources they need.
add_executable(signpost
    main.cpp
    entrymodel.cpp
    environment.cpp
    ranking.cpp
    searchmodel.cpp
    shortcuts.cpp
    launcher.cpp
)

qt_add_qml_module(signpost
    URI org.kde.signpost
    VERSION 1.0
    QML_FILES
        Main.qml
        EntryCard.qml
        AboutPage.qml
)

# The dataset is compiled in: it is small, it is the program's whole subject,
# and an installed copy that cannot find its data file is no use to anyone.
qt_add_resources(signpost "phrasebook"
    PREFIX "/"
    BASE ${CMAKE_SOURCE_DIR}/data
    FILES ${CMAKE_SOURCE_DIR}/data/entries.json
)

qt_add_resources(signpost "appicon"
    PREFIX "/"
    BASE ${CMAKE_SOURCE_DIR}/icons
    FILES ${CMAKE_SOURCE_DIR}/icons/sc-apps-io.github.sonicp3l1c4n.signpost.svg
)

# Interface artwork rather than an icon theme's business, so it is carried
# rather than installed.
qt_add_resources(signpost "origin"
    PREFIX "/origin"
    BASE ${CMAKE_SOURCE_DIR}/icons/origin
    FILES
        ${CMAKE_SOURCE_DIR}/icons/origin/origin-apps.svg
        ${CMAKE_SOURCE_DIR}/icons/origin/origin-files.svg
        ${CMAKE_SOURCE_DIR}/icons/origin/origin-internet.svg
        ${CMAKE_SOURCE_DIR}/icons/origin/origin-settings.svg
        ${CMAKE_SOURCE_DIR}/icons/origin/origin-shortcuts.svg
        ${CMAKE_SOURCE_DIR}/icons/origin/origin-system.svg
)

target_compile_definitions(signpost PRIVATE SIGNPOST_VERSION="${PROJECT_VERSION}")

target_link_libraries(signpost PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::ConfigCore
    KF6::CoreAddons
    KF6::I18n
    KF6::KIOGui
    KF6::Kirigami
    KF6::Service
)

install(TARGETS signpost ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS io.github.sonicp3l1c4n.signpost.desktop
        DESTINATION ${KDE_INSTALL_APPDIR})

# Without this the application cannot appear in Discover, or in any other
# software centre, which is where the people this is written for will look.
install(FILES io.github.sonicp3l1c4n.signpost.metainfo.xml
        DESTINATION ${KDE_INSTALL_METAINFODIR})

# A launch shortcut, shipped the way every KDE application ships one: a copy of
# the menu entry under kglobalaccel/ carrying X-KDE-Shortcuts. Which is the
# same file this application reads to tell you what Win+E became -- it now
# answers for itself as well.
set(SIGNPOST_ACCEL_DIR ${CMAKE_CURRENT_BINARY_DIR}/kglobalaccel)
file(MAKE_DIRECTORY ${SIGNPOST_ACCEL_DIR})
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/io.github.sonicp3l1c4n.signpost.desktop
     SIGNPOST_ENTRY)
file(WRITE ${SIGNPOST_ACCEL_DIR}/io.github.sonicp3l1c4n.signpost.desktop
     "${SIGNPOST_ENTRY}X-KDE-Shortcuts=Meta+/\n")
install(FILES ${SIGNPOST_ACCEL_DIR}/io.github.sonicp3l1c4n.signpost.desktop
        DESTINATION ${KDE_INSTALL_DATADIR}/kglobalaccel)
