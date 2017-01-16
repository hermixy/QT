#ifndef LIB_BASE_H
#define LIB_BASE_H

//#ifndef UNUSE
//#define UNUSE(x) (void)(x)
//#endif

#define OK      (0)
#define ERR     (-1)

#define ABS(x) ((x) >= 0 ? (x) : (-(x)))
#define MIN(x,y)    ((x) < (y) ? (x) : (y))
#define MAX(x,y)    ((x) > (y) ? (x) : (y))

#define IntToBool(i)    (0 != (i))
#define BoolToInt(b)    ((b) ? 1 : 0)

#endif // LIB_BASE_H
