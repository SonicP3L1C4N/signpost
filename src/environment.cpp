// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "environment.h"

#include <kcoreaddons_version.h>

#include <QClipboard>
#include <QGuiApplication>
#include <QProcess>
#include <QStandardPaths>

namespace
{
QString runPlasmashell()
{
    const QString plasmashell = QStandardPaths::findExecutable(QStringLiteral("plasmashell"));
    if (plasmashell.isEmpty()) {
        return {};
    }
    QProcess process;
    process.start(plasmashell, {QStringLiteral("--version")});
    if (!process.waitForFinished(5000) || process.exitStatus() != QProcess::NormalExit
        || process.exitCode() != 0) {
        return {};
    }
    // "plasmashell 6.6.6" -- the number is the last field.
    const QString output = QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();
    return output.section(QLatin1Char(' '), -1);
}
}

Environment::Environment(QObject *parent)
    : QObject(parent)
{
}

QString Environment::version() const
{
    return QStringLiteral(SIGNPOST_VERSION);
}

QString Environment::qtVersion() const
{
    return QString::fromLatin1(qVersion());
}

QString Environment::frameworksVersion() const
{
    return QStringLiteral(KCOREADDONS_VERSION_STRING);
}

QString Environment::plasmaVersion() const
{
    // Asked once and remembered: it costs a process, and the answer cannot
    // change while the window is open.
    static const QString version = runPlasmashell();
    return version;
}

QString Environment::session() const
{
    const QString desktop = qEnvironmentVariable("XDG_CURRENT_DESKTOP");
    const QString type = qEnvironmentVariable("XDG_SESSION_TYPE");
    const QString both = QStringLiteral("%1 %2").arg(desktop, type).trimmed();
    return both.isEmpty() ? QStringLiteral("unknown") : both;
}

QString Environment::homepage() const
{
    return QStringLiteral("https://github.com/SonicP3L1C4N/signpost");
}

QString Environment::bugAddress() const
{
    return homepage() + QStringLiteral("/issues");
}

QString Environment::report(int entries) const
{
    const QList<QPair<QString, QString>> rows = {
        {QStringLiteral("Signpost"), version()},
        {QStringLiteral("Entries"), QString::number(entries)},
        {QStringLiteral("Plasma"), plasmaVersion().isEmpty()
                                       ? QStringLiteral("not detected")
                                       : plasmaVersion()},
        {QStringLiteral("Session"), session()},
        {QStringLiteral("Qt"), qtVersion()},
        {QStringLiteral("Frameworks"), frameworksVersion()},
    };

    QString report;
    for (const auto &[name, value] : rows) {
        report += QStringLiteral("%1%2\n").arg(name, -13).arg(value);
    }
    return report.trimmed();
}

void Environment::copyReport(int entries) const
{
    QGuiApplication::clipboard()->setText(report(entries));
}
