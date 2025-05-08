#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for maximum lengths
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100
#define MAX_CATEGORY_LENGTH 50
#define MAX_NAME_LENGTH 100
#define MAX_EMAIL_LENGTH 100
#define MAX_PHONE_LENGTH 20

// Structures
typedef struct Book {
  int id;
  char title[MAX_TITLE_LENGTH];
  char author[MAX_AUTHOR_LENGTH];
  char category[MAX_CATEGORY_LENGTH];
  struct Book *next;
} Book;
typedef struct User {
  int id;
  char name[MAX_NAME_LENGTH];
  char email[MAX_EMAIL_LENGTH];
  char phone[MAX_PHONE_LENGTH];
  struct User *next;
} User;

// Function Prototypes
void displayMainMenu();
Book *createBookNode(int id, char *title, char *author, char *category);
void insertBook(Book **head, int id, char *title, char *author, char *category);
int deleteBook(Book **head, int id);
Book *searchBook(Book *head, int id);
void viewAllBooks(Book *head);
User *createUserNode(int id, char *name, char *email, char *phone);
void insertUser(User **head, int id, char *name, char *email, char *phone);
int deleteUser(User **head, int id);
User *searchUser(User *head, int id);
void viewAllUsers(User *head);
void clearInputBuffer();
int loadBooks(Book **head);
int saveBooks(Book *head);
int loadUsers(User **head);
int saveUsers(User *head);
int updateBook(Book *head, int id, char *title, char *author, char *category);
void searchBookByCategory(Book *head, char *category);
void freeBooks(Book *head);
void freeUsers(User *head);

// Main Function
int main() {
  Book *bookHead = NULL;
  User *userHead = NULL;
  int choice;

  if (loadBooks(&bookHead) != 0) printf("Error loading books.\n");
  if (loadUsers(&userHead) != 0) printf("Error loading users.\n");

  do {
    displayMainMenu();
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
      clearInputBuffer();
      printf("Invalid input. Please enter a number.\n");
      continue;
    }
    clearInputBuffer();

    if (choice == 1) {
      char title[MAX_TITLE_LENGTH], author[MAX_AUTHOR_LENGTH], category[MAX_CATEGORY_LENGTH];
      printf("Enter Book Title: ");
      fgets(title, sizeof(title), stdin);
      title[strcspn(title, "\n")] = '\0';

      printf("Enter Author: ");
      fgets(author, sizeof(author), stdin);
      author[strcspn(author, "\n")] = '\0';

      printf("Enter Category: ");
      fgets(category, sizeof(category), stdin);
      category[strcspn(category, "\n")] = '\0';

      int newId = (bookHead ? bookHead->id + 1 : 1);
      insertBook(&bookHead, newId, title, author, category);
      saveBooks(bookHead);
    } else if (choice == 2) {
      viewAllBooks(bookHead);
    } else if (choice == 3) {
      int bookId;
      printf("Enter Book ID to delete: ");
      if (scanf("%d", &bookId) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        continue;
      }
      clearInputBuffer();
      if (deleteBook(&bookHead, bookId) == 0) {
        printf("Book deleted.\n");
        saveBooks(bookHead);
      } else {
        printf("Book not found.\n");
      }
    } else if (choice == 4) {
      char name[MAX_NAME_LENGTH], email[MAX_EMAIL_LENGTH], phone[MAX_PHONE_LENGTH];
      printf("Enter User Name: ");
      fgets(name, sizeof(name), stdin);
      name[strcspn(name, "\n")] = '\0';

      printf("Enter Email: ");
      fgets(email, sizeof(email), stdin);
      email[strcspn(email, "\n")] = '\0';

      printf("Enter Phone: ");
      fgets(phone, sizeof(phone), stdin);
      phone[strcspn(phone, "\n")] = '\0';

      int newId = (userHead ? userHead->id + 1 : 1);
      insertUser(&userHead, newId, name, email, phone);
      saveUsers(userHead);
    } else if (choice == 5) {
      viewAllUsers(userHead);
    } else if (choice == 6) {
      int userId;
      printf("Enter User ID to delete: ");
      if (scanf("%d", &userId) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        continue;
      }
      clearInputBuffer();
      if (deleteUser(&userHead, userId) == 0) {
        printf("User deleted.\n");
        saveUsers(userHead);
      } else {
        printf("User not found.\n");
      }
    } else if (choice == 7) {
      int bookId;
      char title[MAX_TITLE_LENGTH], author[MAX_AUTHOR_LENGTH], category[MAX_CATEGORY_LENGTH];
      printf("Enter Book ID to update: ");
      if (scanf("%d", &bookId) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        continue;
      }
      clearInputBuffer();

      printf("Enter New Title: ");
      fgets(title, sizeof(title), stdin);
      title[strcspn(title, "\n")] = '\0';

      printf("Enter New Author: ");
      fgets(author, sizeof(author), stdin);
      author[strcspn(author, "\n")] = '\0';

      printf("Enter New Category: ");
      fgets(category, sizeof(category), stdin);
      category[strcspn(category, "\n")] = '\0';

      if (updateBook(bookHead, bookId, title, author, category) == 0) {
        printf("Book updated.\n");
        saveBooks(bookHead);
      } else {
        printf("Book not found.\n");
      }
    } else if (choice == 8) {
      char category[MAX_CATEGORY_LENGTH];
      printf("Enter Category to search: ");
      fgets(category, sizeof(category), stdin);
      category[strcspn(category, "\n")] = '\0';
      searchBookByCategory(bookHead, category);
    } else if (choice == 9) {
      printf("Exiting program.\n");
    } else {
      printf("Invalid choice.\n");
    }
  } while (choice != 9);

  freeBooks(bookHead);
  freeUsers(userHead);
  return 0;
}

// UI
void displayMainMenu() {
  printf("\nMain Menu\n");
  printf("1. Add New Book\n");
  printf("2. View All Books\n");
  printf("3. Delete a Book\n");
  printf("4. Register New User\n");
  printf("5. View All Users\n");
  printf("6. Delete a User\n");
  printf("7. Update a Book\n");
  printf("8. Search Book by Category\n");
  printf("9. Exit\n");
}

// Book Functions
Book *createBookNode(int id, char *title, char *author, char *category) {
  Book *node = malloc(sizeof(Book));
  if (!node) return NULL;
  node->id = id;
  strcpy(node->title, title);
  strcpy(node->author, author);
  strcpy(node->category, category);
  node->next = NULL;
  return node;
}

void insertBook(Book **head, int id, char *title, char *author, char *category) {
  Book *newNode = createBookNode(id, title, author, category);
  if (!newNode) return;
  if (*head == NULL) {
    *head = newNode;
  } else {
    Book *curr = *head;
    while (curr->next) curr = curr->next;
    curr->next = newNode;
  }
}

int deleteBook(Book **head, int id) {
  Book *curr = *head, *prev = NULL;
  while (curr && curr->id != id) {
    prev = curr;
    curr = curr->next;
  }
  if (!curr) return -1;
  if (!prev) *head = curr->next;
  else prev->next = curr->next;
  free(curr);
  return 0;
}

Book *searchBook(Book *head, int id) {
  while (head) {
    if (head->id == id) return head;
    head = head->next;
  }
  return NULL;
}

void viewAllBooks(Book *head) {
  if (!head) {
    printf("No books available.\n");
    return;
  }
  printf("ID\tTitle\tAuthor\tCategory\n");
  while (head) {
    printf("%d\t%s\t%s\t%s\n", head->id, head->title, head->author, head->category);
    head = head->next;
  }
}

int updateBook(Book *head, int id, char *title, char *author, char *category) {
  Book *book = searchBook(head, id);
  if (!book) return -1;
  strcpy(book->title, title);
  strcpy(book->author, author);
  strcpy(book->category, category);
  return 0;
}

void searchBookByCategory(Book *head, char *category) {
  int found = 0;
  printf("Books in category '%s':\n", category);
  while (head) {
    if (strcmp(head->category, category) == 0) {
      printf("%d\t%s\t%s\t%s\n", head->id, head->title, head->author, head->category);
      found = 1;
    }
    head = head->next;
  }
  if (!found) printf("No books found in category '%s'.\n", category);
}

// User Functions
User *createUserNode(int id, char *name, char *email, char *phone) {
  User *node = malloc(sizeof(User));
  if (!node) return NULL;
  node->id = id;
  strcpy(node->name, name);
  strcpy(node->email, email);
  strcpy(node->phone, phone);
  node->next = NULL;
  return node;
}

void insertUser(User **head, int id, char *name, char *email, char *phone) {
  User *newNode = createUserNode(id, name, email, phone);
  if (!newNode) return;
  if (*head == NULL) {
    *head = newNode;
  } else {
    User *curr = *head;
    while (curr->next) curr = curr->next;
    curr->next = newNode;
  }
}

int deleteUser(User **head, int id) {
  User *curr = *head, *prev = NULL;
  while (curr && curr->id != id) {
    prev = curr;
    curr = curr->next;
  }
  if (!curr) return -1;
  if (!prev) *head = curr->next;
  else prev->next = curr->next;
  free(curr);
  return 0;
}

void viewAllUsers(User *head) {
  if (!head) {
    printf("No users registered.\n");
    return;
  }
  printf("ID\tName\tEmail\tPhone\n");
  while (head) {
    printf("%d\t%s\t%s\t%s\n", head->id, head->name, head->email, head->phone);
    head = head->next;
  }
}

// File I/O
int saveBooks(Book *head) {
  FILE *file = fopen("books.csv", "w");
  if (!file) return -1;
  while (head) {
    fprintf(file, "%d,%s,%s,%s\n", head->id, head->title, head->author, head->category);
    head = head->next;
  }
  fclose(file);
  return 0;
}

int loadBooks(Book **head) {
  FILE *file = fopen("books.csv", "r");
  if (!file) return -1;

  char line[512];
  while (fgets(line, sizeof(line), file)) {
    int id;
    char title[MAX_TITLE_LENGTH], author[MAX_AUTHOR_LENGTH], category[MAX_CATEGORY_LENGTH];
    if (sscanf(line, "%d,%99[^,],%99[^,],%49[^\n]", &id, title, author, category) == 4) {
      insertBook(head, id, title, author, category);
    }
  }
  fclose(file);
  return 0;
}

int saveUsers(User *head) {
  FILE *file = fopen("users.csv", "w");
  if (!file) return -1;
  while (head) {
    fprintf(file, "%d,%s,%s,%s\n", head->id, head->name, head->email, head->phone);
    head = head->next;
  }
  fclose(file);
  return 0;
}

int loadUsers(User **head) {
  FILE *file = fopen("users.csv", "r");
  if (!file) return -1;

  char line[512];
  while (fgets(line, sizeof(line), file)) {
    int id;
    char name[MAX_NAME_LENGTH], email[MAX_EMAIL_LENGTH], phone[MAX_PHONE_LENGTH];
    if (sscanf(line, "%d,%99[^,],%99[^,],%19[^\n]", &id, name, email, phone) == 4) {
      insertUser(head, id, name, email, phone);
    }
  }
  fclose(file);
  return 0;
}

// Helpers
void clearInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void freeBooks(Book *head) {
  while (head) {
    Book *temp = head;
    head = head->next;
    free(temp);
  }
}

void freeUsers(User *head) {
  while (head) {
    User *temp = head;
    head = head->next;
    free(temp);
  }
}
