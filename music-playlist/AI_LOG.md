# AI usage log — 1BAI303 Capstone AAT

Tool: **Cursor** (Grok 4.6)

## Key prompts

1. Shared the assignment PDF: `1BAI303 Capstone AAT(DSA).pdf`.
2. Chose application **Music playlist — next / prev / repeat** and language **C** (not C++).
3. Asked to implement the agreed plan: handwritten circular doubly linked list as the playlist engine, menu-driven program, README, this log, and PPT slides.

## What the tool produced

- `playlist.c`: `Song` node, `head` / `current`, insert (end, beginning, after current), unlink, search, display, demo loader, menu loop.
- `Makefile` and compile check with `gcc -std=c11 -Wall -Wextra`.
- `README.md`, `AI_LOG.md`, `slides.md`.

## What we changed or must own for the viva

Every member must be able to explain these lines without the AI:

- Why `head->prev` is the tail (circular list) and how insert-at-end uses it in O(1).
- The four pointer assignments in `link_after`.
- Single-node case: `next == prev == node`.
- Delete current vs delete head vs delete the only song.
- Play next/prev is a pointer move, not a search.
- Repeat All OFF does **not** break the ring; it only refuses to move past the last/first song.
- `size` is maintained so count is O(1); display/search still walk O(n).

Fixes made while generating (not just “accept the first draft”):

- Menu choice parsing: a non-numeric line must not be treated as `0` (exit). `strtol` is checked with an `end` pointer.
- Empty-list search does not prompt for a title (would desync piped/demo input).
- `--empty` vs default demo load so the live demo is not typing-heavy.

## Rule we followed

Write the data structure ourselves — node, `prev`/`next`, insert, delete, search. No STL / library collections for the playlist.
