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
    NO_FACTION,
    MARQUISE,
    EYRIE,
    ALLIANCE,
    VAGABOND
} Faction;

typedef struct {
    char *name;
} Item;

typedef enum {
    NO_SLOT,
    EMPTY,
    RUIN,
    SAWMILL,
    RECRUITER,
    WORKSHOP,
    ROOST,
    BASE_FOX,
    BASE_RABBIT,
    BASE_MOUSE
} BuildingType;

typedef struct {
    char *name;
    BuildingType type;
    Faction faction;

    bool is_crafter;
    bool has_crafted_this_turn;
} Building;

typedef enum {
    NO_TOKEN,
    KEEP,
    WOOD,
    SYMPATHY
} TokenType;

typedef struct {
    char *name;
    TokenType type;
    Faction faction;

    bool is_crafter;
    bool has_crafted_this_turn;
} Token;

typedef struct {
    Faction faction;
} Warrior;


typedef struct Clearing {
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


Building create_building(char *name, BuildingType type, Faction faction,
                         bool is_crafter) {
    Building new_building = {0};

    new_building.name = name;
    new_building.type = type;
    new_building.faction = faction;

    new_building.is_crafter = is_crafter;
    new_building.has_crafted_this_turn = false;

    return new_building;
}

Token create_token(char *name, TokenType type, Faction faction,
                         bool is_crafter) {
    Token new_token = {0};

    new_token.name = name;
    new_token.type = type;
    new_token.faction = faction;

    new_token.is_crafter = is_crafter;
    new_token.has_crafted_this_turn = false;

    return new_token;
}

Clearing *create_clearing(char *name, Suit suit, int num_slots, bool has_ruin,
                          bool is_corner, Building buildings[], Token tokens[]) {
    Clearing *clearing = malloc(sizeof(Clearing));

    clearing->name = name;
    clearing->suit = suit;
    clearing->ruler = NO_FACTION;

    clearing->is_corner = is_corner;

    // Fill out building slots.
    assert((num_slots > 0) && (num_slots <= 3));
    for (int i = 2; i > (num_slots - 1); i--) {
        clearing->slots[i] = buildings[NO_SLOT];
    }
    for (int i = 0; i < num_slots; i++) {
        clearing->slots[i] = buildings[EMPTY];
    }
    if (has_ruin) {
        clearing->slots[0] = buildings[RUIN];
    }

    // Set clearing to be empty of tokens.
    for (int i = 0; i < 16; i++) {
        clearing->tokens[i] = tokens[NO_TOKEN];
    }

    // Set clearing to be empty of warriors. 'NO_FACTION' warriors don't exist.
    for (int i = 0; i < 32; i++) {
        clearing->warriors[i].faction = NO_FACTION;
    }

    return clearing;
}

Forest *create_forest(char *name) {
    Forest *forest = malloc(sizeof(Forest));

    forest->name = name;

    for (int i = 0; i < 8; i++) {
        forest->warriors[i].faction = NO_FACTION;
    }

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
    switch (clearing->suit) {
        case FOX:
            suit_str = "Fox";
            break;
        case RABBIT:
            suit_str = "Rabbit";
            break;
        case MOUSE:
            suit_str = "Mouse";
    }
    switch (clearing->ruler) {
        case NO_FACTION:
            ruler_str = "None";
            break;
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
            ruler_str = "ERROR! IMPROPER ENUM!";
    }
    printf("\tSuit: %s | Ruler: %s\n", suit_str, ruler_str);

    printf("\tBuilding Slots: ");
    for (int i = 0; i < 3; i++) {
        if (clearing->slots[i].type != NO_SLOT) {
            printf("%s ", clearing->slots[i].name);
        }
    }
    printf("\n");

    printf("\tTokens: ");
    bool has_any_tokens = false;
    for (int i = 0; i < 16; i++) {
        if (clearing->tokens[i].type != NO_TOKEN) {
            printf("%s ", clearing->tokens[i].name);
            has_any_tokens = true;
        }
    }
    if (has_any_tokens == false) {
        printf("None");
    }
    printf("\n");

    printf("\tWarriors: ");
    bool has_any_warriors = false;
    for (int i = 0; i < 32; i++) {
        if (clearing->warriors[i].faction != NO_FACTION) {
            switch (clearing->warriors[i].faction) {
                case MARQUISE:
                    printf("Marquise ");
                    break;
                case EYRIE:
                    printf("Eyrie ");
                    break;
                case ALLIANCE:
                    printf("Alliance ");
                    break;
                default:
                    printf("ERROR! IMPROPER ENUM! ");
            }
            has_any_warriors = true;
        }
    }
    if (has_any_warriors == false) {
        printf("None");
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

void setup_marquise(Clearing *clearings[], Token tokens[]) {
    printf("Welcome, Marquise de Cat!\n");
 
    Clearing *keep_clearing = NULL;

    // Place keep token.
    bool is_placing_keep = true;
    while (is_placing_keep) {
        printf("Where shall you place your keep? ");

        char chosen_clearing[16];
        fgets(chosen_clearing, sizeof(chosen_clearing), stdin);
        chosen_clearing[strcspn(chosen_clearing, "\n")] = '\0';

        keep_clearing = get_clearing(chosen_clearing, clearings);
        if (keep_clearing->is_corner != true) {
            printf("Clearing %s is not a corner clearing! Please choose a corner.\n", chosen_clearing);
            continue;
        }

        keep_clearing->tokens[0] = tokens[KEEP];
        is_placing_keep = false;
    }

    // Place initial warriors.
    Warrior marquise_warrior = {MARQUISE};
    for (int i = 0; i < 12; i++) {
        Clearing *curr_clearing = clearings[i];

        if (strcmp(curr_clearing->name, keep_clearing->opposite_corner->name) != 0) {
            curr_clearing->warriors[0] = marquise_warrior;
        }
    }
}


int main(void) {
    // Create buildings.
    Building no_slot = create_building("NO_SLOT", NO_SLOT, NO_FACTION, false);
    Building empty = create_building("Empty", EMPTY, NO_FACTION, false);
    Building ruin = create_building("Ruin", RUIN, NO_FACTION, false);
    Building sawmill = create_building("Sawmill", SAWMILL, MARQUISE, false);
    Building recruiter = create_building("Recruiter", RECRUITER, MARQUISE, false);
    Building workshop = create_building("Workshop", WORKSHOP, MARQUISE, true);
    Building roost = create_building("Roost", ROOST, EYRIE, true);
    Building base_fox = create_building("Fox Base", BASE_FOX, ALLIANCE, false);
    Building base_rabbit = create_building("Rabbit Base", BASE_RABBIT, ALLIANCE, false);
    Building base_mouse = create_building("Mouse Base", BASE_MOUSE, ALLIANCE, false);

    // Create tokens.
    Token no_token = create_token("NO_TOKEN", NO_TOKEN, NO_FACTION, false);
    Token keep = create_token("Keep", KEEP, MARQUISE, false);
    Token wood = create_token("Wood", WOOD, MARQUISE, false);
    Token sympathy = create_token("Sympathy", SYMPATHY, ALLIANCE, true);

    // Final lists of buildings/tokens.
    Building buildings[] = {no_slot, empty, ruin, sawmill, recruiter, workshop,
                            roost, base_fox, base_rabbit, base_mouse};
    Token tokens[] = {no_token, keep, wood, sympathy};


    // Create each clearing.
    Clearing *fox_1 = create_clearing("F1", FOX, 1, false, true, buildings, tokens);
    Clearing *rabbit_1 = create_clearing("R1", RABBIT, 2, false, false, buildings, tokens);
    Clearing *mouse_1 = create_clearing("M1", MOUSE, 2, false, true, buildings, tokens);
    Clearing *mouse_2 = create_clearing("M2", MOUSE, 2, false, false, buildings, tokens);
    Clearing *rabbit_2 = create_clearing("R2", RABBIT, 2, true, false, buildings, tokens);
    Clearing *fox_2 = create_clearing("F2", FOX, 2, true, false, buildings, tokens);
    Clearing *mouse_3 = create_clearing("M3", MOUSE, 3, true, false, buildings, tokens);
    Clearing *fox_3 = create_clearing("F3", FOX, 2, true, false, buildings, tokens);
    Clearing *rabbit_3 = create_clearing("R3", RABBIT, 1, false, true, buildings, tokens);
    Clearing *fox_4 = create_clearing("F4", FOX, 2, false, false, buildings, tokens);
    Clearing *mouse_4 = create_clearing("M4", MOUSE, 2, false, false, buildings, tokens);
    Clearing *rabbit_4 = create_clearing("R4", RABBIT, 1, false, true, buildings, tokens);

    // Set opposite corners.
    fox_1->opposite_corner = rabbit_4;
    mouse_1->opposite_corner = rabbit_3;
    rabbit_3->opposite_corner = mouse_1;
    rabbit_4->opposite_corner = fox_1;

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

    setup_marquise(clearings, tokens);

    // Game loop:
    bool is_playing = true;
    while (is_playing) {
        printf("Enter next command: ");

        char command_buf[128];
        fgets(command_buf, sizeof(command_buf), stdin);
        command_buf[strcspn(command_buf, "\n")] = '\0';

        char *command = strtok(command_buf, " ");
        char *arg = strtok(NULL, " ");

        // Quit
        if (strcmp(command, "quit") == 0) {
            is_playing = false;
        }
        // Inspect
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
        // Help
        else if (strcmp(command, "help") == 0) {
            printf("\tquit -> quit the game\n\tinspect [clearing] -> show info about that clearing\n");
            printf("\n");
        }
        // Default response
        else {
            printf("Invalid command! Type 'help' for help\n");
            printf("\n");
        }
    }
    printf("Shutting down!\n");

    return 0;
}

