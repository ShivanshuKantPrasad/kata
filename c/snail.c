#include <criterion/criterion.h>
#include <stddef.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int *snail(size_t *outsz, const int **mx, size_t m, size_t n)
{
  *outsz = m * n;
  int *res = calloc(*outsz, sizeof(int));
  int dir = 1;
  size_t i = 0;
  size_t posX = 0, posY = 0;
  while(n > 0) {
    for(size_t j = 0; j < n; j++){
      res[i] = mx[posY][posX];
      i++;
      posX += dir;
    }
    posX -= dir;
    posY += dir;
    for(size_t j = 1; j < n; j++){
      res[i] = mx[posY][posX];
      i++;
      posY += dir;
    }
    posY -= dir;
    dir *= -1;
    posX += dir;
    n--;
  }
  return res;
}

#define DIMLEN      0x4
#define FMT_INT     (CHAR_BIT * sizeof(int) / 3 + 1)
#define FMT_ALIGN   0x2
#define FMT_MX      ((FMT_ALIGN + (FMT_INT + FMT_ALIGN) * DIMLEN) * DIMLEN + 1)

int **ia_to_imx(int **mx, const int *a, size_t rows, size_t cols)
{
  while (rows--)
    *(mx+rows) = (int *)a + rows * cols;
  return mx;
}

char *ia_to_s(char *s, const int *a, size_t n, const char *sep)
{
  const char *p = sep ? sep : ", ";
  size_t pos;
  if (!a) {
    pos = sprintf(s, "NULL");
    return s;
  }
  pos = sprintf(s, "[ %i", *a++);
  while (--n)
    pos += sprintf(s+pos, "%s%i", p, *a++);
  sprintf(s+pos, " ]");
  return s;
}

char *imx_to_s(char *s, const int **mx, size_t nrows, size_t ncols)
{
  size_t pos, i, j;
  for (pos = i = 0; i < nrows; ++i) {
    pos += sprintf(s+pos, "[ ");
    for (j = 0; j < ncols; ++j)
      pos += sprintf(s+pos, "%i, ", mx[i][j]);
    if (j)
      pos -= 2;
    pos += sprintf(s+pos, " ]\n");
  }
  if (i)
    --pos;
  *(s+pos) = '\0';
  return s;
}

typedef enum {
  ASSERT_PASS,
  ASSERT_FAIL
} assertop;

assertop assert_mem_eq(const void *actual, const void *expected, size_t n, size_t size)
{
  size_t i;
  if (!actual || !expected)
    return actual != expected ? ASSERT_FAIL : ASSERT_PASS;
  for (i = 0ul; n--; i += size)
    if (memcmp((char *)actual+i, (char *)expected+i, size))
      return ASSERT_FAIL;
  return ASSERT_PASS;
}

void assert_data(const int *mx, size_t m, size_t n, const int *expected_snail, size_t expected_outsz)
{
  char outpf[3][FMT_MX];
  int *pmx[DIMLEN];
  size_t actual_outsz;
  int *actual_snail = snail(&actual_outsz, ia_to_imx(pmx, mx, m, n), m, n);
  if (!actual_snail || actual_outsz != expected_outsz || assert_mem_eq(actual_snail, expected_snail, expected_outsz, sizeof(int)) == ASSERT_FAIL)
    cr_assert_fail("[*Actual*]\n  Outsz: %lu\n  Snail: %s\n[Expected]\n  Outsz: %lu\n  Snail: %s\nMatrix:\n%s\n",
                   actual_outsz,  ia_to_s(outpf[0], actual_snail, expected_outsz, 0),
                   expected_outsz, ia_to_s(outpf[1], expected_snail, expected_outsz, 0),
                   imx_to_s(outpf[2], (const int **)pmx, m, n));
  else
    cr_assert(1);
  if (actual_snail)
    free(actual_snail);
}

Test(Sample_Test, should_return_the_snail)
{
  assert_data((const int[]){ 1,2,3, 4,5,6, 7,8,9 }, 3ul, 3ul, (const int[]){ 1,2,3,6,9,8,7,4,5 }, 9ul);
  assert_data((const int[]){ 1,2,3, 8,9,4, 7,6,5 }, 3ul, 3ul, (const int[]){ 1,2,3,4,5,6,7,8,9 }, 9ul);
  assert_data((const int[]){ 1,2,3,1, 4,5,6,4, 7,8,9,7, 7,8,9,7 }, 4ul, 4ul, (const int[]){ 1,2,3,1,4,7,7,9,8,7,7,4,5,6,9,8 }, 16ul);
}
