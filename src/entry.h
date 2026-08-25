// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QString>
#include <QStringList>

/**
 * One phrasebook entry: a thing a Windows user knows the name of, and what it
 * is called here.
 *
 * `desktopIds` is a list because the same application is packaged under
 * different ids on different systems -- Firefox is `firefox.desktop` on one
 * machine, `firefox_firefox.desktop` on a snap, `org.mozilla.firefox` on a
 * Flatpak -- and an entry that names only one of them is wrong everywhere
 * else. The first installed candidate is the answer; failing that, the first
 * is what Discover is asked for.
 *
 * A shortcut is deliberately not stored. Plasma's shortcuts are configurable
 * and distributions change the defaults, so printing one from a data file is
 * how a phrasebook starts lying; `Shortcuts` looks up the live binding
 * instead, which is why an entry carries where to look rather than the answer.
 */
struct Entry {
    QString id;
    QString windows;            //< "Task Manager"
    QStringList aliases;        //< "taskmgr", "Ctrl+Shift+Esc", "end task"
    QString kde;                //< "System Monitor"
    QString summary;            //< a sentence or two, in plain words
    QString category;
    QStringList desktopIds;     //< candidates; the first installed one wins
    QString iconName;           //< overrides the application's own icon
    QString shortcutDesktopId;  //< where its launch shortcut is defined
    QString shortcutActionName; //< a named action of that service, not its launch
    QString actionComponent;    //< kglobalshortcutsrc group, for non-launch actions
    QString actionName;
};
