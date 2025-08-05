#define _POSIX_C_SOURCE 200809L


#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// Storing money as cents

typedef struct Income{
  char *name;
  long long money;
  struct Income *next;
} Income;

typedef struct {
  char *name;
  long long money;
  struct Expense *next;
} Expense;

typedef struct Budget {
  char *name;
  long long current;
  long long max;
  struct Budget *next;
} Budget;

typedef struct {
  char* name;
  char* passHash;
  Income *incomes;
  Expense *expenses;
  Budget *budgets;
} User;


User *createUser(char *name, char *password) {
  User *newUser = calloc(1, sizeof(*newUser));
  if (!newUser) {
    perror("failed to create new user");
    return NULL;
  }
  newUser->name = strdup(name);
  newUser->passHash = strdup(password);
  newUser->incomes = NULL;
  newUser->expenses = NULL;
  newUser->budgets = NULL;

  return newUser;
}



int main(void) {
  User *myUser = createUser("Test", "testPass");
  printf("%s\n", myUser->name);



  printf("Finance App\n");
  return 0;
}
