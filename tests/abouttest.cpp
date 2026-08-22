// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "environment.h"

#include <QTest>

class AboutTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void itKnowsWhichVersionItIs()
    {
        Environment environment;
        QVERIFY(!environment.version().isEmpty());
        QVERIFY(environment.report(101).contains(environment.version()));
    }

    // The count is the one number that says whether the dataset loaded at all,
    // which is exactly what a report about a missing answer needs.
    void theReportCarriesTheEntryCount()
    {
        Environment environment;
        QVERIFY(environment.report(101).contains(QStringLiteral("101")));
    }

    void theReportCarriesTheVersionsABugReportNeeds()
    {
        Environment environment;
        const QString report = environment.report(1);
        for (const QString &row : {QStringLiteral("Qt"), QStringLiteral("Frameworks"),
                                   QStringLiteral("Session"), QStringLiteral("Plasma")}) {
            QVERIFY2(report.contains(row), qPrintable(row));
        }
        QVERIFY(!environment.qtVersion().isEmpty());
        QVERIFY(!environment.frameworksVersion().isEmpty());
    }

    // No plasmashell to ask is a normal state -- over ssh, in a test run, or
    // while the shell is restarting -- and says so rather than showing a gap.
    void aMissingPlasmaIsSaidRatherThanLeftBlank()
    {
        Environment environment;
        if (environment.plasmaVersion().isEmpty()) {
            QVERIFY(environment.report(1).contains(QStringLiteral("not detected")));
        } else {
            QVERIFY(environment.report(1).contains(environment.plasmaVersion()));
        }
    }
};

QTEST_GUILESS_MAIN(AboutTest)
#include "abouttest.moc"
