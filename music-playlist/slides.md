# Slides for PowerPoint (8 slides)

Paste each `# Slide N` section onto its own slide. Add screenshots from a live run of `playlist.exe` on slides 5–6.

---

# Slide 1 — Problem

**Music Playlist (next / previous / repeat)**

- Real problem: a player must move forward, backward, and loop the album.
- Users also add and delete songs while one song is “now playing”.
- Course: 1BAI303 DSA Capstone AAT — data structure must do the real work.

---

# Slide 2 — Why a circular doubly linked list

- **Doubly:** `current->next` and `current->prev` are O(1). A singly list cannot go back without scanning.
- **Circular:** last song’s `next` is the first song → Repeat All needs no extra structure.
- **Not an array:** insert/delete in the middle would shift O(n) songs.
- **Not a stack/queue:** those are one-way.

---

# Slide 3 — Data structure diagram

Each node: `id`, `title`, `artist`, `duration_sec`, `prev`, `next`.

```
        prev                         next
   ┌──────────┐     ┌──────────┐     ┌──────────┐
   │  Song A  │◄───►│  Song B  │◄───►│  Song C  │
   └──────────┘     └──────────┘     └──────────┘
        ▲                                   │
        └──────────── circular ─────────────┘

head  = first song (A)
current = now playing (any node)
tail  = head->prev  (C)   ← O(1) insert at end
```

Empty: `head == NULL`. One song: `next` and `prev` both point to itself.

---

# Slide 4 — Operations and complexity

| Operation | Time |
|-----------|------|
| Insert end / beginning / after current | O(1) |
| Play next / previous / repeat current | O(1) |
| Delete by id | O(n) find, O(1) unlink |
| Search title / display | O(n) |
| Count (`size`) | O(1) |
| Clear | O(n) |

Insert at end: `link_after(head->prev, node)`.

---

# Slide 5 — Demo (add / next / prev)

**Screenshot 1:** load demo (menu 13) — four songs, `>` on current.

**Screenshot 2:** Play next (5) and previous (6) — only `current` moves.

Talking point: next/prev never walk the list.

---

# Slide 6 — Demo (insert, wrap, delete)

**Screenshot 3:** Insert after current (3) — new node between current and old next.

**Screenshot 4:** Repeat All OFF (8) + next on last song — stays put; ring is still circular.

**Screenshot 5:** Delete by id (4) — unlinks four pointers; if current is deleted, play moves to `next`.

---

# Slide 7 — What AI did vs what we did

**AI (Cursor):** scaffolded `playlist.c`, Makefile, README, this deck, usage log.

**We must own (viva):**

- Four-pointer `link_after` and unlink
- Empty / one-node / delete-current cases
- Why Repeat All is a flag, not a second list
- Time complexities above

Rule: no library list — the node and every operation are in `playlist.c`.

---

# Slide 8 — Team

| Name | USN | Role |
|------|-----|------|
| (fill) | | |
| (fill) | | |
| (fill) | | |

GitHub: (paste repo URL after you push)

Live demo: `gcc -std=c11 -Wall -Wextra -o playlist playlist.c` then `./playlist`
