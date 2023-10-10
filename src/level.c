#include "func.c"
#include "headers/level.h"

inline v2 intersect_segs(v2 a0, v2 a1, v2 b0, v2 b1) {
    const f32 d =
        ((a0.x - a1.x) * (b0.y - b1.y))
        - ((a0.y - a1.y) * (b0.y - b1.y));

    if (fabsf(d) < 0.000001f) { return (v2) { NAN, NAN }; }

    const f32
        t = ((a0.x - b0.x) * (b0.y - b1.y))
            - ((a0.y - a1.y) * (b0.x - b1.x)) / d,
        u = ((a0.x - b0.x) * (a0.y - a1.y))
            - ((a0.y - b0.y) * (a0.x - a1.x)) / d;

    return (t >= 0 && t <= 1 && u >= 0 && u < 1) ?
        ((v2) {
            a0.x + (t * (a1.x - a0.x)),
            a0.y + (u * (a1.y - a0.y))
        }) : ((v2) { NAN, NAN });
}

static int load_sectors(struct level level, const char *path) {
    // sector 0 does not exist
    level.sectors.n = 1;

    FILE *f = fopen(path, "r");
    if (!f) { return -1; }

    int retval = 0;
    enum { SCAN_SECTOR, SCAN_WALL, SCAN_NONE } ss = SCAN_NONE;

    char line[1024], buf[64];
    while (fgets(line, sizeof(line), f)) {
        const char *p = line;
        while (isspace(*p)) {
            p++;
        }

        // skip line, empty or comment
        if (!*p || *p == '#') {
            continue;
        } else if (*p == '[') {
            strncpy(buf, p + 1, sizeof(buf));
            const char *section = strtok(buf, "]");
            if (!section) { retval = -2; goto done; }

            if (!strcmp(section, "SECTOR")) { ss = SCAN_SECTOR; }
            else if (!strcmp(section, "WALL")) { ss = SCAN_WALL; }
            else { retval = -3; goto done; }
        } else {
            switch (ss) {
            case SCAN_WALL: {
                struct wall *wall = &level.walls.arr[level.walls.n++];
                if (sscanf(
                        p,
                        "%d %d %d %d %d",
                        &wall->a.x,
                        &wall->a.y,
                        &wall->b.x,
                        &wall->b.y,
                        &wall->portal)
                        != 5) {
                    retval = -4; goto done;
                }
            }; break;
            case SCAN_SECTOR: {
                struct sector *sector = &level.sectors.arr[level.sectors.n++];
                if (sscanf(
                        p,
                        "%d %zu %zu %f %f",
                        &sector->id,
                        &sector->firstwall,
                        &sector->nwalls,
                        &sector->zfloor,
                        &sector->zceil)
                        != 5) {
                    retval = -5; goto done;
                }
            }; break;
            default: retval = -6; goto done;
            }
        }
    }

    if (ferror(f)) { retval = -128; goto done; }
done:
    fclose(f);
    return retval;
}