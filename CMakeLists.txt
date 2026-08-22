# SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

cmake_minimum_required(VERSION 3.20)

project(signpost VERSION 0.1.0)

set(QT_MIN_VERSION 6.6.0)
set(KF_MIN_VERSION 6.0.0)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMDeprecationSettings)
include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core Gui Qml Quick QuickControls2 Widgets Test)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Kirigami I18n CoreAddons Config Service KIO)

# Optional so that the application still builds where KRunner's development
# files are not installed; without it you lose the plugin, not the program.
find_package(KF6Runner ${KF_MIN_VERSION})
set_package_properties(KF6Runner PROPERTIES
    TYPE OPTIONAL
    PURPOSE "Answers Windows names in KRunner, without opening Signpost at all"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

ecm_set_disabled_deprecation_versions(QT 6.6 KF 6.0)

qt_standard_project_setup(REQUIRES 6.6)

add_subdirectory(src)

if(TARGET KF6::Runner)
    add_subdirectory(src/runner)
endif()

# Named <size>-<group>-<name>.svg, which is what ecm_install_icons reads to
# work out where each one belongs in the hicolor theme.
ecm_install_icons(ICONS
    icons/16-apps-signpost.svg
    icons/22-apps-signpost.svg
    icons/32-apps-signpost.svg
    icons/48-apps-signpost.svg
    icons/64-apps-signpost.svg
    icons/128-apps-signpost.svg
    icons/sc-apps-signpost.svg
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)

enable_testing()
add_subdirectory(tests)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
