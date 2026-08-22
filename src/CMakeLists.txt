# SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

# One target rather than a library plus a shell over it: the QML types are
# registered by the target that owns the QML module, so splitting them out
# leaves QML unable to see them. The tests compile the sources they need.
add_executable(signpost
    main.cpp
    entrymodel.cpp
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
    FILES ${CMAKE_SOURCE_DIR}/icons/sc-apps-signpost.svg
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
