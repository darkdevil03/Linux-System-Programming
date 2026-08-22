#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int track_count_cve_code = 1;

struct cve {
    int cve_code;
    char severity[10];
    int priority_scale;
}cv[];


int main() {
    int start = 1;

    while (start != 0) {
        printf(" Choose one of the following options : \n ");
        printf("[Option 1] For new entries filling. \n");
        printf("[Option 2] For searching entries. \n");
        printf("[Option 3] Exit...! \n");

        int choice;
        printf("Enter your choice : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("Enter the total number of entries to insert : \n");

                int read;
                scanf("%d", &read);
                int start_index = track_count_cve_code;

                track_count_cve_code = track_count_cve_code + read;

                for (int i = start_index; i < track_count_cve_code; i++) {
                    printf("Enter CVE code : ");
                    scanf("%d", &cv[i].cve_code);
                    printf("Enter CVE severity : ");
                    scanf("%s", cv[i].severity);
                    int j = 0;
                    while (cv[i].severity[j] != '\0') {
                        cv[i].severity[j] = toupper(cv[i].severity[j]);
                        j++;
                    }
                    if (strcmp(cv[i].severity, "LOW") != 0 && strcmp(cv[i].severity, "MEDIUM") != 0 && strcmp(cv[i].severity, "HIGH") != 0 && strcmp(cv[i].severity, "CRITICAL") != 0) {
                        printf("[Error] Please enter correct CVE severity text : { LOW, MEDIUM, HIGH, or CRITICAL } \n");
                        i--;
                        continue;
                    }

                    printf("Enter the priority value : ");
                    scanf("%d", &cv[i].priority_scale);
                }

                break;
            }

            case 2: {
                printf("Search for data of CVE : \n");
                int loop = 1;
                while (loop!=0) {
                    printf("Options \n");
                    printf("1.Search by code \n 2.Search by severity \n 3.Search by priority scale  \n 4.Exit\n");

                    int option;
                    scanf("%d", &option);

                    switch (option) {
                        case 1: {
                            int user_search_code;
                            printf("Enter the code for their severity and priorty scale : ");
                            scanf("%d", &user_search_code);
                            printf("The data of %d : \n", user_search_code);
                            printf("       |------- severity -----> %s\n",cv[user_search_code].severity);
                            printf("       |------- priority -----> %d\n",cv[user_search_code].priority_scale);
                            break;
                        }
                        case 2: {
                            char user_search_severity[10];
                            printf("Enter the severity for list cve code : ");
                            scanf("%s", user_search_severity);

                            int i = 0;
                            while (user_search_severity[i] != '\0') {
                                user_search_severity[i] = toupper(user_search_severity[i]);
                                i++;
                            }

                            for (int i = 1; i <= 4; i++) {
                                if ( user_search_severity == cv[i].severity) {
                                    printf("The data of user search severity %s : \n", user_search_severity);
                                    printf("       |------- CVE Code -----> %d\n",cv[i].cve_code);
                                }
                            }
                            break;
                        }
                        case 3: {
                            int user_search_priority;
                            printf("Enter the priority for list cve code : ");
                            scanf("%d", &user_search_priority);

                            for (int i = 1; i <= 4; i++) {
                                if (user_search_priority == cv[i].priority_scale) {
                                    printf("The data of user priority %d : \n", cv[i].priority_scale);
                                    printf("       |------- CVE Code -----> %d\n",cv[i].cve_code);
                                }
                            }
                            break;
                        }
                        default: {
                            loop--;
                        }
                    }
                }

                break;
            }

            default: {
                start = 0;
                printf("Thank you....\n");
            }
        }
    }

    return 0;
}