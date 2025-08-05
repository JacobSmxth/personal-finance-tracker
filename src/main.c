#define _POSIX_C_SOURCE 200809L


#include <argon2.h>
#include <time.h>
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


void generate_random_string(char *str, size_t length) {
  const char charset[] = "abcedefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  if (length) {
    for (size_t n = 0; n < length; n++) {
      int key = rand() % (int)(sizeof(charset) - 1);
      str[n] = charset[key];
    }
    str[length] = '\0';
  }
}



User *createUser(const char *name, const char *password) {
  User *newUser = calloc(1, sizeof(*newUser));

  srand(time(NULL));
  size_t length = 20;
  char salt[length + 1];
  generate_random_string(salt, length);

  char hash[128];
  char encoded[256];

  long t_cost = 2;
  long m_cost = 1 << 16;
  long parallelism = 1;
  long hashlen = 32;

  int result = argon2_hash(t_cost, m_cost, parallelism, password, strlen(password), salt, strlen(salt), hash, hashlen, encoded, sizeof(encoded), Argon2_id, ARGON2_VERSION_13);

  if (result == ARGON2_OK) {
    printf("Encoded hash: %s\n", encoded);
  } else {
    fprintf(stderr, "error: %s\n", argon2_error_message(result));
  }

  if (!newUser) {
    perror("failed to create new user");
    return NULL;
  }
  newUser->name = strdup(name);
  newUser->passHash = strdup(encoded);
  newUser->salt = strdup(salt);
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
