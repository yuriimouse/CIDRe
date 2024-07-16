#ifndef UNIT_TEST_DEFS_H
#define UNIT_TEST_DEFS_H

#define _GNU_SOURCE

#define FREE_AND_NULL(x) \
    if (x)               \
        free(x);         \
    x = NULL

#define START_USING_TEST_DATA(path)                                                   \
    {                                                                                 \
        FILE *_fp;                                                                    \
        char *_pathstr = NULL;                                                        \
        asprintf(&_pathstr, "%s%s", (path), __func__);                                \
        if ((_fp = fopen(_pathstr, "r")))                                             \
        {                                                                             \
            size_t _len = 0;                                                          \
            char *_str = NULL;                                                        \
            for (int _read = 0; (_read = getline(&_str, &_len, _fp)) != -1; _len = 0) \
            {                                                                         \
                if (strlen(_str) && _str[0] != '#')

#define USE_OF_THE_TEST_DATA(...) \
    sscanf(_str, __VA_ARGS__)

#define FINISH_USING_TEST_DATA                     \
                FREE_AND_NULL(_str);               \
            }                                      \
            FREE_AND_NULL(_str);                   \
            fclose(_fp);                           \
            FREE_AND_NULL(_pathstr);               \
        }                                          \
        else                                       \
        {                                          \
            printf("'%s' not found...", _pathstr); \
            FREE_AND_NULL(_pathstr);               \
            CU_ASSERT_TRUE_FATAL(0);               \
        }                                          \
    }

#endif // UNIT_TEST_DEFS_H
