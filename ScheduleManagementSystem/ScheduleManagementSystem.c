#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <jansson.h>
#include <utf8proc.h>

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
    char courseName[50];
    char teacherName[30];
    char className[MAX_CLASS_NAME_LENGTH];
    char day[15];
    char time[MAX_TIME_FORMAT];
    char dayTime[21];
};

const char* dayNames[MAX_DAY_COUNT] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };

void createSchedule(struct Class classes[], int classCount, struct Teacher teachers[], int teacherCount,
    struct Schedule schedule[], int dayCount);

void printSchedule(struct Schedule schedule[], int scheduleCount);

int compareOrder(const void* a, const void* b);

void readFromJsonFile(struct Class classes[], int* classCount, struct Teacher teachers[], int* teacherCount);//*
void writeToJsonFile(struct Class classes[], int classCount, struct Teacher teachers[], int teacherCount);//*

void addTeacher(struct Teacher teachers[], int* teacherCount);
void deleteTeacher(struct Teacher teachers[], int* teacherCount);
void updateTeacher(struct Teacher teachers[], int teacherCount);

void showMenu();

int main() {
    struct Teacher teachers[MAX_TEACHER_COUNT];
    struct Class classes[MAX_CLASS_COUNT];
    struct Schedule schedule[MAX_COURSE_COUNT];

    int teacherCount, classCount, dayCount;

    readFromJsonFile(classes, &classCount, teachers, &teacherCount);

    printf("For how many days will the schedule be created? (Max %d): ", MAX_DAY_COUNT);
    scanf("%d", &dayCount);

    if (dayCount <= 0 || dayCount > MAX_DAY_COUNT) {
        printf("Invalid day count. Exiting the program.\n");
        return 1;
    }

    createSchedule(classes, MAX_COURSE_COUNT, teachers, teacherCount, schedule, dayCount);

    printf("\nSchedule:\n");
    printSchedule(schedule, MAX_COURSE_COUNT);

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
            writeToJsonFile(classes, classCount, teachers, teacherCount);
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
        printf("Öðretmenin adý: ");
        scanf("%s", teachers[*teacherCount].name);
        printf("Saatlik Kapasite: ");
        scanf("%d", &teachers[*teacherCount].hourlyCapacity);
        teachers[*teacherCount].totalTaughtHours = 0;
        (*teacherCount)++;
        printf("Öðretmen eklendi.\n");
    }
    else {
        printf("Maksimum öðretmen sayýsýna ulaþýldý.\n");
    }
}

void deleteTeacher(struct Teacher teachers[], int* teacherCount) {
    if (*teacherCount > 0) {
        char nameToDelete[30];
        printf("Silinecek öðretmenin adý: ");
        scanf("%s", nameToDelete);

        int indexToDelete = -1;
        for (int i = 0; i < *teacherCount; i++) {
            if (strcmp(teachers[i].name, nameToDelete) == 0) {
                indexToDelete = i;
                break;
            }
        }

        if (indexToDelete != -1) {
            // Ek adýmlar, silinen öðretmen tarafýndan verilen derslerin diðer öðretmenlere atanmasý gibi, düþünülmelidir.
            // Bu örnekte, bu detayý ele almadýk.

            // Öðretmen silindikten sonra kalan öðretmenleri kaydýr
            for (int i = indexToDelete; i < *teacherCount - 1; i++) {
                strcpy(teachers[i].name, teachers[i + 1].name);
                teachers[i].hourlyCapacity = teachers[i + 1].hourlyCapacity;
                teachers[i].totalTaughtHours = teachers[i + 1].totalTaughtHours;
            }

            (*teacherCount)--;
            printf("Öðretmen silindi.\n");
        }
        else {
            printf("Belirtilen adla öðretmen bulunamadý.\n");
        }
    }
    else {
        printf("Silinecek öðretmen yok.\n");
    }
}

void updateTeacher(struct Teacher teachers[], int teacherCount) {
    if (teacherCount > 0) {
        char nameToUpdate[30];
        printf("Güncellenecek öðretmenin adý: ");
        scanf("%s", nameToUpdate);

        int indexToUpdate = -1;
        for (int i = 0; i < teacherCount; i++) {
            if (strcmp(teachers[i].name, nameToUpdate) == 0) {
                indexToUpdate = i;
                break;
            }
        }

        if (indexToUpdate != -1) {
            printf("Yeni saatlik kapasite: ");
            scanf("%d", &teachers[indexToUpdate].hourlyCapacity);
            printf("Öðretmen güncellendi.\n");
        }
        else {
            printf("Belirtilen adla öðretmen bulunamadý.\n");
        }
    }
    else {
        printf("Güncellenecek öðretmen yok.\n");
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
    // Bu fonksiyon, rastgele bir program oluþturacak þekilde tasarlanmýþtýr.
    // Gerçek uygulamalarda, öðretmen ve sýnýf sayýlarý gibi faktörler daha ayrýntýlý bir plan oluþturmayý gerektirebilir.

    int scheduleIndex = 0;

    // Rastgele sayý üretimi için zamanýn kullanýlmasý
    srand((unsigned int)time(NULL));

    for (int i = 0; i < classCount; i++) {
        for (int j = 0; j < classes[i].courseCount; j++) {
            // Rastgele bir öðretmenin seçimi
            int teacherIndex = rand() % teacherCount;

            // Program bilgilerinin schedule dizisine atanmasý
            strcpy(schedule[scheduleIndex].courseName, classes[i].courses[j].name);
            strcpy(schedule[scheduleIndex].className, classes[i].name);
            strcpy(schedule[scheduleIndex].teacherName, teachers[teacherIndex].name);
            schedule[scheduleIndex].order = scheduleIndex + 1;

            // Rastgele bir gün seçimi
            int dayIndex = rand() % dayCount;
            strcpy(schedule[scheduleIndex].day, dayNames[dayIndex]);

            // Rastgele bir saat üretimi (saat ve dakika)
            int hour = rand() % 8 + 8;  // 8 ile 15 arasýnda rastgele bir saat
            int minute = rand() % 60;    // 0 ile 59 arasýnda rastgele bir dakika
            sprintf(schedule[scheduleIndex].time, "%02d:%02d", hour, minute);

            // Dersin okulun kapanma saatinden (15:45) önce bitip bitmediðinin kontrolü
            int courseDuration = classes[i].courses[j].hours;  // Ders süresi
            if (hour + courseDuration < 15 || (hour + courseDuration == 15 && minute <= 45)) {
                // Ders saati uygunsa
                sprintf(schedule[scheduleIndex].dayTime, "%s - Saat-%s", schedule[scheduleIndex].day, schedule[scheduleIndex].time);
                scheduleIndex++;
            }
        }
    }

    // Programýn sýralanmasý için qsort fonksiyonu kullanýlýr
    qsort(schedule, scheduleIndex, sizeof(struct Schedule), compareOrder);
}

void printSchedule(struct Schedule schedule[], int scheduleCount) {
    // Programý tablo formatýnda ekrana yazdýrma
    printf("| %-10s | %-12s | %-15s | %-20s | %-15s |\n", "Sýra", "Ders Adý", "Öðretmen Adý", "Sýnýf Adý", "Gün - Saat");
    printf("|------------|--------------|-----------------|----------------------|-----------------|\n");
    for (int i = 0; i < scheduleCount; i++) {
        printf("| %-10d | %-12s | %-15s | %-20s | %-15s |\n", schedule[i].order, schedule[i].courseName,
            schedule[i].teacherName, schedule[i].className, schedule[i].dayTime);
    }
}

int compareOrder(const void* a, const void* b) {
    // qsort fonksiyonu için karþýlaþtýrma fonksiyonu
    return ((struct Schedule*)a)->order - ((struct Schedule*)b)->order;
}



void readFromJsonFile(struct Class classes[], int* classCount, struct Teacher teachers[], int* teacherCount) {
    json_t* root, * classesArray, * teachersArray;
    json_error_t error;
    int i, j;

    // Read JSON content from file
    FILE* file = fopen("school.json", "r");

    if (!file) {
        // Dosya bulunamadýðýnda yeni bir dosya oluþturun
        printf("Dosya bulunamadý. Yeni bir dosya oluþturuluyor...\n");
        writeToJsonFile(classes, *classCount, teachers, *teacherCount);

        // Yeniden okuma iþlemi için dosyayý açýn
        file = fopen("school.json", "r");
        if (!file) {
            fprintf(stderr, "Could not open file for reading.\n");
            perror("fopen");
            exit(1);
        }
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    fclose(file);

    buffer[file_size] = '\0';

    // Türkçe karakterleri düzelten iþlem
    char* normalizedBuffer = utf8proc_NFC((uint8_t*)buffer);
    free(buffer);

    // Parse JSON data
    root = json_loads(normalizedBuffer, 0, &error);
    free(normalizedBuffer);

    if (!root) {
        fprintf(stderr, "JSON parsing error: %s\n", error.text);
        exit(1);
    }

    // Classes array
    classesArray = json_object_get(root, "classes");
    if (!json_is_array(classesArray)) {
        fprintf(stderr, "Invalid JSON format: 'classes' is not an array.\n");
        exit(1);
    }
    *classCount = json_array_size(classesArray);

    for (i = 0; i < *classCount; i++) {
        json_t* classObj, * coursesArray;

        // Class object
        classObj = json_array_get(classesArray, i);
        strcpy(classes[i].name, json_string_value(json_object_get(classObj, "name")));
        classes[i].courseCount = json_integer_value(json_object_get(classObj, "courseCount"));

        // Courses array
        coursesArray = json_object_get(classObj, "courses");
        if (!json_is_array(coursesArray)) {
            fprintf(stderr, "Invalid JSON format: 'courses' is not an array.\n");
            exit(1);
        }
        int courseCount = json_array_size(coursesArray);

        for (j = 0; j < courseCount; j++) {
            json_t* courseObj;

            // Course object
            courseObj = json_array_get(coursesArray, j);
            strcpy(classes[i].courses[j].name, json_string_value(json_object_get(courseObj, "name")));
            strcpy(classes[i].courses[j].responsibleTeacher, json_string_value(json_object_get(courseObj, "responsibleTeacher")));
            classes[i].courses[j].hours = json_integer_value(json_object_get(courseObj, "hours"));
        }
    }

    // Teachers array
    teachersArray = json_object_get(root, "teachers");
    if (!json_is_array(teachersArray)) {
        fprintf(stderr, "Invalid JSON format: 'teachers' is not an array.\n");
        exit(1);
    }
    *teacherCount = json_array_size(teachersArray);

    for (i = 0; i < *teacherCount; i++) {
        json_t* teacherObj;

        // Teacher object
        teacherObj = json_array_get(teachersArray, i);
        strcpy(teachers[i].name, json_string_value(json_object_get(teacherObj, "name")));
        teachers[i].hourlyCapacity = json_integer_value(json_object_get(teacherObj, "hourlyCapacity"));
        teachers[i].totalTaughtHours = json_integer_value(json_object_get(teacherObj, "totalTaughtHours"));
    }

    // Cleanup
    json_decref(root);
}

void writeToJsonFile(struct Class classes[], int classCount, struct Teacher teachers[], int teacherCount) {
    json_t* root, * classesArray, * teachersArray;
    int i, j;

    // Root JSON object
    root = json_object();

    // Classes array
    classesArray = json_array();
    for (i = 0; i < classCount; i++) {
        json_t* classObj, * coursesArray;

        // Class object
        classObj = json_object();
        json_object_set_new(classObj, "name", json_string(classes[i].name));
        json_object_set_new(classObj, "courseCount", json_integer(classes[i].courseCount));

        // Courses array
        coursesArray = json_array();
        for (j = 0; j < classes[i].courseCount; j++) {
            json_t* courseObj;

            // Course object
            courseObj = json_object();
            json_object_set_new(courseObj, "name", json_string(classes[i].courses[j].name));
            json_object_set_new(courseObj, "responsibleTeacher", json_string(classes[i].courses[j].responsibleTeacher));
            json_object_set_new(courseObj, "hours", json_integer(classes[i].courses[j].hours));

            // Add course object to courses array
            json_array_append_new(coursesArray, courseObj);
        }

        // Add courses array to class object
        json_object_set_new(classObj, "courses", coursesArray);

        // Add class object to classes array
        json_array_append_new(classesArray, classObj);
    }

    // Teachers array
    teachersArray = json_array();
    for (i = 0; i < teacherCount; i++) {
        json_t* teacherObj;

        // Teacher object
        teacherObj = json_object();
        json_object_set_new(teacherObj, "name", json_string(teachers[i].name));
        json_object_set_new(teacherObj, "hourlyCapacity", json_integer(teachers[i].hourlyCapacity));
        json_object_set_new(teacherObj, "totalTaughtHours", json_integer(teachers[i].totalTaughtHours));

        // Add teacher object to teachers array
        json_array_append_new(teachersArray, teacherObj);
    }

    // Add arrays to root object
    json_object_set_new(root, "classes", classesArray);
    json_object_set_new(root, "teachers", teachersArray);

    // Write JSON content to file
    FILE* file = fopen("school.json", "w");
    if (!file) {
        fprintf(stderr, "Could not open file for writing.\n");
        perror("fopen");
        exit(1);
    }

    json_dumpf(root, file, JSON_INDENT(2));

    /*char* jsonStr = json_dumps(root, JSON_INDENT(2));
    fprintf(file, "%s\n", jsonStr);*/

    // Cleanup
    fclose(file);
    //free(jsonStr);
    json_decref(root);
}