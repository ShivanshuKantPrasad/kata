#include <stdio.h>
#include <criterion/criterion.h>

typedef struct {
  short x;
  short y;
} pos;

void knightFind(const pos sp, const pos ep, short moves[8][8]);

short knight(const char *start, const char *finish)
{
  short moves[8][8];
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      moves[i][j] = 10;
    }
  }
  pos sp = { .x = start[0] - 'a', .y = start[1] - '1'};
  pos ep = { .x = finish[0] - 'a', .y = finish[1] - '1' };
  moves[sp.x][sp.y] = 0;
  knightFind(sp, ep, moves);
  printf("%d\n", moves[ep.x][ep.y]);
  return moves[ep.x][ep.y];
}

void knightFind(const pos sp, const pos ep, short moves[8][8])
{
  pos n[8] = {
    { .x = sp.x + 1, .y = sp.y + 2 },
    { .x = sp.x + 2, .y = sp.y + 1 },
    { .x = sp.x + 2, .y = sp.y - 1 },
    { .x = sp.x + 1, .y = sp.y - 2 },
    { .x = sp.x - 1, .y = sp.y - 2 },
    { .x = sp.x - 2, .y = sp.y - 1 },
    { .x = sp.x - 1, .y = sp.y + 1 },
    { .x = sp.x - 2, .y = sp.y + 2 },
  };
  for(int i = 0; i < 8; i++){
    pos next = n[i];
    if(next.x < 0 || next.x > 7) continue;
    if(next.y < 0 || next.y > 7) continue;
    if(moves[next.x][next.y] <= moves[sp.x][sp.y]) continue;
    moves[next.x][next.y] = moves[sp.x][sp.y] + 1;
    knightFind(next, ep, moves);
  }
}

Test(Sample_Tests, should_pass_all_the_tests_provided)
{
  cr_assert_eq(knight("a1", "c1"), 2);
  cr_assert_eq(knight("a1", "f1"), 3);
  cr_assert_eq(knight("a1", "f3"), 3);
  cr_assert_eq(knight("a1", "f4"), 4);
  cr_assert_eq(knight("a1", "f7"), 5);
}
