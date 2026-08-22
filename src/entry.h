// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QString>
#include <QStringList>

/**
 * One phrasebook entry: a thing a Windows user knows the name of, and what it
 * is called here.
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
    QString desktopId;          //< the application to open, if there is one
    QString shortcutDesktopId;  //< where its launch shortcut is defined
    QString actionComponent;    //< kglobalshortcutsrc group, for non-launch actions
    QString actionName;
};
