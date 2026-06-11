#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


typedef enum {
    FOX,
    RABBIT,
    MOUSE,
    BIRD
} Suit;

typedef enum {
    NONE,
    MARQUISE,
    EYRIE,
    ALLIANCE,
    VAGABOND
} Faction;


typedef struct {
    char *name;
} Item;

typedef struct {
    char *name;
    Item item;
} Building;

typedef struct {
    char *name;
} Token;

typedef struct {
    Faction faction;
} Warrior;


typedef struct {
    char *name;
    Suit suit;
    Faction ruler;

    bool is_corner;
    struct Clearing *opposite_corner;

    Building slots[3];
    Token tokens[16];
    Warrior warriors[32];

    struct Clearing *paths[5];
    struct Clearing *rivers[2];
    struct Forest *adjacent_forests[5];
} Clearing;

typedef struct {
    char *name;

    Warrior warriors[8];

    Clearing *adjacent_clearings[5];
    struct Forest *adjacent_forests[5];
} Forest;


Clearing *create_clearing(char *name, Suit suit, int num_slots, bool has_ruin,
                          bool is_corner) {
    Clearing *clearing = malloc(sizeof(Clearing));

    clearing->name = name;
    clearing->suit = suit;
    clearing->ruler = NONE;

    clearing->is_corner = is_corner;

    // Fill out building slots.
    assert((num_slots > 0) && (num_slots <= 3));
    for (int i = 2; i > (num_slots - 1); i--) {
        Building null_slot = {"NULL_SLOT", {"NO_ITEM"}};
        clearing->slots[i] = null_slot;
    }
    for (int i = 0; i < num_slots; i++) {
        Building empty_slot = {"Empty", {"NO_ITEM"}};
        clearing->slots[i] = empty_slot;
    }

    if (has_ruin) {
        Building ruin = {"Ruin", {"PLACEHOLDER_ITEM"}}; 
        clearing->slots[0] = ruin;
    }

    return clearing;
}

Forest *create_forest(char *name) {
    Forest *forest = malloc(sizeof(Forest));

    forest->name = name;

    return forest;
}

void connect_clearing(Clearing *clearing, Clearing *paths[],
                      Clearing *rivers[], Forest *adjacent_forests[]) {
    memcpy(clearing->paths, paths, 5 * sizeof(Clearing *));
    memcpy(clearing->rivers, rivers, 2 * sizeof(Clearing *));
    memcpy(clearing->adjacent_forests, adjacent_forests, 5 * sizeof(Forest *));
}

void connect_forest(Forest *forest, Clearing *adjacent_clearings[],
                     Forest *adjacent_forests[]) {
    memcpy(forest->adjacent_clearings, adjacent_clearings, 5 * sizeof(Clearing *));
    memcpy(forest->adjacent_forests, adjacent_forests, 5 * sizeof(Forest *));
}

void print_clearing(Clearing *clearing) {
    printf("Clearing %s:\n", clearing->name);

    char *suit_str = NULL;
    char *ruler_str = NULL;
    switch(clearing->suit) {
        case FOX:
            suit_str = "Fox";
            break;
        case RABBIT:
            suit_str = "Rabbit";
            break;
        case MOUSE:
            suit_str = "Mouse";
    }
    switch(clearing->ruler) {
        case MARQUISE:
            ruler_str = "Marquise de Cat";
            break;
        case EYRIE:
            ruler_str = "Eyrie Dynasties";
            break;
        case ALLIANCE:
            ruler_str = "Woodland Alliance";
            break;
        default:
            ruler_str = "None";
    }
    printf("\tSuit: %s | Ruler: %s\n", suit_str, ruler_str);

    printf("\tBuilding Slots: ");
    for (int i = 0; i < 3; i++) {
        if (strcmp((clearing->slots[i]).name, "NULL_SLOT") != 0) {
            printf("%s ", (clearing->slots[i]).name);
        }
    }
    printf("\n");
}

Clearing *get_clearing(char *clearing_name, Clearing *clearings[]) {
    for (int i = 0; i < 12; i++) {
        Clearing *curr_clearing = clearings[i];
        if (strcmp(curr_clearing->name, clearing_name) == 0) {
            return curr_clearing;
        }
    }

    return NULL;
}


int main(void) {
    // Create each clearing.
    Clearing *fox_1 = create_clearing("F1", FOX, 1, false, true);
    Clearing *rabbit_1 = create_clearing("R1", RABBIT, 2, false, false);
    Clearing *mouse_1 = create_clearing("M1", MOUSE, 2, false, true);
    Clearing *mouse_2 = create_clearing("M2", MOUSE, 2, false, false);
    Clearing *rabbit_2 = create_clearing("R2", RABBIT, 2, true, false);
    Clearing *fox_2 = create_clearing("F2", FOX, 2, true, false);
    Clearing *mouse_3 = create_clearing("M3", MOUSE, 3, true, false);
    Clearing *fox_3 = create_clearing("F3", FOX, 2, true, false);
    Clearing *rabbit_3 = create_clearing("R3", RABBIT, 1, false, true);
    Clearing *fox_4 = create_clearing("F4", FOX, 2, false, false);
    Clearing *mouse_4 = create_clearing("M4", MOUSE, 2, false, false);
    Clearing *rabbit_4 = create_clearing("R4", RABBIT, 1, false, true);

    // Create each forest.
    Forest *forest_1 = create_forest("For1");
    Forest *forest_2 = create_forest("For2");
    Forest *forest_3 = create_forest("For3");
    Forest *forest_4 = create_forest("For4");
    Forest *forest_5 = create_forest("For5");
    Forest *forest_6 = create_forest("For6");
    Forest *forest_7 = create_forest("For7");


    // Connect all clearings.
    Clearing *fox_1_paths[] = {rabbit_1, rabbit_2, mouse_2, NULL, NULL};
    Clearing *fox_1_rivers[] = {NULL, NULL};
    Forest *fox_1_forests[] = {forest_1, forest_2, NULL, NULL, NULL};
    connect_clearing(fox_1, fox_1_paths, fox_1_rivers, fox_1_forests);

    Clearing *rabbit_1_paths[] = {fox_1, mouse_1, NULL, NULL, NULL};
    Clearing *rabbit_1_rivers[] = {rabbit_2, NULL};
    Forest *rabbit_1_forests[] = {forest_1, NULL, NULL, NULL, NULL};
    connect_clearing(rabbit_1, rabbit_1_paths, rabbit_1_rivers, rabbit_1_forests);

    Clearing *mouse_1_paths[] = {rabbit_1, rabbit_2, fox_3, NULL, NULL};
    Clearing *mouse_1_rivers[] = {NULL, NULL};
    Forest *mouse_1_forests[] = {forest_1, forest_3, NULL, NULL, NULL};
    connect_clearing(mouse_1, mouse_1_paths, mouse_1_rivers, mouse_1_forests);

    Clearing *mouse_2_paths[] = {fox_1, fox_2, rabbit_3, NULL, NULL};
    Clearing *mouse_2_rivers[] = {NULL, NULL};
    Forest *mouse_2_forests[] = {forest_2, forest_4, NULL, NULL, NULL};
    connect_clearing(mouse_2, mouse_2_paths, mouse_2_rivers, mouse_2_forests);

    Clearing *rabbit_2_paths[] = {fox_1, mouse_1, fox_2, NULL, NULL};
    Clearing *rabbit_2_rivers[] = {rabbit_1, mouse_3};
    Forest *rabbit_2_forests[] = {forest_1, forest_2, forest_3, NULL, NULL};
    connect_clearing(rabbit_2, rabbit_2_paths, rabbit_2_rivers, rabbit_2_forests);

    Clearing *fox_2_paths[] = {mouse_2, rabbit_2, mouse_3, mouse_4, rabbit_3};
    Clearing *fox_2_rivers[] = {NULL, NULL};
    Forest *fox_2_forests[] = {forest_2, forest_3, forest_4, forest_5, forest_7};
    connect_clearing(fox_2, fox_2_paths, fox_2_rivers, fox_2_forests);

    Clearing *mouse_3_paths[] = {fox_2, fox_3, rabbit_4, NULL, NULL};
    Clearing *mouse_3_rivers[] = {rabbit_2, mouse_4};
    Forest *mouse_3_forests[] = {forest_3, forest_5, forest_6, NULL, NULL};
    connect_clearing(mouse_3, mouse_3_paths, mouse_3_rivers, mouse_3_forests);

    Clearing *fox_3_paths[] = {mouse_1, mouse_3, rabbit_4, NULL, NULL};
    Clearing *fox_3_rivers[] = {NULL, NULL};
    Forest *fox_3_forests[] = {forest_3, forest_6, NULL, NULL, NULL};
    connect_clearing(fox_3, fox_3_paths, fox_3_rivers, fox_3_forests);

    Clearing *rabbit_3_paths[] = {mouse_2, fox_2, fox_4, NULL, NULL};
    Clearing *rabbit_3_rivers[] = {NULL, mouse_4};
    Forest *rabbit_3_forests[] = {forest_4, forest_7, NULL, NULL, NULL};
    connect_clearing(rabbit_3, rabbit_3_paths, rabbit_3_rivers, rabbit_3_forests);

    Clearing *fox_4_paths[] = {rabbit_3, mouse_4, NULL, NULL, NULL};
    Clearing *fox_4_rivers[] = {NULL, NULL};
    Forest *fox_4_forests[] = {forest_7, NULL, NULL, NULL, NULL};
    connect_clearing(fox_4, fox_4_paths, fox_4_rivers, fox_4_forests);

    Clearing *mouse_4_paths[] = {fox_2, fox_4, rabbit_4, NULL, NULL};
    Clearing *mouse_4_rivers[] = {rabbit_3, mouse_3};
    Forest *mouse_4_forests[] = {forest_5, forest_7, NULL, NULL, NULL};
    connect_clearing(mouse_4, mouse_4_paths, mouse_4_rivers, mouse_4_forests);

    Clearing *rabbit_4_paths[] = {mouse_4, mouse_3, fox_3, NULL, NULL};
    Clearing *rabbit_4_rivers[] = {NULL, NULL};
    Forest *rabbit_4_forests[] = {forest_5, forest_6, NULL, NULL, NULL};
    connect_clearing(rabbit_4, rabbit_4_paths, rabbit_4_rivers, rabbit_4_forests);

    // Connect all forests.
    Clearing *forest_1_clearings[] = {fox_1, rabbit_1, mouse_1, rabbit_2, NULL};
    Forest *forest_1_forests[] = {forest_2, forest_3, NULL, NULL, NULL};
    connect_forest(forest_1, forest_1_clearings, forest_1_forests);

    Clearing *forest_2_clearings[] = {fox_1, rabbit_2, mouse_2, fox_2, NULL};
    Forest *forest_2_forests[] = {forest_1, forest_3, forest_4, NULL, NULL};
    connect_forest(forest_2, forest_2_clearings, forest_2_forests);

    Clearing *forest_3_clearings[] = {rabbit_2, mouse_1, fox_3, mouse_3, fox_2};
    Forest *forest_3_forests[] = {forest_1, forest_2, forest_4, forest_5, NULL};
    connect_forest(forest_3, forest_3_clearings, forest_3_forests);

    Clearing *forest_4_clearings[] = {mouse_2, fox_2, rabbit_3, NULL, NULL};
    Forest *forest_4_forests[] = {forest_2, forest_7, NULL, NULL, NULL};
    connect_forest(forest_4, forest_4_clearings, forest_4_forests);

    Clearing *forest_5_clearings[] = {fox_2, mouse_3, rabbit_4, mouse_4, NULL};
    Forest *forest_5_forests[] = {forest_3, forest_6, forest_7, NULL, NULL};
    connect_forest(forest_5, forest_5_clearings, forest_5_forests);

    Clearing *forest_6_clearings[] = {mouse_3, fox_3, rabbit_4, NULL, NULL};
    Forest *forest_6_forests[] = {forest_3, forest_5, NULL, NULL, NULL};
    connect_forest(forest_6, forest_6_clearings, forest_6_forests);

    Clearing *forest_7_clearings[] = {rabbit_3, fox_2, mouse_4, fox_4, NULL};
    Forest *forest_7_forests[] = {forest_4, forest_5, NULL, NULL, NULL};
    connect_forest(forest_7, forest_7_clearings, forest_7_forests);


    // Final list of clearings/forests.
    Clearing *clearings[12] = {fox_1, fox_2, fox_3, fox_4, mouse_1, mouse_2,
                               mouse_3, mouse_4, rabbit_1, rabbit_2, rabbit_3,
                               rabbit_4};
    Forest *forests[7] = {forest_1, forest_2, forest_3, forest_4, forest_5,
                          forest_6, forest_7};


    // Game loop:
    bool is_playing = true;
    while (is_playing) {
        printf("Enter next command: ");

        char command_buf[128];
        fgets(command_buf, sizeof(command_buf), stdin);
        command_buf[strcspn(command_buf, "\n")] = '\0';

        char *command = strtok(command_buf, " ");
        char *arg = strtok(NULL, " ");

        if (strcmp(command, "quit") == 0) {
            is_playing = false;
        }
        else if (strcmp(command, "inspect") == 0) {
            Clearing *clearing = get_clearing(arg, clearings);
            if (clearing != NULL) {
                print_clearing(clearing);
            }
            else {
                printf("Clearing '%s' does not exist!\n", arg);
            }

            printf("\n");
        }
        else if (strcmp(command, "help") == 0) {
            printf("\tquit -> quit the game\n\tinspect [clearing] -> show info about that clearing\n");
            printf("\n");
        }
        else {
            printf("Invalid command! Type 'help' for help\n");
            printf("\n");
        }
    }
    printf("Shutting down!\n");

    return 0;
}

