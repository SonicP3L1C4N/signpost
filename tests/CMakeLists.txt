# SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

include(ECMAddTests)

# The phrasebook without the window: everything the tests actually exercise.
set(SIGNPOST_SOURCES
    ${CMAKE_SOURCE_DIR}/src/entrymodel.cpp
    ${CMAKE_SOURCE_DIR}/src/searchmodel.cpp
    ${CMAKE_SOURCE_DIR}/src/shortcuts.cpp
)

ecm_add_test(searchtest.cpp ${SIGNPOST_SOURCES}
    TEST_NAME searchtest
    LINK_LIBRARIES Qt6::Test Qt6::Gui Qt6::Qml KF6::ConfigCore KF6::CoreAddons
                   KF6::I18n KF6::Service
)
target_include_directories(searchtest PRIVATE ${CMAKE_SOURCE_DIR}/src)

# The shipped dataset is the thing under test, so the test carries it too.
qt_add_resources(searchtest "phrasebook"
    PREFIX "/"
    BASE ${CMAKE_SOURCE_DIR}/data
    FILES ${CMAKE_SOURCE_DIR}/data/entries.json
)

ecm_add_test(shortcutstest.cpp ${CMAKE_SOURCE_DIR}/src/shortcuts.cpp
    TEST_NAME shortcutstest
    LINK_LIBRARIES Qt6::Test Qt6::Gui KF6::ConfigCore
)
target_include_directories(shortcutstest PRIVATE ${CMAKE_SOURCE_DIR}/src)

ecm_add_test(abouttest.cpp ${CMAKE_SOURCE_DIR}/src/environment.cpp
    TEST_NAME abouttest
    LINK_LIBRARIES Qt6::Test Qt6::Gui Qt6::Qml KF6::CoreAddons
)
target_include_directories(abouttest PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(abouttest PRIVATE SIGNPOST_VERSION="${PROJECT_VERSION}")
