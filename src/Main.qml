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

    /** Seeded from the command line, so `signpost "task manager"` lands on it. */
    property string initialSearch: ""

    minimumWidth: Kirigami.Units.gridUnit * 26
    minimumHeight: Kirigami.Units.gridUnit * 20
    width: Kirigami.Units.gridUnit * 46
    height: Kirigami.Units.gridUnit * 34

    Phrasebook {
        id: phrasebook
    }

    Component {
        id: aboutPage
        AboutPage {}
    }

    pageStack.initialPage: Kirigami.ScrollablePage {
        id: page

        // No global toolbar: the window title already says Signpost, and the
        // search field is the whole interface rather than a decoration on a
        // heading.
        globalToolBarStyle: Kirigami.ApplicationHeaderStyle.None

        header: QQC2.Control {
            width: page.width

            leftPadding: Kirigami.Units.largeSpacing
            rightPadding: Kirigami.Units.largeSpacing
            topPadding: Kirigami.Units.smallSpacing
            bottomPadding: Kirigami.Units.smallSpacing

            contentItem: ColumnLayout {
                spacing: Kirigami.Units.smallSpacing

                Kirigami.SearchField {
                    id: search
                    Layout.fillWidth: true
                    text: root.initialSearch
                    placeholderText: i18n("What is it called on Windows?")
                    onTextChanged: phrasebook.search = text
                    focus: true
                }

                // Ninety-one answers in one alphabetical column is a list to
                // scroll rather than a thing to browse.
                RowLayout {
                    Layout.fillWidth: true
                    spacing: Kirigami.Units.smallSpacing

                    Kirigami.Chip {
                        text: i18n("Everything")
                        closable: false
                        checkable: true
                        checked: phrasebook.category === ""
                        onClicked: phrasebook.category = ""
                    }

                    Repeater {
                        model: phrasebook.categories

                        Kirigami.Chip {
                            required property string modelData

                            text: modelData
                            closable: false
                            checkable: true
                            checked: phrasebook.category === modelData
                            onClicked: phrasebook.category =
                                (phrasebook.category === modelData ? "" : modelData)
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    QQC2.Label {
                        text: phrasebook.count === phrasebook.total
                            ? i18np("%1 answer", "%1 answers", phrasebook.total)
                            : i18n("%1 of %2", phrasebook.count, phrasebook.total)
                        opacity: 0.6
                        font: Kirigami.Theme.smallFont
                    }

                    // Which version is running, answerable at a glance rather
                    // than only from the About page.
                    QQC2.Label {
                        text: i18n("· Signpost %1", Environment.version)
                        opacity: 0.45
                        font: Kirigami.Theme.smallFont
                    }

                    QQC2.ToolButton {
                        icon.name: "application-menu"
                        display: QQC2.AbstractButton.IconOnly
                        text: i18n("Menu")
                        onClicked: menu.popup(this, 0, height)

                        QQC2.Menu {
                            id: menu

                            QQC2.MenuItem {
                                icon.name: "help-about"
                                text: i18n("About Signpost")
                                onTriggered: root.pageStack.push(aboutPage, {
                                    entryCount: phrasebook.total,
                                })
                            }

                            QQC2.MenuSeparator {}

                            QQC2.MenuItem {
                                icon.name: "internet-web-browser"
                                text: i18n("Project page…")
                                onTriggered: Qt.openUrlExternally(Environment.homepage)
                            }

                            QQC2.MenuItem {
                                icon.name: "tools-report-bug"
                                text: i18n("Report a bug…")
                                onTriggered: Qt.openUrlExternally(Environment.bugAddress)
                            }
                        }
                    }
                }
            }
        }

        ListView {
            id: list

            model: phrasebook
            spacing: Kirigami.Units.largeSpacing
            currentIndex: -1
            reuseItems: true

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
