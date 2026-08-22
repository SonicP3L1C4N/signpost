# SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

# The same sources the window uses, compiled again rather than shared through a
# library: the window's target owns the QML module, and a plugin has no use for
# any of that.
kcoreaddons_add_plugin(signpostrunner INSTALL_NAMESPACE "kf6/krunner")

target_sources(signpostrunner PRIVATE
    signpostrunner.cpp
    ${CMAKE_SOURCE_DIR}/src/entrymodel.cpp
    ${CMAKE_SOURCE_DIR}/src/ranking.cpp
    ${CMAKE_SOURCE_DIR}/src/shortcuts.cpp
)

target_include_directories(signpostrunner PRIVATE ${CMAKE_SOURCE_DIR}/src)

# The plugin carries its own copy of the phrasebook: it is loaded into
# KRunner's process, where the application's resources are not.
qt_add_resources(signpostrunner "runner_phrasebook"
    PREFIX "/"
    BASE ${CMAKE_SOURCE_DIR}/data
    FILES ${CMAKE_SOURCE_DIR}/data/entries.json
)

target_link_libraries(signpostrunner PRIVATE
    Qt6::Gui
    KF6::Runner
    KF6::I18n
    KF6::Service
    KF6::KIOGui
    KF6::ConfigCore
    KF6::CoreAddons
)
