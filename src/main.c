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

typedef struct Expense {
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
  char *salt;
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


void addIncome(User *user, char *incomeName, long long cents) {
  Income *current = user->incomes;
  Income *newIncome = calloc(1, sizeof(*newIncome));

  newIncome->name = strdup(incomeName);
  newIncome->money = cents;
  newIncome->next = NULL;
  if (current == NULL) {
    user->incomes = newIncome;
    return;
  }

  while (current->next != NULL) {
    current = current->next;
  }

  current->next = newIncome;
}


long long totalIncome(User *user) {
  Income *current = user->incomes;
  long long total = 0;

  if (current == NULL) {
    return 0;
  }

  while(current->next != NULL) {
    total += current->money;
    current = current->next;
  }
  total += current->money;

  return total;
}


void freeIncomeList(Income *head) {
  while (head) {
    Income *next = head->next;

    free(head->name);
    free(head);
    head = next;
  }
}

void freeExpenseList(Expense *head) {
  while (head) {
    Expense *next = head->next;

    free(head->name);
    free(head);
    head = next;
  }
}

void freeBudgetList(Budget *head) {
  while (head) {
    Budget *next = head->next;

    free(head->name);
    free(head);
    head = next;
  }
}


void freeUser(User *user) {
  if (!user) return;

  freeIncomeList(user->incomes);
  freeExpenseList(user->expenses);
  freeBudgetList(user->budgets);

  free(user->name);
  free(user->passHash);
  free(user->salt);
  free(user);
}


int main(void) {
  User *myUser = createUser("Test", "testPass");
  printf("%s\n", myUser->name);
  addIncome(myUser, "Test", 19000);
  addIncome(myUser, "Test", 19000);
  addIncome(myUser, "Test", 11000);
  printf("%lli\n", totalIncome(myUser));



  printf("Finance App\n");
  freeUser(myUser);
  return 0;
}
