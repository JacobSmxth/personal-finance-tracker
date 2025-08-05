#include <stdio.h>



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

int main(void) {


  printf("Finance App\n");
  return 0;
}
