#include <stdio.h>
#include <Windows.h>
#include <ctype.h>

#include "headers/level.h"
#include "headers/vector2.h"

int load_level(const char* path, struct level* level) {

    level->sectors.n = 0;
    FILE *file = fopen(path, "r");
    if (!file) { return -1; }

    int retval = 0;
    enum { SCAN_SECTOR, SCAN_WALL, SCAN_NONE } scan = SCAN_NONE;

    char line[1024], buf[64];
    while (fgets(line, sizeof(line), file)) {
        const char *p = line;
        while(isspace(*p)) {
            p++;
        }

        if (!*p || *p == '#') { 
            continue; 
        }
        else if (*p == '[') {
            strncpy(buf, p+1, sizeof(buf));
            const char *section = strtok(buf, "]");
            if (!section) { retval = -2; goto done; }

            if (!strcmp(section, "SECTOR")) { scan = SCAN_SECTOR; }
            else if (!strcmp(section, "WALL")) { scan = SCAN_WALL; }
            else { retval = -3; goto done; }
        } else {
            switch (scan) {
            case SCAN_WALL: {
                wall *scwall = calloc(sizeof(wall), 1);
                int sector;
                if (sscanf(
                        p,
                        "%i %f %f %f %f %i",
                        &sector,
                        &scwall->a.x,
                        &scwall->a.y,
                        &scwall->b.x,
                        &scwall->b.y,
                        &scwall->portal)
                        != 6) {
                    retval = -4; goto done;
                }
                

                struct sector *csector = &level->sectors.arr[sector];
                wall *cwall = csector->list.fwall;
                wall *pwall = cwall;


                if (cwall == NULL) {
                    csector->list.fwall = scwall;
                    printf("\ninit wall: %p | (%i, %i)\n", csector->list.fwall, (int)csector->list.fwall->a.x, (int)csector->list.fwall->a.y);
                    printf("first next: %p\n", csector->list.fwall->next);
                } else {
                    while (cwall != NULL) {
                        printf("(null-(%i,%i)): %p, ", (int)cwall->a.x, (int)cwall->a.y, cwall);
                        pwall = cwall;
                        cwall = cwall->next;
                    }
                    cwall = scwall;
                    pwall->next = cwall;
                    printf("\n");
                    printf("added wall: %p | (%i, %i)\n", cwall, (int)cwall->a.x, (int)cwall->a.y);
                }
                
                csector->list.n++;
                printf("\n");
            }; break;
            case SCAN_SECTOR: {
                struct sector *sector = &level->sectors.arr[level->sectors.n];
                printf("SECTOR: %i\n", level->sectors.n);
                if (sscanf(
                        p,
                        "%d %f %f",
                        &sector->id,
                        &sector->zfloor,
                        &sector->zceil)
                        != 3) {
                    retval = -5; goto done;
                }
                level->sectors.n++;
            }; break;
        default: retval = -6; goto done;
            }
        }
    }
    if (ferror(file)) { retval = -128; goto done; }
done:
    fclose(file);
    return retval;
}