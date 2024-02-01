#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct Book {
    int id;
    char title[50];
    char author[50];
    char publication_date[12];
};

struct Student {
    int id;
    char name[50];
    char class[50];
    int roll_number;
    char book_title[50];
    char issue_date[12];
};

FILE *book_file;
FILE *issue_file;

void add_book();
void list_books();
void remove_book();
void issue_book();
void list_issues();

int main() {
    int choice;

    while(1) {
        system("cls");
        printf("<== Library Management System ==>\n");
        printf("1. Add Book\n");
        printf("2. Books List\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Issued Book List\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 0:
                exit(0);
            case 1:
                add_book();
                break;
            case 2:
                list_books();
                break;
            case 3:
                remove_book();
                break;
            case 4:
                issue_book();
                break;
            case 5:
                list_issues();
                break;
            default:
                printf("Invalid Choice...\n\n");
        }
        printf("Press Any Key To Continue...");
        getch();
    }

    return 0;
}

void add_book() {
    struct Book new_book;
    char publication_date[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(publication_date, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(new_book.publication_date, publication_date);

    book_file = fopen("books.txt", "ab");

    printf("Enter book id: ");
    scanf("%d", &new_book.id);

    printf("Enter book title: ");
    fflush(stdin);
    gets(new_book.title);

    printf("Enter author name: ");
    fflush(stdin);
    gets(new_book.author);

    printf("Book Added Successfully");

    fwrite(&new_book, sizeof(new_book), 1, book_file);
    fclose(book_file);
}

void list_books() {
    struct Book book;

    system("cls");
    printf("<== Available Books ==>\n\n");
    printf("%-10s %-30s %-20s %s\n\n", "Book id", "Book Title", "Author", "Publication Date");

    book_file = fopen("books.txt", "rb");
    while(fread(&book, sizeof(book), 1, book_file) == 1) {
        printf("%-10d %-30s %-20s %s\n", book.id, book.title, book.author, book.publication_date);
    }

    fclose(book_file);
}

void remove_book() {
    int id, found = 0;
    struct Book book;

    system("cls");
    printf("<== Remove Books ==>\n\n");
    printf("Enter Book id to remove: ");
    scanf("%d", &id);

    FILE *temp_file;
    book_file = fopen("books.txt", "rb");
    temp_file = fopen("temp.txt", "wb");

    while(fread(&book, sizeof(book), 1, book_file) == 1) {
        if(id == book.id) {
            found = 1;
        } else {
            fwrite(&book, sizeof(book), 1, temp_file);
        }
    }

    if(found == 1) {
        printf("\n\nDeleted Successfully.");
    } else {
        printf("\n\nRecord Not Found !");
    }

    fclose(book_file);
    fclose(temp_file);

    remove("books.txt");
    rename("temp.txt", "books.txt");
}

void issue_book() {
    struct Student student;
    struct Book book;
    char issue_date[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(issue_date, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(student.issue_date, issue_date);

    int found = 0;

    system("cls");
    printf("<== Issue Books ==>\n\n");

    printf("Enter Book id to issue: ");
    scanf("%d", &student.id);

    book_file = fopen("books.txt", "rb");

    while(fread(&book, sizeof(book), 1, book_file) == 1) {
        if(book.id == student.id) {
            strcpy(student.book_title, book.title);
            found = 1;
            break;
        }
    }

    if(found == 0) {
        printf("No book found with this id\n");
        printf("Please try again...\n\n");
        return;
    }

    issue_file = fopen("issue.txt", "ab");

    printf("Enter Student Name: ");
    fflush(stdin);
    gets(student.name);

    printf("Enter Student Class: ");
    fflush(stdin);
    gets(student.class);

    printf("Enter Student Roll Number: ");
    scanf("%d", &student.roll_number);

    printf("Book Issued Successfully\n\n");

    fwrite(&student, sizeof(student), 1, issue_file);
    fclose(issue_file);
}

void list_issues() {
    struct Student student;

    system("cls");
    printf("<== Book Issue List ==>\n\n");

    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n", "Book id", "Student Name", "Class", "Roll Number", "Book Title", "Issue Date");

    issue_file = fopen("issue.txt", "rb");
    while(fread(&student, sizeof(student), 1, issue_file) == 1) {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", student.id, student.name, student.class, student.roll_number, student.book_title, student.issue_date);
    }

    fclose(issue_file);
}
