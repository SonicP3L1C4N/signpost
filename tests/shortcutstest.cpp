// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "shortcuts.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTest>
#include <QTextStream>

namespace
{
/** A file where the test says, rather than where this machine keeps its own. */
void write(const QString &path, const QString &contents)
{
    QDir().mkpath(QFileInfo(path).absolutePath());
    QFile file(path);
    QVERIFY2(file.open(QIODevice::WriteOnly | QIODevice::Truncate),
             qPrintable(path + QLatin1String(": ") + file.errorString()));
    QTextStream(&file) << contents;
}

QString dataPath(const QString &relative)
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
        + QLatin1Char('/') + relative;
}
}

class ShortcutsTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    /**
     * The lookups read two real files, so the test writes its own rather than
     * asking the machine it is running on -- where the answers change with the
     * distribution, the desktop, and whatever the developer has rebound.
     */
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);

        // Ships a launch chord, and an action carrying one of its own.
        write(dataPath(QStringLiteral("kglobalaccel/org.example.thing.desktop")),
              QStringLiteral("[Desktop Entry]\n"
                             "X-KDE-Shortcuts=Ctrl+Alt+T\n"
                             "\n"
                             "[Desktop Action Special]\n"
                             "X-KDE-Shortcuts=Display,Meta+P\n"));

        // kscreen's shape: nothing to launch it by, a chord on the action.
        write(dataPath(QStringLiteral("kglobalaccel/org.example.osd.desktop")),
              QStringLiteral("[Desktop Entry]\n"
                             "Actions=ShowOSD\n"
                             "\n"
                             "[Desktop Action ShowOSD]\n"
                             "X-KDE-Shortcuts=Display,Meta+P\n"));

        write(dataPath(QStringLiteral("kglobalaccel/org.example.cleared.desktop")),
              QStringLiteral("[Desktop Entry]\n"
                             "X-KDE-Shortcuts=Meta+E\n"));

        write(dataPath(QStringLiteral("kglobalaccel/org.example.emptied.desktop")),
              QStringLiteral("[Desktop Entry]\n"
                             "X-KDE-Shortcuts=Meta+E\n"));

        write(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation)
                  + QStringLiteral("/kglobalshortcutsrc"),
              QStringLiteral("[services][org.example.rebound.desktop]\n"
                             "_launch=Meta+K,Meta+E,Rebound\n"
                             "\n"
                             "[services][org.example.cleared.desktop]\n"
                             "_launch=none,Meta+E,Cleared\n"
                             "\n"
                             "[services][org.example.emptied.desktop]\n"
                             "_launch=\n"
                             "\n"
                             "[services][org.example.osd.desktop]\n"
                             "ShowOSD=Meta+Y,Meta+P,Switch Display\n"));
    }

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

    // ---- the two files, in the order they are believed ----

    void whatTheApplicationShippedIsTheAnswerUntilSomethingSaysOtherwise()
    {
        QCOMPARE(Shortcuts::forLaunch(QStringLiteral("org.example.thing.desktop")),
                 QStringLiteral("Ctrl+Alt+T"));
    }

    void whatTheUserReboundItToWins()
    {
        QCOMPARE(Shortcuts::forLaunch(QStringLiteral("org.example.rebound.desktop")),
                 QStringLiteral("Meta+K"));
    }

    /**
     * The whole point of reading the machine: an unbound shortcut must not be
     * filled back in from the default the application shipped, or the card
     * prints a chord that does nothing.
     */
    void aBindingTheUserTookAwayIsNotFilledInFromTheDefault()
    {
        QCOMPARE(Shortcuts::forLaunch(QStringLiteral("org.example.cleared.desktop")),
                 QString());
        QCOMPARE(Shortcuts::forLaunch(QStringLiteral("org.example.emptied.desktop")),
                 QString());
    }

    void nothingSaidAtAllIsNotTheSameAsSayingNone()
    {
        QCOMPARE(Shortcuts::forLaunch(QStringLiteral("org.example.thing.desktop")),
                 QStringLiteral("Ctrl+Alt+T"));
        QCOMPARE(Shortcuts::forLaunch(QStringLiteral("org.example.unknown.desktop")),
                 QString());
    }

    // ---- actions of a service, which are not its launch shortcut ----

    void anActionCarriesItsOwnShortcut()
    {
        QCOMPARE(Shortcuts::forService(QStringLiteral("org.example.thing.desktop"),
                                       QStringLiteral("Special")),
                 QStringLiteral("Meta+P"));
    }

    void aServiceWithNoLaunchShortcutStillHasItsActions()
    {
        QCOMPARE(Shortcuts::forLaunch(QStringLiteral("org.example.osd.desktop")),
                 QString());
        QCOMPARE(Shortcuts::forService(QStringLiteral("org.example.osd.desktop"),
                                       QStringLiteral("ShowOSD")),
                 QStringLiteral("Meta+Y"));
    }

    void anActionNobodyDefinedHasNoShortcut()
    {
        QCOMPARE(Shortcuts::forService(QStringLiteral("org.example.thing.desktop"),
                                       QStringLiteral("Absent")),
                 QString());
    }
};

QTEST_GUILESS_MAIN(ShortcutsTest)
#include "shortcutstest.moc"
