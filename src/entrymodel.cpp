// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "entrymodel.h"
#include "shortcuts.h"

#include <KService>

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
Entry readEntry(const QJsonObject &object)
{
    Entry entry;
    entry.id = object.value(QLatin1String("id")).toString();
    entry.windows = object.value(QLatin1String("windows")).toString();
    entry.kde = object.value(QLatin1String("kde")).toString();
    entry.summary = object.value(QLatin1String("summary")).toString();
    entry.category = object.value(QLatin1String("category")).toString();
    entry.desktopId = object.value(QLatin1String("desktop")).toString();
    entry.shortcutDesktopId = object.value(QLatin1String("shortcutDesktop")).toString();

    const QJsonArray aliases = object.value(QLatin1String("aliases")).toArray();
    for (const QJsonValue &alias : aliases) {
        entry.aliases.append(alias.toString());
    }

    const QJsonObject action = object.value(QLatin1String("action")).toObject();
    entry.actionComponent = action.value(QLatin1String("component")).toString();
    entry.actionName = action.value(QLatin1String("name")).toString();

    return entry;
}
}

EntryModel::EntryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

bool EntryModel::load(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "signpost: cannot read the phrasebook at" << path
                   << file.errorString();
        return false;
    }

    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "signpost: the phrasebook is not valid JSON:" << error.errorString();
        return false;
    }

    const QJsonArray entries = document.object().value(QLatin1String("entries")).toArray();

    beginResetModel();
    m_entries.clear();
    m_shortcuts.clear();
    m_installed.clear();
    m_entries.reserve(entries.size());
    for (const QJsonValue &value : entries) {
        const Entry entry = readEntry(value.toObject());
        if (entry.windows.isEmpty() || entry.kde.isEmpty()) {
            qWarning() << "signpost: skipping an entry with no Windows or KDE name:"
                       << entry.id;
            continue;
        }
        m_entries.append(entry);
        m_shortcuts.append(Shortcuts::forEntry(entry));
        // An entry with nothing to launch counts as present: the answer is
        // "it is in Dolphin" or "there is no such thing here", not an install.
        const KService::Ptr service = entry.desktopId.isEmpty()
            ? KService::Ptr()
            : KService::serviceByStorageId(entry.desktopId);
        m_installed.append(entry.desktopId.isEmpty() || bool(service));
    }
    endResetModel();

    return true;
}

int EntryModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_entries.count();
}

QVariant EntryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_entries.count()) {
        return {};
    }
    const Entry &entry = m_entries.at(index.row());
    switch (role) {
    case IdRole:
        return entry.id;
    case WindowsRole:
    case Qt::DisplayRole:
        return entry.windows;
    case KdeRole:
        return entry.kde;
    case SummaryRole:
        return entry.summary;
    case CategoryRole:
        return entry.category;
    case ShortcutRole:
        return m_shortcuts.at(index.row());
    case DesktopIdRole:
        return entry.desktopId;
    case InstalledRole:
        return m_installed.at(index.row());
    default:
        return {};
    }
}

QHash<int, QByteArray> EntryModel::roleNames() const
{
    return {
        {IdRole, QByteArrayLiteral("entryId")},
        {WindowsRole, QByteArrayLiteral("windowsName")},
        {KdeRole, QByteArrayLiteral("kdeName")},
        {SummaryRole, QByteArrayLiteral("summary")},
        {CategoryRole, QByteArrayLiteral("category")},
        {ShortcutRole, QByteArrayLiteral("shortcut")},
        {DesktopIdRole, QByteArrayLiteral("desktopId")},
        {InstalledRole, QByteArrayLiteral("installed")},
    };
}

const Entry &EntryModel::entryAt(int row) const
{
    return m_entries.at(row);
}

QString EntryModel::shortcutAt(int row) const
{
    return m_shortcuts.at(row);
}
