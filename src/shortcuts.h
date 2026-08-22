// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QString>

struct Entry;

/**
 * What the shortcut actually is on *this* machine.
 *
 * Two files answer that, in two different formats. `kglobalaccel/*.desktop`
 * under the data dirs carries what an application shipped as its launch
 * shortcut, comma-separated; `kglobalshortcutsrc` carries what the user has
 * since changed it to, as "current,default,description" where current may hold
 * several tab-separated alternatives. The user's file wins where it says
 * anything at all.
 */
namespace Shortcuts
{
/** The launch shortcut for a .desktop id, or "" when it has none. */
QString forLaunch(const QString &desktopId);

/** A non-launch action, e.g. component "kwin", name "Show Desktop". */
QString forAction(const QString &component, const QString &name);

/** Whichever of the two an entry asks for. */
QString forEntry(const Entry &entry);

/** "Meta+Tab\tAlt+Tab,Meta+Tab\tAlt+Tab,Walk Through Windows" -> "Meta+Tab". */
QString fromGlobalShortcutsValue(const QString &value);

/**
 * "Display,Meta+P" -> "Meta+P".
 *
 * The list mixes chords with media keys ("Calculator", "Search", "Display"),
 * and a chord is the useful answer to "what do I press", so a chord wins when
 * the list has one.
 */
QString fromDesktopShortcutsValue(const QString &value);
}
