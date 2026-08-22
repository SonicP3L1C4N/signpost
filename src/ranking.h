// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include <QString>

struct Entry;

/**
 * How well an entry answers what was typed.
 *
 * Lives apart from the model because two things ask it now: the window, which
 * sorts the whole phrasebook by it, and the KRunner plugin, which has no model
 * at all and only wants the few entries worth interrupting the user with.
 */
namespace Ranking
{
/** 0 is no match; 100 is the Windows name typed exactly. */
int score(const Entry &entry, const QString &needle);

/**
 * Worth putting in front of someone who did not ask for us.
 *
 * KRunner sees every keystroke aimed at anything, so a summary mentioning the
 * word is not enough: only a name or an alias earns a row there.
 */
constexpr int UnpromptedThreshold = 50;
}
