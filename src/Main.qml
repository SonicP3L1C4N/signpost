// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.kirigami as Kirigami
import org.kde.signpost

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Signpost")

    minimumWidth: Kirigami.Units.gridUnit * 22
    minimumHeight: Kirigami.Units.gridUnit * 20
    width: Kirigami.Units.gridUnit * 42
    height: Kirigami.Units.gridUnit * 32

    Phrasebook {
        id: phrasebook
    }

    pageStack.initialPage: Kirigami.ScrollablePage {
        id: page

        // The search field is the whole interface, so it takes the title's
        // place rather than sitting under it.
        titleDelegate: Kirigami.SearchField {
            Layout.fillWidth: true
            Layout.rightMargin: Kirigami.Units.largeSpacing
            placeholderText: i18n("What is it called on Windows?")
            onTextChanged: phrasebook.search = text
        }

        ListView {
            id: list

            model: phrasebook
            spacing: Kirigami.Units.largeSpacing
            currentIndex: -1

            delegate: EntryCard {
                width: ListView.view.width
            }

            Kirigami.PlaceholderMessage {
                anchors.centerIn: parent
                width: parent.width - Kirigami.Units.gridUnit * 4
                visible: list.count === 0
                icon.name: "edit-find"
                text: i18n("Nothing by that name")
                explanation: i18n("Try the Windows name for it — “Task Manager”, “Control Panel”, “Win+E”.")
            }
        }
    }
}
