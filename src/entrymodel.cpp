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
/**
 * The mark on the Windows side of a card.
 *
 * Our own artwork rather than Microsoft's: their icons are theirs, and a
 * phrasebook that shipped them could not be distributed by anyone. These are
 * generic, and grey on purpose -- the card reads left to right as the thing
 * you knew giving way to the thing in front of you.
 */
QString originIcon(const QString &category)
{
    static const QStringList known = {
        QStringLiteral("Apps"),      QStringLiteral("Files"),
        QStringLiteral("Internet"),  QStringLiteral("Settings"),
        QStringLiteral("Shortcuts"), QStringLiteral("System"),
    };
    const QString name = known.contains(category) ? category.toLower()
                                                  : QStringLiteral("apps");
    return QStringLiteral("qrc:/origin/origin-%1.svg").arg(name);
}

Entry readEntry(const QJsonObject &object)
{
    Entry entry;
    entry.id = object.value(QLatin1String("id")).toString();
    entry.windows = object.value(QLatin1String("windows")).toString();
    entry.kde = object.value(QLatin1String("kde")).toString();
    entry.summary = object.value(QLatin1String("summary")).toString();
    entry.category = object.value(QLatin1String("category")).toString();
    // "desktop" is one id or several, and several is the honest case.
    const QJsonValue desktop = object.value(QLatin1String("desktop"));
    if (desktop.isArray()) {
        const QJsonArray candidates = desktop.toArray();
        for (const QJsonValue &candidate : candidates) {
            entry.desktopIds.append(candidate.toString());
        }
    } else if (!desktop.toString().isEmpty()) {
        entry.desktopIds.append(desktop.toString());
    }
    entry.iconName = object.value(QLatin1String("icon")).toString();
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

        KService::Ptr service;
        QString resolved;
        for (const QString &candidate : std::as_const(entry.desktopIds)) {
            service = KService::serviceByStorageId(candidate);
            if (service) {
                resolved = candidate;
                break;
            }
        }
        // Nothing installed: keep the first candidate, which is what the Get
        // it button hands to Discover.
        m_resolved.append(resolved.isEmpty() ? entry.desktopIds.value(0) : resolved);
        // An entry with nothing to launch counts as present: the answer is
        // "it is in Dolphin" or "there is no such thing here", not an install.
        m_installed.append(entry.desktopIds.isEmpty() || bool(service));

        // The application's own icon says more than any category glyph could;
        // an entry with no application names one itself.
        QString icon = entry.iconName;
        if (icon.isEmpty() && service) {
            icon = service->icon();
        }
        m_icons.append(icon.isEmpty() ? QStringLiteral("help-about") : icon);
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
        return m_resolved.at(index.row());
    case IconRole:
        return m_icons.at(index.row());
    case OriginIconRole:
        return originIcon(entry.category);
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
        {IconRole, QByteArrayLiteral("iconName")},
        {OriginIconRole, QByteArrayLiteral("originIcon")},
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
