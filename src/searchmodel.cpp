// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "searchmodel.h"
#include "entry.h"
#include "entrymodel.h"

SearchModel::SearchModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_entries(new EntryModel(this))
{
    m_entries->load(QStringLiteral(":/entries.json"));
    setSourceModel(m_entries);
    setDynamicSortFilter(true);
    sort(0);

    connect(this, &QAbstractItemModel::rowsInserted, this, &SearchModel::countChanged);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &SearchModel::countChanged);
    connect(this, &QAbstractItemModel::modelReset, this, &SearchModel::countChanged);
}

QString SearchModel::search() const
{
    return m_search;
}

void SearchModel::setSearch(const QString &search)
{
    const QString trimmed = search.trimmed();
    if (trimmed == m_search) {
        return;
    }
    m_search = trimmed;
    invalidate();
    Q_EMIT searchChanged();
    Q_EMIT countChanged();
}

QString SearchModel::category() const
{
    return m_category;
}

void SearchModel::setCategory(const QString &category)
{
    if (category == m_category) {
        return;
    }
    m_category = category;
    invalidate();
    Q_EMIT categoryChanged();
    Q_EMIT countChanged();
}

QStringList SearchModel::categories() const
{
    QStringList found;
    for (int row = 0; row < m_entries->rowCount(); ++row) {
        const QString category = m_entries->entryAt(row).category;
        if (!category.isEmpty() && !found.contains(category)) {
            found.append(category);
        }
    }
    found.sort();
    return found;
}

int SearchModel::count() const
{
    return rowCount();
}

int SearchModel::total() const
{
    return m_entries->rowCount();
}

int SearchModel::score(const Entry &entry, const QString &needle)
{
    if (needle.isEmpty()) {
        return 1;
    }

    const QString wanted = needle.toCaseFolded();
    const auto has = [&wanted](const QString &text) {
        return text.toCaseFolded().contains(wanted);
    };
    const auto is = [&wanted](const QString &text) {
        return text.toCaseFolded() == wanted;
    };

    if (is(entry.windows)) {
        return 100;
    }
    if (entry.windows.toCaseFolded().startsWith(wanted)) {
        return 80;
    }
    for (const QString &alias : entry.aliases) {
        if (is(alias)) {
            return 70;
        }
    }
    if (has(entry.windows)) {
        return 60;
    }
    for (const QString &alias : entry.aliases) {
        if (has(alias)) {
            return 50;
        }
    }
    if (has(entry.kde)) {
        return 30;
    }
    if (has(entry.category)) {
        return 20;
    }
    if (has(entry.summary)) {
        return 10;
    }
    return 0;
}

bool SearchModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    const Entry &entry = m_entries->entryAt(row);
    if (!m_category.isEmpty() && entry.category != m_category) {
        return false;
    }
    return score(entry, m_search) > 0;
}

bool SearchModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const Entry &first = m_entries->entryAt(left.row());
    const Entry &second = m_entries->entryAt(right.row());

    if (!m_search.isEmpty()) {
        const int firstScore = score(first, m_search);
        const int secondScore = score(second, m_search);
        if (firstScore != secondScore) {
            return firstScore > secondScore;
        }
    }
    // Alphabetical within a score, so an empty search reads like a glossary.
    return first.windows.localeAwareCompare(second.windows) < 0;
}
