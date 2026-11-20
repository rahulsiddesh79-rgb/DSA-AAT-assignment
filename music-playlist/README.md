# Music Playlist — Circular Doubly Linked List

Course: **1BAI303 Data Structures and Applications** · Capstone AAT  
Language: **C11** · Core structure: **circular doubly linked list** (handwritten; no library lists)

## Problem

A music player must support **next**, **previous**, and **repeat-all** without scanning the whole playlist each time. Songs are also added and removed while the user is listening.

This program models the playlist as a **circular doubly linked list**. Playback is a pointer (`current`) that walks `next` / `prev`. Repeat-all is the circular invariant: the last song’s `next` is the first song.

## Why this data structure (and why not another)

| Structure | Fit for a playlist |
|-----------|-------------------|
| **Circular doubly linked list** | Next and previous are O(1). Wrap-around for repeat-all is free. Insert/delete in the middle is O(1) once you have the node. |
| Singly linked list | Previous needs a scan from the head. |
| Array | Next/prev are easy, but insert/delete in the middle shifts O(n) songs. Wrap-around needs extra index math; the structure is not naturally circular. |
| Stack / queue | One-way only; cannot walk both directions. |
| BST / hash table | Fast search, but no inherent play order. |

The list is the heart of the program: every menu action calls an insert, unlink, search, or pointer move on the ring.

## Operations and time complexity

`n` = number of songs. Tail is `head->prev`, so end-insert does not walk the list.

| Operation | What it does | Time |
|-----------|----------------|------|
| Insert at end | Link after tail | O(1) |
| Insert at beginning | Link before head, then `head = new` | O(1) |
| Insert after current | `link_after(current, node)` | O(1) |
| Delete by id | Search then unlink four pointers | O(n) search, O(1) unlink |
| Play next / previous | `current = current->next` / `prev` | O(1) |
| Repeat current | Stay on `current` | O(1) |
| Toggle Repeat All | Flag only; the ring is always circular | O(1) |
| Display playlist | Walk from `head` until back | O(n) |
| Search by title | Walk the ring | O(n) |
| Count songs | Stored `size` | O(1) |
| Clear playlist | Delete every node | O(n) |

**Edge cases handled:** empty list; one song (`next` and `prev` both point to itself); deleting the only song; deleting the current song (current moves to `next`); deleting `head`; duplicate titles allowed (ids are unique); invalid id / empty title / bad duration / invalid menu choice; Repeat All OFF stops at the last (next) or first (previous) song.

## Build and run

Needs `gcc`.

```text
cd music-playlist
make
./playlist          # loads 4 demo songs
./playlist --empty  # start with no songs
```

On Windows without `make`:

```text
gcc -std=c11 -Wall -Wextra -o playlist.exe playlist.c
.\playlist.exe
```

## Sample run

```text
Loaded 4 demo songs. Repeat All is ON.
Playlist (4 songs). '>' is now playing.
  > [1] "Blinding Lights" — The Weeknd (200 sec)
    [2] "Levitating" — Dua Lipa (203 sec)
    [3] "Peaches" — Justin Bieber (198 sec)
    [4] "Heat Waves" — Glass Animals (238 sec)
Now playing: [1] "Blinding Lights" — The Weeknd (200 sec)  [Repeat All ON]
```

Demo path for the viva: **13** (load demo) → **5** / **6** (next/prev) → **3** (insert after current) → **8** then **5** until wrap is blocked → **4** (delete) → **10** (search).

Capture screenshots of this session for the PPT.

## Team members

Fill in before GitHub submit.

| Name | USN | Role |
|------|-----|------|
| RAHUL.S     | 1DS25AI083    | Backend development,data structure and algorithm

|      |     | |
|      |     | |

## Files

- `playlist.c` — node, all list operations, menu
- `Makefile` — `gcc -std=c11 -Wall -Wextra`
- `AI_LOG.md` — AI usage log (assignment requirement)
- `slides.md` — 8 slides to paste into PowerPoint
