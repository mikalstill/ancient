// Find all the zero divisors in a specified ring

// Stolen from Schneier, p 245
int gcd(int x, int y){
  int g;

  if(x < 0) x = -x;
  if(y < 0) y = -y;
  if(x + y == 0) return -1;
  g = y;
  while(x > 0){
    g = x;
    x = y % x;
    y = g;
  }
  return g;
}

int rebound(int u, int modulus){
  while(u < 0) u += modulus;
  while(u >= modulus) u -= modulus;
  return u;
}

int main(int argc, char *argv[]){
  int ring, count1, count2;

  /////////////////////////////////////////
  // Get the ring
  ring = atoi(argv[1]);
  printf("Assuming ring Z[%d]\n", ring);

  // Each pair of zero divisors will be listed twice in this output
  printf("Method one, brute force zero divisors:\n");
  for(count1 = 1; count1 < ring; count1++){
    for(count2 = 1; count2 < ring; count2++){
      if(rebound(count1 * count2, ring) == 0)
	printf("  %d and %d (%d)\n", count1, count2, count1 * count2);
    }
  }
  printf("\n");

  printf("Method two, GCD calculation:\n");
  for(count1 = 1; count1 < ring; count1++){
    if(gcd(count1, ring) != 1)
      printf("  %d\n", count1);
  }
}
