#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_SUBJECT_LENGTH 100
#define DATABASE_FILE "students.txt"

typedef struct {
    int rollNumber;
    char name[MAX_NAME_LENGTH];
    char subject[MAX_SUBJECT_LENGTH];
} Student;

void addStudent() {
    FILE *file = fopen(DATABASE_FILE, "a+");
    if (file == NULL) {
        printf("Error opening database file.\n");
        return;
    }

    Student student;
    printf("Enter Roll Number: ");
    scanf("%d", &student.rollNumber);

    printf("Enter Name: ");
    scanf(" %[^\n]s", student.name);

    printf("Enter Subject: ");
    scanf(" %[^\n]s", student.subject);

    fprintf(file, "%d;%s;%s\n", student.rollNumber, student.name, student.subject);
    fclose(file);

    printf("Student added successfully.\n");
}

void searchStudent() {
    FILE *file = fopen(DATABASE_FILE, "r");
    if (file == NULL) {
        printf("Error opening database file.\n");
        return;
    }

    int rollNumber;
    printf("Enter Roll Number to search: ");
    scanf("%d", &rollNumber);

    Student student;
    int found = 0;

    while (fscanf(file, "%d;%[^;];%[^\n]\n", &student.rollNumber, student.name, student.subject) != EOF) {
        if (student.rollNumber == rollNumber) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Student found:\n");
        printf("Roll Number: %d\n", student.rollNumber);
        printf("Name: %s\n", student.name);
        printf("Subject: %s\n", student.subject);
    } else {
        printf("Student not found.\n");
    }
}

void changeStudent() {
    FILE *file = fopen(DATABASE_FILE, "r+");
    if (file == NULL) {
        printf("Error opening database file.\n");
        return;
    }

    int rollNumber;
    printf("Enter Roll Number to change: ");
    scanf("%d", &rollNumber);

    Student student;
    int found = 0;

    long filePosition = ftell(file);

    while (fscanf(file, "%d;%[^;];%[^\n]\n", &student.rollNumber, student.name, student.subject) != EOF) {
        if (student.rollNumber == rollNumber) {
            found = 1;
            break;
        }
        filePosition = ftell(file);
    }

    if (found) {
        printf("Enter new Name: ");
        scanf(" %[^\n]s", student.name);

        printf("Enter new Subject: ");
        scanf(" %[^\n]s", student.subject);

        fseek(file, filePosition, SEEK_SET);
        fprintf(file, "%d;%s;%s\n", student.rollNumber, student.name, student.subject);
        fclose(file);

        printf("Student record changed successfully.\n");
    } else {
        fclose(file);
        printf("Student not found.\n");
    }
}

void removeStudent() {
    FILE *file = fopen(DATABASE_FILE, "r+");
    if (file == NULL) {
        printf("Error opening database file.\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w+");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    int rollNumber;
    printf("Enter Roll Number to remove: ");
    scanf("%d", &rollNumber);

    Student student;
    int found = 0;

    while (fscanf(file, "%d;%[^;];%[^\n]\n", &student.rollNumber, student.name, student.subject) != EOF) {
        if (student.rollNumber != rollNumber) {
            fprintf(tempFile, "%d;%s;%s\n", student.rollNumber, student.name, student.subject);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(DATABASE_FILE);
    rename("temp.txt", DATABASE_FILE);

    if (found) {
        printf("Student record removed successfully.\n");
    } else {
        printf("Student not found.\n");
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n----- Student Management System -----\n");
        printf("1. Add Student\n");
        printf("2. Search Student\n");
        printf("3. Change Student Record\n");
        printf("4. Remove Student Record\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                searchStudent();
                break;
            case 3:
                changeStudent();
                break;
            case 4:
                removeStudent();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
