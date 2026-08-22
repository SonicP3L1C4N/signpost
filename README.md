<!--
SPDX-FileCopyrightText: 2026 Gary Bissett <gary.bissett@gmail.com>

SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
-->

# Signpost

**What things are called on KDE, when you have come from Windows.**

Type the word you already know — `Task Manager`, `Control Panel`, `Win+E` — and
Signpost tells you what it is here, what the shortcut is on *this* machine, and
opens it for you.

It is for the week after the switch. The install went fine, the files came
across, the welcome tour has been dismissed, and now every third thing you want
to do begins with not knowing what it is called.

## Why an application and not a wiki page

Two reasons, and they are the whole design.

**It opens the thing.** A table of equivalents tells you that Task Manager is
System Monitor and leaves you to go and find it. Signpost has a button.

**It reads your machine rather than quoting a document.** Shortcuts are
configurable, and distributions change the defaults, so a phrasebook that prints
`Meta+E` from a data file is one distribution decision away from lying. Signpost
looks the binding up when it loads: `X-KDE-Shortcuts` from the
`kglobalaccel/*.desktop` files for launch shortcuts, and `kglobalshortcutsrc` for
what you have since changed and for KWin's own actions. If you have rebound
Overview to something else, that is what it shows you.

The same goes for the answer itself: an entry whose application is not installed
says so, and offers Discover instead of pretending.

## Status

Early. The first cut is the search, the dataset and the two buttons. The
dataset is 34 entries and is the part that needs the most work — see below.

## Building

Kubuntu/Debian:

```
sudo apt install build-essential cmake ninja-build extra-cmake-modules \
    qt6-base-dev qt6-declarative-dev libkirigami-dev kirigami-addons-dev \
    libkf6coreaddons-dev libkf6i18n-dev libkf6config-dev libkf6service-dev \
    libkf6kio-dev gettext

cmake -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
./build/bin/signpost
```

## The dataset

`data/entries.json`, compiled into the binary. One entry looks like this:

```json
{
  "id": "task-manager",
  "windows": "Task Manager",
  "aliases": ["taskmgr", "Ctrl+Shift+Esc", "end task"],
  "kde": "System Monitor",
  "summary": "The same job with more of it: processes, CPU, memory...",
  "category": "System",
  "desktop": "org.kde.plasma-systemmonitor.desktop"
}
```

`desktop` is what the Open button starts, and where the launch shortcut is
looked up. For a thing with no application behind it, `action` names a
`kglobalshortcutsrc` entry instead — `{"component": "kwin", "name": "Show
Desktop"}` — and `shortcutDesktop` covers the few whose shortcut is defined
under a different id from the one that launches.

Three rules for a new entry:

1. **Use the Windows word.** The entry is found by what the reader already
   calls it, so `aliases` should carry the wrong-but-common names too:
   `regedit`, `netplwiz`, `my computer`, the chord they used to press.
2. **Check the `.desktop` id exists** before adding it. A wrong id makes the
   entry look uninstalled forever.
3. **Say when the answer is "there isn't one".** OneDrive has no client;
   there is no registry; no antivirus is shipped. A phrasebook that invents an
   equivalent to avoid an awkward answer is worse than no phrasebook.

Entries are written from scratch rather than copied — a list of facts we
compose ourselves, so the dataset carries the project's own licence rather
than inheriting one.

## Licence

GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL, which is KDE's
preferred expression for application code — the project is written to be
proposable upstream. REUSE-compliant; every file says so itself.
