// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "entry.h"

#include <QAbstractListModel>

/**
 * The phrasebook, as loaded from the JSON dataset.
 *
 * Shortcuts and "is it installed" are resolved once at load rather than per
 * delegate: both go to disk, and a list view asks for the same row many times.
 */
class EntryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role {
        IdRole = Qt::UserRole + 1,
        WindowsRole,
        KdeRole,
        SummaryRole,
        CategoryRole,
        ShortcutRole,
        DesktopIdRole,
        IconRole,
        OriginIconRole,
        InstalledRole,
    };
    Q_ENUM(Role)

    explicit EntryModel(QObject *parent = nullptr);

    /** Reads the dataset. Returns false, and explains on stderr, if it cannot. */
    bool load(const QString &path);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    const Entry &entryAt(int row) const;
    QString shortcutAt(int row) const;

private:
    QList<Entry> m_entries;
    QStringList m_shortcuts;
    QStringList m_resolved;   //< the candidate that turned out to be installed
    QStringList m_icons;
    QList<bool> m_installed;
};
