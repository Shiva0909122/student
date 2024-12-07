#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store student details
typedef struct {
    int rollNumber;
    char name[50];
    float marks;
} Student;

// Function declarations
void addStudent();
void viewStudents();
void updateStudent();
void deleteStudent();

// File to store student records
const char *filename = "students.dat";

int main() {
    int choice;
    while (1) {
        printf("\n--- Student Record Management System ---\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: exit(0); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Function to add a new student record
void addStudent() {
    FILE *file = fopen(filename, "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    Student student;
    printf("Enter Roll Number: ");
    scanf("%d", &student.rollNumber);
    printf("Enter Name: ");
    scanf(" %[^\n]", student.name);
    printf("Enter Marks: ");
    scanf("%f", &student.marks);

    fwrite(&student, sizeof(Student), 1, file);
    fclose(file);
    printf("Student record added successfully!\n");
}

// Function to view all student records
void viewStudents() {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    Student student;
    printf("\n--- Student Records ---\n");
    while (fread(&student, sizeof(Student), 1, file)) {
        printf("Roll Number: %d\n", student.rollNumber);
        printf("Name: %s\n", student.name);
        printf("Marks: %.2f\n", student.marks);
        printf("------------------------\n");
    }
    fclose(file);
}

// Function to update a student record
void updateStudent() {
    FILE *file = fopen(filename, "rb+");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    int rollNumber, found = 0;
    Student student;

    printf("Enter Roll Number to update: ");
    scanf("%d", &rollNumber);

    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.rollNumber == rollNumber) {
            found = 1;
            printf("Enter new Name: ");
            scanf(" %[^\n]", student.name);
            printf("Enter new Marks: ");
            scanf("%f", &student.marks);

            fseek(file, -sizeof(Student), SEEK_CUR);
            fwrite(&student, sizeof(Student), 1, file);
            printf("Student record updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Student with Roll Number %d not found!\n", rollNumber);
    }
    fclose(file);
}

// Function to delete a student record
void deleteStudent() {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    int rollNumber, found = 0;
    Student student;

    printf("Enter Roll Number to delete: ");
    scanf("%d", &rollNumber);

    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.rollNumber == rollNumber) {
            found = 1;
            continue;  // Skip the record to be deleted
        }
        fwrite(&student, sizeof(Student), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("temp.dat", filename);

    if (found) {
        printf("Student record deleted successfully!\n");
    } else {
        printf("Student with Roll Number %d not found!\n", rollNumber);
    }
}