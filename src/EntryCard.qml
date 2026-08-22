// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.kirigami as Kirigami
import org.kde.signpost

/**
 * One answer, read left to right: the thing you knew, then the thing in front
 * of you. The grey mark is ours rather than Microsoft's, and grey against the
 * colour of the application icon is the point of the card.
 */
Kirigami.AbstractCard {
    id: card

    required property string windowsName
    required property string kdeName
    required property string summary
    required property string category
    required property string shortcut
    required property string desktopId
    required property string iconName
    required property string originIcon
    required property bool installed

    // A fixed column for the Windows side, so every arrow and every
    // application icon lands in the same place down the list. Ragged ones
    // read as a list of unrelated rows rather than as one translation
    // repeated.
    readonly property int originWidth: Math.min(Kirigami.Units.gridUnit * 10,
                                                width * 0.34)

    showClickFeedback: installed && desktopId.length > 0
    onClicked: if (showClickFeedback) Launcher.launch(card.desktopId)

    contentItem: ColumnLayout {
        spacing: Kirigami.Units.smallSpacing

        RowLayout {
            Layout.fillWidth: true
            spacing: Kirigami.Units.largeSpacing

            // ---- what you came in knowing ----
            RowLayout {
                spacing: Kirigami.Units.smallSpacing
                Layout.preferredWidth: card.originWidth
                Layout.maximumWidth: card.originWidth

                Image {
                    source: card.originIcon
                    sourceSize.width: Kirigami.Units.iconSizes.smallMedium
                    sourceSize.height: Kirigami.Units.iconSizes.smallMedium
                    Layout.alignment: Qt.AlignVCenter
                    opacity: 0.9
                }

                QQC2.Label {
                    Layout.fillWidth: true
                    text: card.windowsName
                    elide: Text.ElideRight
                    font.weight: Font.DemiBold
                    opacity: 0.75
                }
            }

            Kirigami.Icon {
                source: "arrow-right"
                implicitWidth: Kirigami.Units.iconSizes.small
                implicitHeight: Kirigami.Units.iconSizes.small
                opacity: 0.5
                Layout.alignment: Qt.AlignVCenter
            }

            // ---- what it is here ----
            Kirigami.Icon {
                source: card.iconName
                implicitWidth: Kirigami.Units.iconSizes.medium
                implicitHeight: Kirigami.Units.iconSizes.medium
                Layout.alignment: Qt.AlignVCenter
            }

            Kirigami.Heading {
                level: 3
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
                Layout.alignment: Qt.AlignVCenter
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.smallSpacing
            spacing: Kirigami.Units.largeSpacing

            QQC2.Label {
                Layout.fillWidth: true
                Layout.leftMargin: card.originWidth
                                   + Kirigami.Units.iconSizes.small
                                   + Kirigami.Units.largeSpacing * 2
                text: card.summary
                wrapMode: Text.WordWrap
                opacity: 0.8
            }

            QQC2.Button {
                visible: card.desktopId.length > 0 && card.installed
                icon.name: "system-run"
                text: i18n("Open it")
                Layout.alignment: Qt.AlignBottom
                onClicked: Launcher.launch(card.desktopId)
            }

            QQC2.Button {
                visible: card.desktopId.length > 0 && !card.installed
                icon.name: "download"
                text: i18n("Get it")
                Layout.alignment: Qt.AlignBottom
                onClicked: Launcher.showInDiscover(card.desktopId)
            }
        }
    }
}
