#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "searchReport.h"

const char* caseTypeToString(CaseType type) {
    switch (type) {
        case EMERGENCY: return "Emergency";
        case NORMAL: return "Normal";
        case ROUTINE: return "Routine";
        case CONSULTATION: return "Consultation";
        default: return "Unknown";
    }
}

const char* statusToString(Status status) {
    switch (status) {
        case WAITING: return "Waiting";
        case EXAMINING: return "Examining";
        case FINISHED: return "Finished";
        default: return "Unknown";
    }
}

void searchByIDCard(PatientList* list, char* IDCard) {
    if (list == NULL || list->head == NULL || IDCard == NULL) {
        printf("Invalid input.\n");
        return;
    }

    int found = 0;
    PatientNode* current = list->head;
    
    while (current != NULL) {
        if (strcmp(current->patient->IDCard, IDCard) == 0) {
            if (!found) {
                printf("+------------+----------------------+------------+------------+---------------------+----------------+---------------------+---------------------+\n");
                printf("| ID         | Name                 | Birth Year | Status     | Arrival Time        | Case Type      | Examining Time      | Finished Time       |\n");
                printf("+------------+----------------------+------------+------------+---------------------+----------------+---------------------+---------------------+\n");
            }
            if (current->patient->examiningEndTime != 0) {
                current = current->next;
                continue;
            }
            char arrivalBuffer[100], startBuffer[100], endBuffer[100];
            strftime(arrivalBuffer, sizeof(arrivalBuffer), "%d-%m-%Y %H:%M:%S", localtime(&current->patient->arrivalTime));
            strftime(startBuffer, sizeof(startBuffer), "%d-%m-%Y %H:%M:%S", localtime(&current->patient->examiningStartTime));
            strftime(endBuffer, sizeof(endBuffer), "%d-%m-%Y %H:%M:%S", localtime(&current->patient->examiningEndTime));

            printf("| %-10s | %-20s | %-10d | %-10s | %-19s | %-14s | %-19s | %-19s |\n",
                   current->patient->id,
                   current->patient->name,
                   current->patient->year,
                   statusToString(current->patient->status),
                   arrivalBuffer,
                   caseTypeToString(current->patient->caseType),
                   current->patient->examiningStartTime == 0 ? "Not started" : startBuffer,
                   current->patient->examiningEndTime == 0 ? "Not finished" : endBuffer);
            printf("+------------+----------------------+------------+------------+---------------------+----------------+---------------------+---------------------+\n");
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No patient found with ID Card \"%s\".\n", IDCard);
    }
}

Patient* searchByID(PatientList* list, char* id) {
    if (list == NULL || list->head == NULL || id == NULL) {
        return NULL;
    }

    PatientNode* current = list->head;
    while (current != NULL) {
        if (strcmp(current->patient->id, id) == 0) {
            if (current->patient->examiningEndTime == 0) {
                return current->patient;
            } else {
                return NULL;
            }
        }
        current = current->next;
    }

    return NULL;
}

void showAllPatients(PatientList *list) {
    if (list == NULL) {
        printf("Memory allocation failed\n");
    }
    PatientNode *current = list->head;
    printf("+------------+----------------------+------------+------------+---------------------+----------------+---------------------+---------------------+\n");
    printf("| ID         | Name                 | Birth Year | Status     | Arrival Time        | Case Type      | Examining Time      | Finished Time       |\n");
    printf("+------------+----------------------+------------+------------+---------------------+----------------+---------------------+---------------------+\n");
    while (current != NULL) {
        if (current->patient->examiningEndTime != 0) {
            current = current->next;
            continue;
        }
        char arrivalBuffer[100], startBuffer[100], endBuffer[100];
        strftime(arrivalBuffer, sizeof(arrivalBuffer), "%d-%m-%Y %H:%M:%S", localtime(&current->patient->arrivalTime));
        strftime(startBuffer, sizeof(startBuffer), "%d-%m-%Y %H:%M:%S", localtime(&current->patient->examiningStartTime));
        strftime(endBuffer, sizeof(endBuffer), "%d-%m-%Y %H:%M:%S", localtime(&current->patient->examiningEndTime));

        printf("| %-10s | %-20s | %-10d | %-10s | %-19s | %-14s | %-19s | %-19s |\n",
            current->patient->id,
            current->patient->name,
            current->patient->year,
            statusToString(current->patient->status),
            arrivalBuffer,
            caseTypeToString(current->patient->caseType),
            current->patient->examiningStartTime == 0 ? "Not started" : startBuffer,
            current->patient->examiningEndTime == 0 ? "Not finished" : endBuffer);
        printf("+------------+----------------------+------------+------------+---------------------+----------------+---------------------+---------------------+\n");
        current = current->next;
    }
}

void searchVisitHistoryByIDCard(historyList *list, const char *IDCard) {
    if (list == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    historyNode *current = list->head;
    printf("+------------------------+--------------+----------------------+------------+---------------------+---------------------+---------------------+\n");
    printf("| Visited ID             | IDCard       | Name                 | Birth Year | Arrival Time        | Examining Time      | Finished Time       |\n");
    printf("+------------------------+--------------+----------------------+------------+---------------------+---------------------+---------------------+\n");
    while (current != NULL) {
        if (strcmp(current->data.IDCard, IDCard) == 0) {
            char arrivalBuffer[100], startBuffer[100], endBuffer[100];
            strftime(arrivalBuffer, sizeof(arrivalBuffer), "%d-%m-%Y %H:%M:%S", localtime(&current->data.arrivalTime));
            strftime(startBuffer, sizeof(startBuffer), "%d-%m-%Y %H:%M:%S", localtime(&current->data.examiningStartTime));
            strftime(endBuffer, sizeof(endBuffer), "%d-%m-%Y %H:%M:%S", localtime(&current->data.examiningEndTime));
            printf("| %-22s | %-12s | %-20s | %-10d | %-19s | %-19s | %-19s |\n",
                current->data.visitedID,
                current->data.IDCard,
                current->data.name,
                current->data.year,
                arrivalBuffer,
                startBuffer,
                endBuffer);
        }
        current = current->next;
    }
}

void freeList(historyList *list) {
    if (list == NULL) {
        return;
    }
    historyNode *current = list->head;
    while (current != NULL) {
        historyNode *tmp = current;
        current = current->next;
        free(tmp);
    }
    list->head = NULL;
    list->tail = NULL;
    free(list);
}