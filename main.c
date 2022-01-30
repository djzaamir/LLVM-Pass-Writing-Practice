
int add_two_numbers(int, int);

int main(int argc, char **argv){
  int a = 10;
  int b = 10;

  //get the result and then print it
  int c = add_two_numbers(a, b);

  return c;
}


int add_two_numbers(int a, int b){
   int c =  a + b;
   return c;
}
