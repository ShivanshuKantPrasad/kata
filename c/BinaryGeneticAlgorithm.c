#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <criterion/criterion.h>

typedef double fitness_t (const char *);
const char *goal = "101010101010";

double fitness(const char *chromosome){
  double fit = 0;
  for(size_t i = 0; i < strlen(goal); i++){
    fit += goal[i] == chromosome[i] ? 1 : 0;
  }
  fit /= strlen(goal);
  return fit;
}

char* generate (size_t length) {
  char *c = calloc(length, sizeof(char));
  for(size_t i = 0; i < length; i++){
    c[i] = '0' + rand() % 2;
  }
  return c;
}

char* selectChromosome (int size, char* population[size], double fitnesses[size]) {
  double total = 0;
  for(int i = 0; i < size; i++){
    total += fitnesses[i];
  }
  double prob = total * (float) rand() / (float) RAND_MAX;
  int i = -1;
  while(prob > 0){
    i++;
    prob -= fitnesses[i];
  }
  return strdup(population[i]);
}

void mutate (size_t n, char* s, double p) {
  for(size_t i = 0; i < n; i++){
    double mut = (float) rand() / (float) RAND_MAX;
    s[i] = mut > p ? s[i] : s[i] == '0' ? '1' : '0';
  }
}

void crossover (size_t n, char* s1, char* s2) {
  for(size_t i = rand() % n; i < n; i++){
    s1[i] = s2[i];
  }
}

char* runN (fitness_t f, int length, double p_c, double p_m, int iterations);

char* runN (fitness_t f, int length, double p_c, double p_m, int iterations) {

  //Generate initial population
  int popSize = 100;
  char *pop[popSize];
  double fitnesses[popSize];
  for(int i = 0; i < popSize; i++){
    pop[i] = generate(length);
    fitnesses[i] = f(pop[i]);
  }

  while(iterations--) {
    char *newPop[popSize];

    //Create new population
    for(int i = 0; i < popSize - 1; i += 2){
      char *a = selectChromosome(popSize, pop, fitnesses);
      char *b = selectChromosome(popSize, pop, fitnesses);
      float prob = (float) rand() / (float) RAND_MAX;
      if(prob < p_c) crossover(length, a, b);
      mutate(length, a, p_m);
      mutate(length, b, p_m);
      newPop[i] = a;
      newPop[i + 1] = b;
    }

    //Swap Populations
    for(int i = 0; i < popSize; i++){
      free(pop[i]);
      pop[i] = newPop[i];
      fitnesses[i] = f(newPop[i]);
    }

    if(fitnesses[0] == 1) break;

  }

  //Get rid of all population except first since first is the result
  for(int i = 1; i < popSize; i++) {
    free(pop[i]);
  }

  return pop[0];

}

char* run (fitness_t f, int length, double p_c, double p_m) {
  return runN(f, length, p_c, p_m, 100);
}

void test(const char *actual, const char *expected, char *message){
  if(strcmp(actual, expected)){
    printf("Actual: %s\nExpected: %s\n%s\n", actual, expected, message);
  } else {
    printf("Test Passed!");
  }
}


Test(binary_genetic_algorithm, testRun) {
  cr_assert_str_eq(runN(fitness, strlen(goal), 0.6, 0.002, 100), goal, "Error");
}
