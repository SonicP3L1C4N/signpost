// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QString>

struct Entry;

/**
 * What the shortcut actually is on *this* machine.
 *
 * Two files answer that, in two different formats. The `kglobalaccel`
 * desktop files under the data dirs carry what an application shipped as its launch
 * shortcut, comma-separated; `kglobalshortcutsrc` carries what the user has
 * since changed it to, as "current,default,description" where current may hold
 * several tab-separated alternatives. The user's file wins where it says
 * anything at all.
 */
namespace Shortcuts
{
/** The launch shortcut for a .desktop id, or "" when it has none. */
QString forLaunch(const QString &desktopId);

/**
 * A named action of a service -- "ShowOSD" of `org.kde.kscreen.desktop` -- or
 * the launch shortcut when the name is empty.
 *
 * Not every shortcut a service ships starts the service. kscreen has no launch
 * shortcut at all; Meta+P belongs to its Switch Display action and lives in a
 * `[Desktop Action ShowOSD]` group, which is why an entry can say which action
 * it means rather than getting nothing.
 *
 * A binding the user has taken away is an answer, not a gap: where their file
 * carries the action at all, it is what this returns, empty or not.
 */
QString forService(const QString &desktopId, const QString &actionName);

/** A non-launch action, e.g. component "kwin", name "Show Desktop". */
QString forAction(const QString &component, const QString &name);

/** Whichever of the two an entry asks for. */
QString forEntry(const Entry &entry);

/** "Meta+Tab\tAlt+Tab,Meta+Tab\tAlt+Tab,Walk Through Windows" -> "Meta+Tab". */
QString fromGlobalShortcutsValue(const QString &value);

/**
 * "Display,Meta+P" -> "Meta+P".
 *
 * The list mixes chords with media keys ("Calculator", "Search", "Display").
 * Only a chord answers "what do I press", so a media key on its own counts as
 * no shortcut at all.
 */
QString fromDesktopShortcutsValue(const QString &value);
}
