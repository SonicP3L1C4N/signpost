// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.kirigami as Kirigami
import org.kde.signpost

Kirigami.AbstractCard {
    id: card

    required property string windowsName
    required property string kdeName
    required property string summary
    required property string category
    required property string shortcut
    required property string desktopId
    required property string iconName
    required property bool installed

    contentItem: RowLayout {
        spacing: Kirigami.Units.largeSpacing

        // The application's own icon: half of "where is it" is knowing what
        // to look for once you are in the launcher.
        Kirigami.Icon {
            source: card.iconName
            Layout.alignment: Qt.AlignTop
            Layout.preferredWidth: Kirigami.Units.iconSizes.large
            Layout.preferredHeight: Kirigami.Units.iconSizes.large
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: Kirigami.Units.smallSpacing

            RowLayout {
                Layout.fillWidth: true
                spacing: Kirigami.Units.smallSpacing

                Kirigami.Heading {
                    level: 3
                    text: card.windowsName
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                QQC2.Label {
                    text: card.category
                    opacity: 0.6
                    font: Kirigami.Theme.smallFont
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: Kirigami.Units.smallSpacing

                Kirigami.Icon {
                    source: "arrow-right"
                    implicitWidth: Kirigami.Units.iconSizes.small
                    implicitHeight: Kirigami.Units.iconSizes.small
                }

                Kirigami.Heading {
                    level: 4
                    text: card.kdeName
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                // Read from this machine's own configuration, so it is what is
                // bound here rather than what a wiki said in 2019.
                Kirigami.Chip {
                    visible: card.shortcut.length > 0
                    text: card.shortcut
                    closable: false
                    checkable: false
                }
            }

            QQC2.Label {
                Layout.fillWidth: true
                text: card.summary
                wrapMode: Text.WordWrap
                opacity: 0.85
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.topMargin: Kirigami.Units.smallSpacing
                visible: card.desktopId.length > 0

                Item {
                    Layout.fillWidth: true
                }

                QQC2.Button {
                    visible: card.installed
                    icon.name: "system-run"
                    text: i18n("Open it")
                    onClicked: Launcher.launch(card.desktopId)
                }

                QQC2.Button {
                    visible: !card.installed
                    icon.name: "download"
                    text: i18n("Get it")
                    onClicked: Launcher.showInDiscover(card.desktopId)
                }
            }
        }
    }
}
