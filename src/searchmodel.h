// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QQmlEngine>
#include <QSortFilterProxyModel>

class EntryModel;
struct Entry;

/**
 * The phrasebook, searched.
 *
 * Someone typing "task manager" wants the Task Manager row first, not the row
 * whose summary happens to mention task management, so matches are scored
 * rather than merely filtered: the Windows name outranks an alias, an alias
 * outranks the KDE name, and the summary comes last.
 */
class SearchModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Phrasebook)

    Q_PROPERTY(QString search READ search WRITE setSearch NOTIFY searchChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit SearchModel(QObject *parent = nullptr);

    QString search() const;
    void setSearch(const QString &search);

    int count() const;

    /** 0 means "no match"; higher is a better one. Public so tests can rank. */
    static int score(const Entry &entry, const QString &needle);

Q_SIGNALS:
    void searchChanged();
    void countChanged();

protected:
    bool filterAcceptsRow(int row, const QModelIndex &parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    EntryModel *m_entries;
    QString m_search;
};
