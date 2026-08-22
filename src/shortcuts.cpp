// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "shortcuts.h"
#include "entry.h"

#include <KConfig>
#include <KConfigGroup>

#include <QStandardPaths>

namespace
{
/** A chord has a modifier in it; "Calculator" and "Search" are media keys. */
bool looksLikeAChord(const QString &candidate)
{
    return candidate.contains(QLatin1Char('+'));
}
}

QString Shortcuts::fromGlobalShortcutsValue(const QString &value)
{
    if (value.isEmpty()) {
        return {};
    }
    // "current,default,description" -- only the first field is what is bound.
    const QString current = value.section(QLatin1Char(','), 0, 0);
    if (current.isEmpty() || current == QLatin1String("none")) {
        return {};
    }
    // Several alternatives may be bound at once; the first is the one to show.
    return current.section(QLatin1Char('\t'), 0, 0).trimmed();
}

QString Shortcuts::fromDesktopShortcutsValue(const QString &value)
{
    const QStringList candidates = value.split(QLatin1Char(','), Qt::SkipEmptyParts);
    for (const QString &candidate : candidates) {
        if (looksLikeAChord(candidate)) {
            return candidate.trimmed();
        }
    }
    // Only a media key is left -- "Calculator", "Search", "Tools". Those are
    // real bindings, but a chip reading "Calculator" answers a question nobody
    // asked; "what do I press" wants a chord or nothing.
    return {};
}

QString Shortcuts::forLaunch(const QString &desktopId)
{
    if (desktopId.isEmpty()) {
        return {};
    }

    KConfig config(QStringLiteral("kglobalshortcutsrc"), KConfig::NoGlobals);
    const KConfigGroup services = config.group(QStringLiteral("services"));
    if (services.hasGroup(desktopId)) {
        const QString changed = fromGlobalShortcutsValue(
            services.group(desktopId).readEntry(QStringLiteral("_launch"), QString()));
        if (!changed.isEmpty()) {
            return changed;
        }
    }

    const QString shipped = QStandardPaths::locate(
        QStandardPaths::GenericDataLocation,
        QStringLiteral("kglobalaccel/") + desktopId);
    if (shipped.isEmpty()) {
        return {};
    }

    KConfig desktopFile(shipped, KConfig::SimpleConfig);
    return fromDesktopShortcutsValue(desktopFile.group(QStringLiteral("Desktop Entry"))
                                         .readEntry(QStringLiteral("X-KDE-Shortcuts"), QString()));
}

QString Shortcuts::forAction(const QString &component, const QString &name)
{
    if (component.isEmpty() || name.isEmpty()) {
        return {};
    }
    KConfig config(QStringLiteral("kglobalshortcutsrc"), KConfig::NoGlobals);
    return fromGlobalShortcutsValue(
        config.group(component).readEntry(name, QString()));
}

QString Shortcuts::forEntry(const Entry &entry)
{
    if (!entry.actionName.isEmpty()) {
        return forAction(entry.actionComponent, entry.actionName);
    }
    if (!entry.shortcutDesktopId.isEmpty()) {
        return forLaunch(entry.shortcutDesktopId);
    }
    // Whichever candidate is installed is the one whose shortcut applies.
    for (const QString &candidate : entry.desktopIds) {
        const QString shortcut = forLaunch(candidate);
        if (!shortcut.isEmpty()) {
            return shortcut;
        }
    }
    return {};
}
