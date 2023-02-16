#include <foo.h>
#include <stdio.h>

int main(void)
{
    register const int x = 11;
    register const int y = 5;

    printf("%d + %d = %d\n", x, y, foo_add(x, y));
    printf("%d - %d = %d\n", x, y, foo_sub(x, y));
    printf("%d * %d = %d\n", x, y, foo_mul(x, y));
    printf("%d / %d = %d\n", x, y, foo_div(x, y));

    return 0;
}
