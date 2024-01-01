#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COURSE_COUNT 10
#define MAX_TEACHER_COUNT 3
#define MAX_CLASS_COUNT 5
#define MAX_DAY_COUNT 5
#define MAX_CLASS_NAME_LENGTH 50
#define MAX_TIME_FORMAT 6  // HH:MM format

struct Teacher {
    char name[30];
    int hourlyCapacity;
    int totalTaughtHours;
};

struct Course {
    char name[50];
    char responsibleTeacher[30];
    int hours;
};

struct Class {
    char name[MAX_CLASS_NAME_LENGTH];
    int courseCount;
    struct Course courses[MAX_COURSE_COUNT];
};

struct Schedule {
    int order;
    char courseName[50];  // Increased size to accommodate longer course names
    char teacherName[30];
    char className[MAX_CLASS_NAME_LENGTH];
    char day[15];  // Increased size to accommodate longer day names
    char time[MAX_TIME_FORMAT];
    char dayTime[21];  // Adjusted size to accommodate combined day and time
};

const char* dayNames[MAX_DAY_COUNT] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };

void createSchedule(struct Class classes[], int classCount, struct Teacher teachers[], int teacherCount,
    struct Schedule schedule[], int dayCount);

void printSchedule(struct Schedule schedule[], int scheduleCount);

int compareOrder(const void* a, const void* b);

void readFromFile(struct Class classes[], int* classCount, struct Teacher teachers[], int* teacherCount);

void writeToFile(struct Class classes[], int classCount, struct Teacher teachers[], int teacherCount);

void addTeacher(struct Teacher teachers[], int* teacherCount);

void deleteTeacher(struct Teacher teachers[], int* teacherCount);

void updateTeacher(struct Teacher teachers[], int teacherCount);

void showMenu();

int main() {
    struct Teacher teachers[MAX_TEACHER_COUNT];
    struct Class classes[MAX_CLASS_COUNT];
    struct Schedule schedule[MAX_COURSE_COUNT];

    int teacherCount, classCount, dayCount;

    readFromFile(classes, &classCount, teachers, &teacherCount);

    printf("For how many days will the schedule be created? (Max %d): ", MAX_DAY_COUNT);
    scanf("%d", &dayCount);

    if (dayCount <= 0 || dayCount > MAX_DAY_COUNT) {
        printf("Invalid day count. Exiting the program.\n");
        return 1;
    }

    createSchedule(classes, classCount, teachers, teacherCount, schedule, dayCount);

    printf("\nSchedule:\n");
    printSchedule(schedule, classCount);

    int choice;
    do {
        showMenu();
        printf("Enter your choice (0-5): ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addTeacher(teachers, &teacherCount);
            break;
        case 2:
            deleteTeacher(teachers, &teacherCount);
            break;
        case 3:
            updateTeacher(teachers, teacherCount);
            break;
        case 4:
            // Operations related to class, course, and day information
            // You can add relevant functions here.
            break;
        case 5:
            writeToFile(classes, classCount, teachers, teacherCount);
            break;
        case 0:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}

void addTeacher(struct Teacher teachers[], int* teacherCount) {
    if (*teacherCount < MAX_TEACHER_COUNT) {
        printf("Teacher name: ");
        scanf("%s", teachers[*teacherCount].name);
        printf("Hourly Capacity: ");
        scanf("%d", &teachers[*teacherCount].hourlyCapacity);
        teachers[*teacherCount].totalTaughtHours = 0;
        (*teacherCount)++;
        printf("Teacher added.\n");
    }
    else {
        printf("Maximum number of teachers reached.\n");
    }
}

void deleteTeacher(struct Teacher teachers[], int* teacherCount) {
    if (*teacherCount > 0) {
        char nameToDelete[30];
        printf("Name of the teacher to be deleted: ");
        scanf("%s", nameToDelete);

        int indexToDelete = -1;
        for (int i = 0; i < *teacherCount; i++) {
            if (strcmp(teachers[i].name, nameToDelete) == 0) {
                indexToDelete = i;
                break;
            }
        }

        if (indexToDelete != -1) {
            // Additional steps, such as reassigning the courses taught by the deleted teacher to other teachers, should be considered.
            // In this example, we did not address this detail.

            // After deleting the teacher, shift the remaining teachers.
            for (int i = indexToDelete; i < *teacherCount - 1; i++) {
                strcpy(teachers[i].name, teachers[i + 1].name);
                teachers[i].hourlyCapacity = teachers[i + 1].hourlyCapacity;
                teachers[i].totalTaughtHours = teachers[i + 1].totalTaughtHours;
            }

            (*teacherCount)--;
            printf("Teacher deleted.\n");
        }
        else {
            printf("No teacher found with the specified name.\n");
        }
    }
    else {
        printf("No teacher to delete.\n");
    }
}

void updateTeacher(struct Teacher teachers[], int teacherCount) {
    if (teacherCount > 0) {
        char nameToUpdate[30];
        printf("Name of the teacher to be updated: ");
        scanf("%s", nameToUpdate);

        int indexToUpdate = -1;
        for (int i = 0; i < teacherCount; i++) {
            if (strcmp(teachers[i].name, nameToUpdate) == 0) {
                indexToUpdate = i;
                break;
            }
        }

        if (indexToUpdate != -1) {
            printf("New hourly capacity: ");
            scanf("%d", &teachers[indexToUpdate].hourlyCapacity);
            printf("Teacher updated.\n");
        }
        else {
            printf("No teacher found with the specified name.\n");
        }
    }
    else {
        printf("No teacher to update.\n");
    }
}

void showMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Add Teacher\n");
    printf("2. Delete Teacher\n");
    printf("3. Update Teacher\n");
    printf("4. Operations on Schedule\n");
    printf("5. Write to File\n");
    printf("0. Exit\n");
}

void createSchedule(struct Class classes[], int classCount, struct Teacher teachers[], int teacherCount,
    struct Schedule schedule[], int dayCount) {
    // This function performs the necessary calculations and assignments to create the schedule.
    // It does not consider details like the number of students and teachers, which you should address in a real-world application.

    int scheduleIndex = 0;

    // Seed for randomization based on current time
    srand((unsigned int)time(NULL));

    // An example assignment, distributing one course from each class based on the number of students and teachers
    for (int i = 0; i < classCount; i++) {
        for (int j = 0; j < classes[i].courseCount; j++) {
            // Determine the teacher who will teach the course
            int teacherIndex = rand() % teacherCount;
            strcpy(schedule[scheduleIndex].courseName, classes[i].courses[j].name);
            strcpy(schedule[scheduleIndex].className, classes[i].name);
            strcpy(schedule[scheduleIndex].teacherName, teachers[teacherIndex].name);
            schedule[scheduleIndex].order = scheduleIndex + 1;

            // Determine the day and time of the course
            int dayIndex = rand() % dayCount;
            strcpy(schedule[scheduleIndex].day, dayNames[dayIndex]);

            // Generate a random time (hour and minute)
            int hour = rand() % 8 + 8;  // Random hour between 8 and 15
            int minute = rand() % 60;    // Random minute between 0 and 59
            sprintf(schedule[scheduleIndex].time, "%02d:%02d", hour, minute);

            // Check if the course ends before the school's closing time (15:45)
            int courseDuration = classes[i].courses[j].hours;  // Course duration in hours
            if (hour + courseDuration < 15 || (hour + courseDuration == 15 && minute <= 45)) {
                // Course time is valid
                sprintf(schedule[scheduleIndex].dayTime, "%s - Time-%s", schedule[scheduleIndex].day, schedule[scheduleIndex].time);
                scheduleIndex++;
            }
        }
    }

    // Use qsort function to sort the schedule
    qsort(schedule, scheduleIndex, sizeof(struct Schedule), compareOrder);
}

void printSchedule(struct Schedule schedule[], int scheduleCount) {
    // Print the schedule in tabular format
    printf("| %-10s | %-12s | %-15s | %-20s | %-15s |\n", "Order", "Course Name", "Teacher Name", "Class Name", "Day - Time");
    printf("|------------|--------------|-----------------|----------------------|-----------------|\n");
    for (int i = 0; i < scheduleCount; i++) {
        /*printf("| %-10d | %-12s | %-15s | %-20s | %-15s |\n", schedule[i].order, schedule[i].courseName,
            schedule[i].teacherName, schedule[i].className, schedule[i].dayTime);*/
        printf("| %-10d | %-12s | %-15s | %-20s | %-15s |\n", schedule[i].order, schedule[i].courseName,
            schedule[i].teacherName, schedule[i].className, schedule[i].day, schedule[i].time);

    }
}

int compareOrder(const void* a, const void* b) {
    // Comparison function for the qsort function
    return ((struct Schedule*)a)->order - ((struct Schedule*)b)->order;
}

void readFromFile(struct Class classes[], int* classCount, struct Teacher teachers[], int* teacherCount) {
    FILE* file = fopen("school.txt", "r");
    if (file == NULL) {
        printf("File reading error or file not found. A new file will be created.\n");
        printf("Enter the number of classes: ");
        scanf("%d", classCount);

        for (int i = 0; i < *classCount; i++) {
            printf("Enter the class name and number of courses (e.g., A 3): ");
            scanf("%s %d", classes[i].name, &classes[i].courseCount);

            for (int j = 0; j < classes[i].courseCount; j++) {
                printf("Enter the course name, responsible teacher, and hours (e.g., math ali 4): ");
                scanf("%s %s %d", classes[i].courses[j].name, classes[i].courses[j].responsibleTeacher,
                    &classes[i].courses[j].hours);
            }
        }

        printf("Enter the number of teachers: ");
        scanf("%d", teacherCount);

        for (int i = 0; i < *teacherCount; i++) {
            printf("Enter the teacher name and hourly capacity (e.g., ali 10): ");
            scanf("%s %d", teachers[i].name, &teachers[i].hourlyCapacity);
            teachers[i].totalTaughtHours = 0;
        }

        // Write the newly entered information to the file
        writeToFile(classes, *classCount, teachers, *teacherCount);

        return;
    }

    fscanf(file, "%d", classCount);
    for (int i = 0; i < *classCount; i++) {
        fscanf(file, "%s %d", classes[i].name, &classes[i].courseCount);
        for (int j = 0; j < classes[i].courseCount; j++) {
            fscanf(file, "%s %s %d", classes[i].courses[j].name, classes[i].courses[j].responsibleTeacher,
                &classes[i].courses[j].hours);
        }
    }

    fscanf(file, "%d", teacherCount);
    for (int i = 0; i < *teacherCount; i++) {
        fscanf(file, "%s %d", teachers[i].name, &teachers[i].hourlyCapacity);
        teachers[i].totalTaughtHours = 0;
    }

    fclose(file);
}

void writeToFile(struct Class classes[], int classCount, struct Teacher teachers[], int teacherCount) {
    FILE* file = fopen("school.txt", "w");
    if (file == NULL) {
        printf("File writing error.\n");
        exit(1);
    }

    fprintf(file, "%d\n\n", classCount);
    for (int i = 0; i < classCount; i++) {
        fprintf(file, "%s %d\n", classes[i].name, classes[i].courseCount);
        for (int j = 0; j < classes[i].courseCount; j++) {
            fprintf(file, "%s %s %d\n", classes[i].courses[j].name, classes[i].courses[j].responsibleTeacher,
                classes[i].courses[j].hours);
        }
    }

    fprintf(file, "%d\n\n", teacherCount);
    for (int i = 0; i < teacherCount; i++) {
        fprintf(file, "%s %d\n", teachers[i].name, teachers[i].hourlyCapacity);
    }

    fclose(file);
}
