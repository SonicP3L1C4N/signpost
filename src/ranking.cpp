// SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "ranking.h"
#include "entry.h"

int Ranking::score(const Entry &entry, const QString &needle)
{
    if (needle.isEmpty()) {
        return 1;
    }

    const QString wanted = needle.toCaseFolded();
    const auto has = [&wanted](const QString &text) {
        return text.toCaseFolded().contains(wanted);
    };
    const auto is = [&wanted](const QString &text) {
        return text.toCaseFolded() == wanted;
    };

    if (is(entry.windows)) {
        return 100;
    }
    if (entry.windows.toCaseFolded().startsWith(wanted)) {
        return 80;
    }
    for (const QString &alias : entry.aliases) {
        if (is(alias)) {
            return 70;
        }
    }
    if (has(entry.windows)) {
        return 60;
    }
    for (const QString &alias : entry.aliases) {
        if (has(alias)) {
            return 50;
        }
    }
    if (has(entry.kde)) {
        return 30;
    }
    if (has(entry.category)) {
        return 20;
    }
    if (has(entry.summary)) {
        return 10;
    }
    return 0;
}
