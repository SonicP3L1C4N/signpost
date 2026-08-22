// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "signpostrunner.h"

#include "entry.h"
#include "ranking.h"

#include <KIO/ApplicationLauncherJob>
#include <KLocalizedString>
#include <KService>

#include <QProcess>
#include <QStandardPaths>

namespace
{
constexpr int MaximumMatches = 6;

/** The window, opened on this entry, for the answers with nothing to launch. */
void explain(const QString &windowsName)
{
    const QString signpost = QStandardPaths::findExecutable(QStringLiteral("signpost"));
    if (signpost.isEmpty()) {
        return;
    }
    QProcess::startDetached(signpost, {windowsName});
}
}

SignpostRunner::SignpostRunner(QObject *parent, const KPluginMetaData &metaData)
    : KRunner::AbstractRunner(parent, metaData)
{
    m_phrasebook.load(QStringLiteral(":/entries.json"));
}

void SignpostRunner::match(KRunner::RunnerContext &context)
{
    const QString query = context.query().trimmed();
    if (query.length() < 3) {
        return;
    }

    QList<QPair<int, int>> hits;   // score, row
    for (int row = 0; row < m_phrasebook.rowCount(); ++row) {
        const int score = Ranking::score(m_phrasebook.entryAt(row), query);
        if (score >= Ranking::UnpromptedThreshold) {
            hits.append({score, row});
        }
    }
    if (hits.isEmpty()) {
        return;
    }

    std::sort(hits.begin(), hits.end(), [](const auto &first, const auto &second) {
        return first.first > second.first;
    });
    hits = hits.mid(0, MaximumMatches);

    for (const auto &[score, row] : std::as_const(hits)) {
        const Entry &entry = m_phrasebook.entryAt(row);
        const QString shortcut = m_phrasebook.shortcutAt(row);
        const QModelIndex index = m_phrasebook.index(row);

        KRunner::QueryMatch match(this);
        match.setId(entry.id);
        // The KDE name leads: it is the thing being looked for, and the
        // Windows word is already in front of the person who typed it.
        match.setText(entry.kde);
        match.setSubtext(shortcut.isEmpty()
                             ? i18n("%1 on Windows", entry.windows)
                             : i18n("%1 on Windows · %2", entry.windows, shortcut));
        match.setIconName(index.data(EntryModel::IconRole).toString());
        match.setRelevance(score / 100.0);
        match.setData(QVariantMap{
            {QStringLiteral("desktop"), index.data(EntryModel::DesktopIdRole)},
            {QStringLiteral("installed"), index.data(EntryModel::InstalledRole)},
            {QStringLiteral("windows"), entry.windows},
        });
        context.addMatch(match);
    }
}

void SignpostRunner::run(const KRunner::RunnerContext &context,
                         const KRunner::QueryMatch &match)
{
    Q_UNUSED(context)

    const QVariantMap data = match.data().toMap();
    const QString desktopId = data.value(QStringLiteral("desktop")).toString();
    const bool installed = data.value(QStringLiteral("installed")).toBool();

    // Nothing to launch, or nothing installed to launch: the answer itself is
    // what was wanted, so open the window on it rather than failing quietly.
    if (desktopId.isEmpty() || !installed) {
        explain(data.value(QStringLiteral("windows")).toString());
        return;
    }

    const KService::Ptr service = KService::serviceByStorageId(desktopId);
    if (!service) {
        explain(data.value(QStringLiteral("windows")).toString());
        return;
    }
    auto *job = new KIO::ApplicationLauncherJob(service);
    job->start();
}

K_PLUGIN_CLASS_WITH_JSON(SignpostRunner, "signpostrunner.json")

#include "signpostrunner.moc"
