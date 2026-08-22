# SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

# The phrasebook itself: everything but main(), so the tests can link it.
add_library(signpost_lib STATIC
    entrymodel.cpp
    searchmodel.cpp
    shortcuts.cpp
    launcher.cpp
)

target_link_libraries(signpost_lib PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    KF6::ConfigCore
    KF6::CoreAddons
    KF6::I18n
    KF6::Service
    KF6::KIOGui
)

target_include_directories(signpost_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# The dataset is compiled in: it is small, it is the program's whole subject,
# and an installed copy that cannot find its data file is no use to anyone.
qt_add_resources(signpost_lib "phrasebook"
    PREFIX "/"
    BASE ${CMAKE_SOURCE_DIR}/data
    FILES ${CMAKE_SOURCE_DIR}/data/entries.json
)

add_executable(signpost main.cpp)

qt_add_qml_module(signpost
    URI org.kde.signpost
    VERSION 1.0
    QML_FILES
        Main.qml
        EntryCard.qml
)

target_compile_definitions(signpost PRIVATE SIGNPOST_VERSION="${PROJECT_VERSION}")

target_link_libraries(signpost PRIVATE
    signpost_lib
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
    KF6::I18n
    KF6::Kirigami
)

install(TARGETS signpost ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS io.github.sonicp3l1c4n.signpost.desktop
        DESTINATION ${KDE_INSTALL_APPDIR})
