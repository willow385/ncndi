# Program to test the MPL compiler

funct one() int {
  return 1;
}

start {
  int i = 68;
  i = i + one();
  print i + "\n";
  print "Nice\n";
} end
