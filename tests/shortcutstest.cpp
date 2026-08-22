// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "shortcuts.h"

#include <QTest>

class ShortcutsTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    // "current,default,description", where current may hold alternatives.
    void takesWhatIsBoundRatherThanTheDefault()
    {
        QCOMPARE(Shortcuts::fromGlobalShortcutsValue(
                     QStringLiteral("Meta+D,Meta+D,Peek at Desktop")),
                 QStringLiteral("Meta+D"));
    }

    void takesTheFirstOfSeveralAlternatives()
    {
        QCOMPARE(Shortcuts::fromGlobalShortcutsValue(
                     QStringLiteral("Meta+Tab\tAlt+Tab,Meta+Tab\tAlt+Tab,Walk Through Windows")),
                 QStringLiteral("Meta+Tab"));
    }

    void anUnboundActionHasNoShortcut()
    {
        QCOMPARE(Shortcuts::fromGlobalShortcutsValue(
                     QStringLiteral("none,none,Cycle through Overview and Grid View")),
                 QString());
        QCOMPARE(Shortcuts::fromGlobalShortcutsValue(QString()), QString());
    }

    // X-KDE-Shortcuts is a plain comma-separated list, and mixes chords with
    // media keys: "what do I press" is answered by the chord.
    void aChordBeatsAMediaKey()
    {
        QCOMPARE(Shortcuts::fromDesktopShortcutsValue(QStringLiteral("Display,Meta+P")),
                 QStringLiteral("Meta+P"));
        QCOMPARE(Shortcuts::fromDesktopShortcutsValue(QStringLiteral("Tools,Meta+I")),
                 QStringLiteral("Meta+I"));
    }

    void theFirstChordWinsWhenThereAreSeveral()
    {
        QCOMPARE(Shortcuts::fromDesktopShortcutsValue(
                     QStringLiteral("Alt+Space,Alt+F2,Search")),
                 QStringLiteral("Alt+Space"));
    }

    // KCalc ships only the Calculator media key. A chip reading "Calculator"
    // answers a question nobody asked, so it counts as no shortcut.
    void aMediaKeyOnItsOwnIsNotAnAnswer()
    {
        QCOMPARE(Shortcuts::fromDesktopShortcutsValue(QStringLiteral("Calculator")),
                 QString());
        QCOMPARE(Shortcuts::fromDesktopShortcutsValue(QString()), QString());
    }
};

QTEST_GUILESS_MAIN(ShortcutsTest)
#include "shortcutstest.moc"
