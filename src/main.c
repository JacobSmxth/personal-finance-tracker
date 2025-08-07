#define _POSIX_C_SOURCE 200809L

#include <argon2.h>
#include <sodium/utils.h>
#include <sodium.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



// Storing money as cents
typedef struct Income{
  int id;
  char *name;
  long long money;
  struct Income *next;
} Income;

typedef struct Expense {
  int id;
  char *name;
  long long money;
  struct Expense *next;
} Expense;

typedef struct Budget {
  int id;
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


// Create User & info
User *createUser(const char *name, const char *password);
void createIncome(User *user, char *incomeName, long long cents);
void createExpense(User *user, char *expenseName, long long cents);
void createBudget(User *user, char *budgetName, long long max);

// Adjust User info
void addToBudget(User *user, int id, long long amount);
void resetBudget(User *user, int id);

// Utility functions
void centsToString(char *str, long long cents);

// Test Functions
void userOverview(User *user);

// Teardown functions functions
void freeIncomeList(Income *head);
void freeExpenseList(Expense *head);
void freeBudgetList(Budget *head);
void freeUser(User *user);

// Main Function
int main(void) {
  // Check for any failures in the salt library initializing
  if (sodium_init() < 0) {
    fputs("libsodium init failed\n", stderr);
    return 1;
  }

  User *myUser = createUser("Jacob Smith", "testPass");
  createBudget(myUser, "Grocery", 80000);
  addToBudget(myUser, 0, 75121);
  userOverview(myUser);

  resetBudget(myUser, 0);

  userOverview(myUser);

  freeUser(myUser);
  return 0;
}


User *createUser(const char *name, const char *password) {
  User *newUser = calloc(1, sizeof(*newUser));
  if (!newUser) {
    perror("failed to create new user");
    return NULL;
  }

  // Generate a salt to avoid Rainbow table attacks or whatever
  uint8_t salt[crypto_pwhash_SALTBYTES];
  randombytes_buf(salt, sizeof(salt));
  char theSalt[(sizeof(salt) * 4/3) + 4];
  sodium_bin2base64(theSalt, sizeof(theSalt), salt, sizeof(salt), sodium_base64_VARIANT_ORIGINAL);

  // Set up some variables for the argon2 hash
  char hash[128];
  char encoded[256];

  long t_cost = 2;
  long m_cost = 1 << 16;
  long parallelism = 1;
  long hashlen = 32;

  // Don't fully understand this. Added from an example online.
  int result = argon2_hash(t_cost, m_cost, parallelism, password, strlen(password), theSalt, strlen(theSalt), hash, hashlen, encoded, sizeof(encoded), Argon2_id, ARGON2_VERSION_13);

  // Check for proper hash
  if (result != ARGON2_OK) {
    fprintf(stderr, "error: %s\n", argon2_error_message(result));
  }

  newUser->name = strdup(name);
  newUser->passHash = strdup(encoded);
  newUser->salt = strdup(theSalt);
  newUser->incomes = NULL;
  newUser->expenses = NULL;
  newUser->budgets = NULL;

  return newUser;
}


void createIncome(User *user, char *incomeName, long long cents) {
  Income *current = user->incomes;
  Income *newIncome = calloc(1, sizeof(*newIncome));
  if (!newIncome) {
    perror("Error: failed to add new income source");
    return;
  }
  int count = 0;
  newIncome->name = strdup(incomeName);
  newIncome->money = cents;
  newIncome->next = NULL;
  if (current == NULL) {
    newIncome->id = count;
    user->incomes = newIncome;
    return;
  }

  while (current->next != NULL) {
    count++;
    current = current->next;
  }

  newIncome->id = count;
  current->next = newIncome;
}

void createExpense(User *user, char *expenseName, long long cents) {
  Expense *current = user->expenses;
  Expense *newExpense = calloc(1, sizeof(*newExpense));

  if (!newExpense) {
    perror("Error: failed to create new expense");
    return;
  }

  int count = 0;
  newExpense->name = strdup(expenseName);
  newExpense->money = cents;
  newExpense->next = NULL;
  if (current == NULL) {
    newExpense->id = count;
    user->expenses = newExpense;
    return;
  }

  count = 1;
  while (current->next != NULL) {
    count++;
    current = current->next;
  }

  newExpense->id = count;
  current->next = newExpense;
}

void createBudget(User *user, char *budgetName, long long max) {
  Budget *current = user->budgets;
  Budget *newBudget = calloc(1, sizeof(*newBudget));

  if (!newBudget) {
    perror("Error: failed to create new budget");
    return;
  }

  int count = 0;
  newBudget->name = strdup(budgetName);
  newBudget->current = 0;
  newBudget->max = max;
  newBudget->next = NULL;
  if (current == NULL) {
    newBudget->id = count;
    user->budgets = newBudget;
    return;
  }

  count = 1;
  while (current->next != NULL) {
    count++;
    current = current->next;
  }

  newBudget->id = count;
  current->next = newBudget;
}


void addToBudget(User *user, int id, long long amount) {
  Budget *current = user->budgets;
  while (current->id != id) {
    if (current->next != NULL) {
      current = current->next;
    } else {
      perror("Error: that's not a valid ID...\n");
      return;
    }
  }
  current->current += amount;
}

void resetBudget(User *user, int id) {
  Budget *current = user->budgets;
  while(current->id != id) {
    if (current->next != NULL) {
      current = current->next;
    } else {
      perror("Error: Invalid ID...");
      return;
    }
  }
  current->current = 0;
}


void centsToString(char *str, long long cents) {
  bool neg = cents < 0;
  long long absCents = llabs(cents);
  long long dollars = absCents / 100;
  long long rem = absCents % 100;


  sprintf(str, "%s%lld.%.02lld", neg ? "-" : "", dollars, rem);
}


void userOverview(User *user) {
  Income *currentI = user->incomes;
  Expense *currentE = user->expenses;
  Budget *currentB = user->budgets;

  long long totalIncome = 0;
  long long totalExpense = 0;

  if (!currentI) {
    printf("%s currently has no incomes\n", user->name);
  } else {
    while(currentI->next != NULL) {
      totalIncome += currentI->money;
      currentI = currentI->next;
    }
    totalIncome += currentI->money;


    char total[15];
    centsToString(total, totalIncome);
    printf("You bring in an income of $%s!\n", total);
  }
  if (!currentE) {
    printf("%s currently has no expenses\n", user->name);
  } else {
    while(currentE->next != NULL) {
      totalExpense += currentE->money;
      currentE = currentE->next;
    }
    totalExpense += currentE->money;
    char total[15];
    centsToString(total, totalExpense);
    printf("Your expenses total: $%s\n", total);
  }
  if (!currentB) {
    printf("%s currently has no budgets\n", user->name);
  } else {
    while(currentB != NULL) {
      char bottom[15];
      char top[15];
      centsToString(bottom, currentB->current);
      centsToString(top, currentB->max);
      printf("%d. %s: %s/%s\n", currentB->id, currentB->name,bottom, top);
      currentB = currentB->next;
    }
  }

  if (currentI && currentE) {
    char total[15];
    centsToString(total, totalIncome - totalExpense);


    printf("Your Net Cash Flow: $%s\n", total);
    if (totalIncome <= totalExpense) {
      printf("We need to bring in more money!\n");
    }
  }
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

