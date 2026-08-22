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
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QStringList categories READ categories CONSTANT)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int total READ total CONSTANT)

public:
    explicit SearchModel(QObject *parent = nullptr);

    QString search() const;
    void setSearch(const QString &search);

    /** "" means every category. */
    QString category() const;
    void setCategory(const QString &category);

    /** Every category in the dataset, in the order they should be offered. */
    QStringList categories() const;

    int count() const;
    int total() const;

    /** 0 means "no match"; higher is a better one. Public so tests can rank. */
    static int score(const Entry &entry, const QString &needle);

Q_SIGNALS:
    void searchChanged();
    void categoryChanged();
    void countChanged();

protected:
    bool filterAcceptsRow(int row, const QModelIndex &parent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    EntryModel *m_entries;
    QString m_search;
    QString m_category;
};
