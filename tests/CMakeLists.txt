# SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

include(ECMAddTests)

ecm_add_test(searchtest.cpp
    TEST_NAME searchtest
    LINK_LIBRARIES signpost_lib Qt6::Test
)

ecm_add_test(shortcutstest.cpp
    TEST_NAME shortcutstest
    LINK_LIBRARIES signpost_lib Qt6::Test
)
