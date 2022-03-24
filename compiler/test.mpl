# Program to test the MPL compiler

funct foo(int x) int {
  return x + 1;
}

start {
  int i = 68;
  print foo(i) + "\n";
  print "Nice\n";
} end
