// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "entry.h"
#include "entrymodel.h"
#include "searchmodel.h"

#include <QTest>

class SearchTest : public QObject
{
    Q_OBJECT

private:
    static Entry taskManager()
    {
        Entry entry;
        entry.id = QStringLiteral("task-manager");
        entry.windows = QStringLiteral("Task Manager");
        entry.aliases = {QStringLiteral("taskmgr"), QStringLiteral("end task")};
        entry.kde = QStringLiteral("System Monitor");
        entry.summary = QStringLiteral("Processes, CPU and memory.");
        entry.category = QStringLiteral("System");
        return entry;
    }

private Q_SLOTS:
    void everythingMatchesAnEmptySearch()
    {
        QVERIFY(SearchModel::score(taskManager(), QString()) > 0);
    }

    void theWindowsNameOutranksEverythingElse()
    {
        const Entry entry = taskManager();
        const int exact = SearchModel::score(entry, QStringLiteral("task manager"));
        const int alias = SearchModel::score(entry, QStringLiteral("taskmgr"));
        const int summary = SearchModel::score(entry, QStringLiteral("memory"));
        QVERIFY(exact > alias);
        QVERIFY(alias > summary);
        QVERIFY(summary > 0);
    }

    void searchingIsCaseInsensitive()
    {
        QCOMPARE(SearchModel::score(taskManager(), QStringLiteral("TASK MANAGER")),
                 SearchModel::score(taskManager(), QStringLiteral("task manager")));
    }

    void aHalfTypedNameStillFindsIt()
    {
        QVERIFY(SearchModel::score(taskManager(), QStringLiteral("task man")) > 0);
    }

    void nothingMatchesNonsense()
    {
        QCOMPARE(SearchModel::score(taskManager(), QStringLiteral("defragment")), 0);
    }

    // Someone who knows the Windows chord types the chord, not the name.
    void anAliasCanBeAShortcut()
    {
        Entry entry = taskManager();
        entry.aliases.append(QStringLiteral("Ctrl+Shift+Esc"));
        QVERIFY(SearchModel::score(entry, QStringLiteral("ctrl+shift+esc")) > 0);
    }

    // The dataset is the product; a broken one must fail the build, not ship.
    void theShippedPhrasebookLoads()
    {
        SearchModel phrasebook;
        QVERIFY(phrasebook.count() > 20);
    }

    // The README tells people to type these. A dataset that has quietly
    // stopped answering one of them is a broken promise, not a missing entry.
    void thePromisedExamplesAllAnswer()
    {
        const QStringList promised = {
            QStringLiteral("Task Manager"),
            QStringLiteral("Control Panel"),
            QStringLiteral("Win+E"),
            QStringLiteral("Windows key"),
        };
        SearchModel phrasebook;
        for (const QString &term : promised) {
            phrasebook.setSearch(term);
            QVERIFY2(phrasebook.count() > 0, qPrintable(term));
        }
    }

    void searchingNarrowsTheList()
    {
        SearchModel phrasebook;
        const int all = phrasebook.count();
        phrasebook.setSearch(QStringLiteral("task manager"));
        QVERIFY(phrasebook.count() < all);
        QVERIFY(phrasebook.count() > 0);
        QCOMPARE(phrasebook.data(phrasebook.index(0, 0), EntryModel::WindowsRole).toString(),
                 QStringLiteral("Task Manager"));
    }
};

QTEST_GUILESS_MAIN(SearchTest)
#include "searchtest.moc"
