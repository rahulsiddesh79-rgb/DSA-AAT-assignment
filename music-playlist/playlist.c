/*
 * Music Playlist — circular doubly linked list
 *
 * The playlist IS the list: next / previous / repeat-all are pointer moves,
 * not a second data structure. No library list types.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TITLE_LEN 64
#define ARTIST_LEN 64
#define LINE_LEN 128

typedef struct Song {
    int id;
    char title[TITLE_LEN];
    char artist[ARTIST_LEN];
    int duration_sec;
    struct Song *prev;
    struct Song *next;
} Song;

static Song *head = NULL;     /* first song; NULL if empty */
static Song *current = NULL;  /* now playing */
static int next_id = 1;
static int size = 0;
static int repeat_all = 1;    /* wrap is always circular; flag is for messaging */

/* ---------- helpers ---------- */

static void trim_newline(char *s)
{
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n')
        s[n - 1] = '\0';
}

static int read_line(char *buf, size_t cap)
{
    if (!fgets(buf, (int)cap, stdin))
        return 0;
    trim_newline(buf);
    return 1;
}

static int read_int(const char *prompt, int *out)
{
    char buf[LINE_LEN];
    char *end;
    long v;

    printf("%s", prompt);
    if (!read_line(buf, sizeof buf))
        return 0;
    if (buf[0] == '\0')
        return 0;
    v = strtol(buf, &end, 10);
    if (end == buf)
        return 0;
    while (*end && isspace((unsigned char)*end))
        end++;
    if (*end != '\0')
        return 0;
    *out = (int)v;
    return 1;
}

static Song *make_song(const char *title, const char *artist, int duration_sec)
{
    Song *s = (Song *)malloc(sizeof(Song));
    if (!s) {
        fprintf(stderr, "Out of memory.\n");
        exit(1);
    }
    s->id = next_id++;
    strncpy(s->title, title, TITLE_LEN - 1);
    s->title[TITLE_LEN - 1] = '\0';
    strncpy(s->artist, artist, ARTIST_LEN - 1);
    s->artist[ARTIST_LEN - 1] = '\0';
    s->duration_sec = duration_sec;
    s->prev = s;
    s->next = s;
    return s;
}

static void print_song(const Song *s, int mark_current)
{
    printf("  %s [%d] \"%s\" — %s (%d sec)\n",
           (mark_current && s == current) ? ">" : " ",
           s->id, s->title, s->artist, s->duration_sec);
}

static void now_playing(void)
{
    if (!current) {
        printf("Nothing is playing (playlist empty).\n");
        return;
    }
    printf("Now playing: [%d] \"%s\" — %s (%d sec)%s\n",
           current->id, current->title, current->artist, current->duration_sec,
           repeat_all ? "  [Repeat All ON]" : "  [Repeat All OFF]");
}

/* ---------- circular DLL operations ---------- */

/* Insert as the only node, or link into an existing ring. */
static void link_after(Song *pos, Song *node)
{
    Song *nxt = pos->next;
    node->prev = pos;
    node->next = nxt;
    pos->next = node;
    nxt->prev = node;
}

static void insert_empty(Song *node)
{
    node->next = node;
    node->prev = node;
    head = node;
    current = node;
    size = 1;
}

void insert_at_end(Song *node)
{
    if (!head) {
        insert_empty(node);
        return;
    }
    /* tail is head->prev in a circular list — O(1) */
    link_after(head->prev, node);
    size++;
}

void insert_at_beginning(Song *node)
{
    if (!head) {
        insert_empty(node);
        return;
    }
    link_after(head->prev, node); /* insert before head, then reassign */
    head = node;
    size++;
}

void insert_after_current(Song *node)
{
    if (!current) {
        insert_empty(node);
        return;
    }
    link_after(current, node);
    size++;
}

Song *find_by_id(int id)
{
    Song *p;
    if (!head)
        return NULL;
    p = head;
    do {
        if (p->id == id)
            return p;
        p = p->next;
    } while (p != head);
    return NULL;
}

Song *find_by_title(const char *title)
{
    Song *p;
    if (!head)
        return NULL;
    p = head;
    do {
        if (strcmp(p->title, title) == 0)
            return p;
        p = p->next;
    } while (p != head);
    return NULL;
}

/* Unlink node from the ring. O(1) after we have the pointer. */
int delete_song(Song *node)
{
    if (!node || !head)
        return 0;

    if (node->next == node) {
        /* only song */
        head = NULL;
        current = NULL;
        size = 0;
        free(node);
        return 1;
    }

    node->prev->next = node->next;
    node->next->prev = node->prev;
    if (head == node)
        head = node->next;
    if (current == node)
        current = node->next;
    size--;
    free(node);
    return 1;
}

int delete_by_id(int id)
{
    Song *n = find_by_id(id);
    if (!n)
        return 0;
    return delete_song(n);
}

void play_next(void)
{
    if (!current) {
        printf("Playlist is empty.\n");
        return;
    }
    if (!repeat_all && current->next == head) {
        printf("End of playlist (Repeat All is OFF). Staying on last song.\n");
        now_playing();
        return;
    }
    current = current->next;
    now_playing();
}

void play_prev(void)
{
    if (!current) {
        printf("Playlist is empty.\n");
        return;
    }
    if (!repeat_all && current == head) {
        printf("Start of playlist (Repeat All is OFF). Staying on first song.\n");
        now_playing();
        return;
    }
    current = current->prev;
    now_playing();
}

void repeat_current(void)
{
    if (!current) {
        printf("Playlist is empty.\n");
        return;
    }
    printf("Repeating this song.\n");
    now_playing();
}

void display_playlist(void)
{
    Song *p;
    if (!head) {
        printf("Playlist is empty.\n");
        return;
    }
    printf("Playlist (%d song%s). '>' is now playing.\n",
           size, size == 1 ? "" : "s");
    p = head;
    do {
        print_song(p, 1);
        p = p->next;
    } while (p != head);
}

void clear_playlist(void)
{
    while (head)
        delete_song(head);
    next_id = 1;
}

void load_demo(void)
{
    clear_playlist();
    insert_at_end(make_song("Blinding Lights", "The Weeknd", 200));
    insert_at_end(make_song("Levitating", "Dua Lipa", 203));
    insert_at_end(make_song("Peaches", "Justin Bieber", 198));
    insert_at_end(make_song("Heat Waves", "Glass Animals", 238));
    current = head;
    printf("Loaded 4 demo songs. Repeat All is ON.\n");
    display_playlist();
    now_playing();
}

/* ---------- menu ---------- */

static void read_song_fields(char *title, char *artist, int *dur)
{
    printf("Title: ");
    read_line(title, TITLE_LEN);
    printf("Artist: ");
    read_line(artist, ARTIST_LEN);
    if (!read_int("Duration (seconds): ", dur) || *dur <= 0)
        *dur = 0;
}

static void add_flow(int where)
{
    char title[TITLE_LEN];
    char artist[ARTIST_LEN];
    int dur;
    Song *s;

    read_song_fields(title, artist, &dur);
    if (title[0] == '\0') {
        printf("Invalid input: title cannot be empty.\n");
        return;
    }
    if (dur <= 0) {
        printf("Invalid input: duration must be a positive integer.\n");
        return;
    }
    s = make_song(title, artist, dur);
    if (where == 0)
        insert_at_end(s);
    else if (where == 1)
        insert_at_beginning(s);
    else
        insert_after_current(s);
    printf("Added [%d] \"%s\".\n", s->id, s->title);
    display_playlist();
}

static void print_menu(void)
{
    printf("\n======== MUSIC PLAYLIST (circular doubly linked list) ========\n");
    printf(" 1. Add song at end\n");
    printf(" 2. Add song at beginning\n");
    printf(" 3. Insert after current song\n");
    printf(" 4. Delete song by id\n");
    printf(" 5. Play next\n");
    printf(" 6. Play previous\n");
    printf(" 7. Repeat current song\n");
    printf(" 8. Toggle Repeat All (now %s)\n", repeat_all ? "ON" : "OFF");
    printf(" 9. Display playlist\n");
    printf("10. Search by title\n");
    printf("11. Count songs\n");
    printf("12. Now playing\n");
    printf("13. Load demo playlist\n");
    printf("14. Clear playlist\n");
    printf(" 0. Exit\n");
    printf("Choice: ");
}

int main(int argc, char **argv)
{
    char line[LINE_LEN];
    int choice;
    int empty_start = 0;

    if (argc > 1 && strcmp(argv[1], "--empty") == 0)
        empty_start = 1;

    printf("Music Playlist — circular doubly linked list\n");
    if (!empty_start)
        load_demo();
    else
        printf("Started empty. Use menu option 1 or 13 to add songs.\n");

    for (;;) {
        print_menu();
        if (!read_line(line, sizeof line))
            break;
        {
            char *end = NULL;
            long v;
            if (line[0] == '\0') {
                printf("Invalid choice.\n");
                continue;
            }
            v = strtol(line, &end, 10);
            if (end == line) {
                printf("Invalid choice.\n");
                continue;
            }
            while (*end && isspace((unsigned char)*end))
                end++;
            if (*end != '\0') {
                printf("Invalid choice.\n");
                continue;
            }
            choice = (int)v;
        }

        switch (choice) {
        case 1:
            add_flow(0);
            break;
        case 2:
            add_flow(1);
            break;
        case 3:
            add_flow(2);
            break;
        case 4: {
            int id;
            if (!head) {
                printf("Playlist is empty. Nothing to delete.\n");
                break;
            }
            if (!read_int("Song id to delete: ", &id)) {
                printf("Invalid id.\n");
                break;
            }
            if (delete_by_id(id)) {
                printf("Deleted song %d.\n", id);
                display_playlist();
                now_playing();
            } else {
                printf("Not found: no song with id %d.\n", id);
            }
            break;
        }
        case 5:
            play_next();
            break;
        case 6:
            play_prev();
            break;
        case 7:
            repeat_current();
            break;
        case 8:
            repeat_all = !repeat_all;
            printf("Repeat All is now %s.\n", repeat_all ? "ON" : "OFF");
            break;
        case 9:
            display_playlist();
            break;
        case 10: {
            char title[TITLE_LEN];
            Song *found;
            if (!head) {
                printf("Playlist is empty.\n");
                break;
            }
            printf("Title to search: ");
            read_line(title, sizeof title);
            if (title[0] == '\0') {
                printf("Invalid input: empty title.\n");
                break;
            }
            found = find_by_title(title);
            if (found) {
                printf("Found:\n");
                print_song(found, 1);
            } else {
                printf("Not found: \"%s\".\n", title);
            }
            break;
        }
        case 11:
            printf("Song count: %d\n", size);
            break;
        case 12:
            now_playing();
            break;
        case 13:
            load_demo();
            break;
        case 14:
            clear_playlist();
            printf("Playlist cleared.\n");
            break;
        case 0:
            clear_playlist();
            printf("Goodbye.\n");
            return 0;
        default:
            printf("Invalid choice.\n");
            break;
        }
    }
    clear_playlist();
    return 0;
}
