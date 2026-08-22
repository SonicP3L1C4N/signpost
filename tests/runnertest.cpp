// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include <KRunner/AbstractRunnerTest>

#include <QTest>

/**
 * The plugin, loaded and queried the way KRunner does it.
 *
 * Worth more than looking at a screenshot of the launcher: what matters here
 * is not only that the right answer appears, but that the wrong ones stay out
 * of a box every other runner is also answering into.
 */
class RunnerTest : public KRunner::AbstractRunnerTest
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        initProperties();
    }

    void theWindowsNameFindsTheKdeOne()
    {
        const QList<KRunner::QueryMatch> matches = launchQuery(QStringLiteral("task manager"));
        QVERIFY(!matches.isEmpty());
        // The KDE name leads: it is the thing being looked for. The Windows
        // word is already in front of whoever typed it.
        QCOMPARE(matches.constFirst().text(), QStringLiteral("System Monitor"));
        QVERIFY(matches.constFirst().subtext().contains(QStringLiteral("Task Manager")));
    }

    void theChordFindsItToo()
    {
        const QList<KRunner::QueryMatch> matches = launchQuery(QStringLiteral("win+e"));
        QVERIFY(!matches.isEmpty());
        QCOMPARE(matches.constFirst().text(), QStringLiteral("Dolphin"));
    }

    // The launcher belongs to every runner at once. A passing mention in a
    // summary is a fine answer in our own window and an intrusion here.
    void aPassingMentionIsNotWorthInterruptingWith()
    {
        QVERIFY(launchQuery(QStringLiteral("memory")).isEmpty());
    }

    void twoLettersAreNotAQuestion()
    {
        QVERIFY(launchQuery(QStringLiteral("ta")).isEmpty());
    }

    void nonsenseAnswersNothing()
    {
        QVERIFY(launchQuery(QStringLiteral("defragment the flux capacitor")).isEmpty());
    }

    // Whatever is run, something has to happen: either the application starts
    // or the window opens on the explanation, and both need this to be set.
    void everyMatchCarriesWhatRunningItNeeds()
    {
        const QList<KRunner::QueryMatch> matches = launchQuery(QStringLiteral("control panel"));
        QVERIFY(!matches.isEmpty());
        for (const KRunner::QueryMatch &match : matches) {
            const QVariantMap data = match.data().toMap();
            QVERIFY(!data.value(QStringLiteral("windows")).toString().isEmpty());
            QVERIFY(data.contains(QStringLiteral("installed")));
            QVERIFY(!match.iconName().isEmpty());
        }
    }
};

QTEST_MAIN(RunnerTest)
#include "runnertest.moc"
