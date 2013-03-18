#include "convex.h"

int main(int argc, char *argv[]) {
  if (argc != 2) 
    puts("Usage: ./convex N"), exit(1);
  print_cartesian();
  load_points(atoi(argv[1]));
  print_points();
  create_path();
  set_draw_function(print_ordering);
  gflush();
  return 0;
}
