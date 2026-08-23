/**
Scenario:
    A simple CVE (Common Vulnerabilities and Exposures) tracking system.

Details:
     CONCEPT THEORY & ARCHITECTURE:
         1. Struct Arrays: Utilizes an array of structures to act as a lightweight,
            in-memory database for CVE records.
         2. Control Flow: Implements a while-loop and switch-case state machine to
            handle interactive CLI menu routing.
         3. Data Normalization: Uses ctype.h (toupper) to mutate user string input
            in-place, ensuring case-insensitive data entry and searching.
         4. Safe String Comparison: Replaces improper `==` string comparisons with
            <string.h> strcmp() to accurately evaluate memory contents.

 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_ENTRIES 100 // Defined a strict limit for the array size

static int track_count_cve_code = 1;

// Define the CVE structure
static struct cve {
    int cve_code;
    char severity[10];
    int priority_scale;
} cv[MAX_ENTRIES]; // Initialized with a maximum capacity

int main() {
    int start = 1;

    // Main application loop
    while (start != 0) {
        printf("\n=== CVE Tracker Menu ===\n");
        printf("[Option 1] Insert new entries\n");
        printf("[Option 2] Search entries\n");
        printf("[Option 0] Exit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("Enter the total number of entries to insert: ");
                int read;
                scanf("%d", &read);

                // Prevent buffer overflow of the struct array
                if (track_count_cve_code + read > MAX_ENTRIES) {
                    printf("[Error] Not enough space for %d entries.\n", read);
                    break;
                }

                int start_index = track_count_cve_code;
                track_count_cve_code = track_count_cve_code + read;

                // Entry insertion loop
                for (int i = start_index; i < track_count_cve_code; i++) {
                    printf("\nEnter CVE code: ");
                    scanf("%d", &cv[i].cve_code);

                    printf("Enter CVE severity (LOW, MEDIUM, HIGH, CRITICAL): ");
                    scanf("%9s", cv[i].severity); // %9s prevents string overflow

                    // Normalize severity input to uppercase
                    int j = 0;
                    while (cv[i].severity[j] != '\0') {
                        cv[i].severity[j] = toupper(cv[i].severity[j]);
                        j++;
                    }

                    // Validate severity text
                    if (strcmp(cv[i].severity, "LOW") != 0 &&
                        strcmp(cv[i].severity, "MEDIUM") != 0 &&
                        strcmp(cv[i].severity, "HIGH") != 0 &&
                        strcmp(cv[i].severity, "CRITICAL") != 0) {

                        printf("[Error] Invalid severity text.\n");
                        i--; // Decrement i to retry this entry
                        continue;
                    }

                    printf("Enter the priority value: ");
                    scanf("%d", &cv[i].priority_scale);
                }
                break;
            }

            case 2: {
                printf("\n--- Search CVE Data ---\n");
                int loop = 1;

                // Search sub-menu loop
                while (loop != 0) {
                    printf("\nOptions:\n");
                    printf("1. Search by Code\n2. Search by Severity\n3. Search by Priority\n0. Return to Main Menu\n");

                    int option;
                    printf("Select search option: ");
                    scanf("%d", &option);

                    switch (option) {
                        case 1: {
                            int user_search_code;
                            printf("Enter the CVE code: ");
                            scanf("%d", &user_search_code);

                            // Bounds checking for array access
                            if (user_search_code > 0 && user_search_code < track_count_cve_code) {
                                printf("\nData for Code %d:\n", user_search_code);
                                printf(" |--- Severity : %s\n", cv[user_search_code].severity);
                                printf(" |--- Priority : %d\n", cv[user_search_code].priority_scale);
                            } else {
                                printf("[Error] CVE Code not found.\n");
                            }
                            break;
                        }
                        case 2: {
                            char user_search_severity[10];
                            printf("Enter the severity: ");
                            scanf("%9s", user_search_severity);

                            // Normalize search term to uppercase
                            int i = 0;
                            while (user_search_severity[i] != '\0') {
                                user_search_severity[i] = toupper(user_search_severity[i]);
                                i++;
                            }

                            printf("\nData for Severity '%s':\n", user_search_severity);
                            // Dynamic search loop instead of hardcoded 1 to 4
                            for (int k = 1; k < track_count_cve_code; k++) {
                                if (strcmp(user_search_severity, cv[k].severity) == 0) {
                                    printf(" |--- CVE Code : %d\n", cv[k].cve_code);
                                }
                            }
                            break;
                        }
                        case 3: {
                            int user_search_priority;
                            printf("Enter the priority scale: ");
                            scanf("%d", &user_search_priority);

                            printf("\nData for Priority '%d':\n", user_search_priority);
                            for (int k = 1; k < track_count_cve_code; k++) {
                                if (user_search_priority == cv[k].priority_scale) {
                                    printf(" |--- CVE Code : %d\n", cv[k].cve_code);
                                }
                            }
                            break;
                        }
                        case 0:
                        default: {
                            loop = 0; // Exit sub-menu
                            break;
                        }
                    }
                }
                break;
            }

            case 0: {
                start = 0;
                printf("Exiting... Thank you!\n");
                break;
            }
            default: {
                printf("[Error] Invalid selection. Try again.\n");
            }
        }
    }
    return 0;
}