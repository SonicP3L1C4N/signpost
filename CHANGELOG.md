<!--
SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>

SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
-->

# Changelog

## Unreleased

- **A KRunner plugin.** Type the Windows word into KRunner and the KDE name is
  in the list, with the Windows term and the live shortcut underneath it. Enter
  starts the application; where there is nothing to start, it opens Signpost on
  the explanation. Only names and aliases match there, and queries under three
  letters are ignored — KRunner belongs to every runner at once.
- **Meta+/** launches Signpost, shipped as a `kglobalaccel` entry — the same
  mechanism Signpost reads to answer questions about shortcuts, so it now
  answers for itself and reports the binding if you change it.
- The runner loads and translates on the main thread. A runner is moved to its
  own thread after construction, and both KSycoca and KLocalizedString parent
  objects to the application from whichever thread touches them first — so the
  phrasebook is read and its subtexts translated up front, and matching does
  nothing but look things up.
- Ranking moved into `src/ranking.{h,cpp}`: the window and the runner both ask
  the same question and neither should own the answer.

## 0.1.0 — 2026-08-22

First release, and an early one: the shape of the thing is settled, the dataset
is not finished and will not be for a while.

### The phrasebook

- **101 entries**, each pairing the Windows word with what it is called here,
  in six categories: Apps, Files, Internet, Settings, Shortcuts and System.
- **Scored search** rather than filtering. Typing `task manager` puts Task
  Manager first and the entries that merely mention task management after it;
  the Windows name outranks an alias, an alias outranks the KDE name, and the
  summary comes last.
- **Aliases carry the wrong-but-common names** — `regedit`, `netplwiz`,
  `my computer`, `Win+E`, `Ctrl+Shift+Esc` — because the reader searches for
  what they already call it.
- **The honest answers are in.** There is no registry, no product key, no
  Microsoft account, nothing recording the screen, nothing to defragment, no
  OneDrive client, and `.exe` files do not run. A phrasebook that invents an
  equivalent to avoid an awkward answer is worse than no phrasebook.

### Reading the machine rather than a document

- **Shortcuts are resolved at load**, not stored: `X-KDE-Shortcuts` from the
  `kglobalaccel` desktop files for launch shortcuts, `kglobalshortcutsrc` for
  what the user has since rebound and for KWin's own actions. Rebind Overview
  and Signpost says what you rebound it to.
- **Installed state is resolved the same way.** An entry whose application is
  missing says so and offers Discover, rather than pretending.
- **An entry may name several `.desktop` candidates**, first installed one
  wins, because Firefox is `firefox.desktop` on one machine,
  `firefox_firefox.desktop` on a snap and `org.mozilla.firefox.desktop` on a
  Flatpak.

### The window

- Kirigami. Each card reads left to right — a grey mark for what you knew, then
  the real icon and name of what answers it — in a fixed column so the whole
  list reads as one translation repeated.
- Category filters and a live count in the header.
- `signpost "task manager"` opens on the answer, which is the form worth
  binding to a key or typing into KRunner.
- **About Signpost** carries a details block — version, entry count, Plasma,
  session, Qt, Frameworks — behind a button that copies it for a bug report.
- The Windows-side marks are original artwork. Microsoft's icons are
  Microsoft's, and shipping them would make this undistributable.

### Packaging

- CMake with ECM, KF6 and Qt 6.6+, installable into `~/.local` with no root.
- Icon set installed into hicolor at seven sizes; the scalable one is compiled
  in as well, so an uninstalled copy still looks like the application.
- Four test suites: search ranking, shortcut parsing, the About report, and the
  shape of every entry in the dataset — including that the four examples the
  README promises still answer.
- REUSE-compliant, under KDE's preferred licence expression for applications.
