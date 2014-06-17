#ifndef ASSERT_H_INCLUDED_NFFUIUQL
#define ASSERT_H_INCLUDED_NFFUIUQL


void _assert(const char *cond, const char *msg, const char *file, int line);


#define ASSERT_MSG(cond, msg)                                                 \
    do                                                                        \
    {                                                                         \
        if (!(cond))                                                          \
        {                                                                     \
            _assert(#cond, msg, __FILE__, __LINE__);                          \
        }                                                                     \
    } while (0)


#define ASSERT(cond)                                                          \
    do                                                                        \
    {                                                                         \
        if (!(cond))                                                          \
        {                                                                     \
            _assert(#cond, 0, __FILE__, __LINE__);                            \
        }                                                                     \
    } while (0)


#endif // include guard

