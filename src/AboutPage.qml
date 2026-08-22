// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.kirigami as Kirigami
import org.kde.signpost

/**
 * Which version this is, and the block a bug report needs.
 *
 * The details are the reason this is a page rather than a message box: a
 * report from someone who moved to this desktop last week is worth far more
 * with the versions attached, and they will not know where to find them.
 */
Kirigami.ScrollablePage {
    id: page

    required property int entryCount

    title: i18n("About Signpost")

    ColumnLayout {
        width: page.width - Kirigami.Units.gridUnit * 2
        spacing: Kirigami.Units.largeSpacing

        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            spacing: Kirigami.Units.largeSpacing

            Kirigami.Icon {
                source: "io.github.sonicp3l1c4n.signpost"
                fallback: "qrc:/sc-apps-io.github.sonicp3l1c4n.signpost.svg"
                implicitWidth: Kirigami.Units.iconSizes.huge
                implicitHeight: Kirigami.Units.iconSizes.huge
                Layout.alignment: Qt.AlignTop
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: Kirigami.Units.smallSpacing

                Kirigami.Heading {
                    level: 1
                    text: i18n("Signpost %1", Environment.version)
                }

                QQC2.Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    text: i18np("What things are called on KDE, when you have come from Windows. %1 answer.",
                                "What things are called on KDE, when you have come from Windows. %1 answers.",
                                page.entryCount)
                }

                QQC2.Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    opacity: 0.7
                    text: i18n("© 2026 Gary Bissett · GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL")
                }

                RowLayout {
                    spacing: Kirigami.Units.largeSpacing

                    Kirigami.UrlButton {
                        url: Environment.homepage
                        text: i18n("Project page")
                    }

                    Kirigami.UrlButton {
                        url: Environment.bugAddress
                        text: i18n("Report a bug")
                    }
                }
            }
        }

        Kirigami.Heading {
            level: 3
            text: i18n("Details")
        }

        Kirigami.AbstractCard {
            Layout.fillWidth: true

            contentItem: ColumnLayout {
                spacing: Kirigami.Units.largeSpacing

                QQC2.TextArea {
                    Layout.fillWidth: true
                    text: Environment.report(page.entryCount)
                    readOnly: true
                    font.family: "monospace"
                    background: null
                    wrapMode: TextEdit.NoWrap
                }

                RowLayout {
                    Layout.fillWidth: true

                    QQC2.Label {
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        opacity: 0.7
                        text: i18n("Worth pasting into a bug report: the answer to “what did it happen on” is usually the first thing asked and the last thing anyone has to hand.")
                    }

                    QQC2.Button {
                        id: copyButton
                        icon.name: "edit-copy"
                        text: i18n("Copy details")
                        Layout.alignment: Qt.AlignBottom
                        onClicked: {
                            Environment.copyReport(page.entryCount);
                            text = i18n("Copied");
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
