#include <stdio.h>
#include <string.h>

struct _huge_number {
  char *num;
  long long len;
} typedef huge_number;

huge_number *add_number               (huge_number *a, huge_number *b);
huge_number *init_number              (char *num);
void         free_number              (huge_number *a);
void         print_number             (huge_number *a);

int main() {

  char *num1, *num2;

  num1 = (char *) malloc(100);
  num2 = (char *) malloc(100);

  num1 = "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
  num2 = "111111111111121111111111111111111111121111111111111111111111111111111111111111111111111112";

  huge_number *a = init(num1);
  huge_number *b = init(num2);

  huge_number *c = add(a, b);

  free_number(a);
  free_number(b);
  free_number(c);
  return 0;
}

huge_number *init_number(char* num) {
  huge_number *a = (huge_number *)malloc(sizeof(huge_number));
  a.num = num;
  a.len = strlen(num);
  return a;
}

void print_number(huge_number* a) {
  for (int i = a->len; i >= 0; i--) {
    printf("%c", a->num[i]);
  }
  printf("\n");
}

huge_number *add_number(huge_number *a, huge_number *b) {
  huge_number *sum = malloc(sizeof(huge_number));


}
