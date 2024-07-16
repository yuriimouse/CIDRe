#ifndef UNIT_TEST_DEFS_H
#define UNIT_TEST_DEFS_H

#define _GNU_SOURCE

/**
 * Frees the allocated block, if it exists, 
 * and sets the pointer to NULL
 * 
 */
#define FREE_AND_NULL(x) \
    if (x)               \
        free(x);         \
    x = NULL

/**
 * Starts reading test data from the file 
 * with the name of the current function 
 * at the specified path  
 * 
 * The local variable `DATA_DIR` contains the specified path
 * 
 */
#define START_USING_TEST_DATA(path)                                                   \
    {                                                                                 \
        char *DATA_DIR = strdup(path);                                                \
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

/**
 * Scans variables from the current line, 
 * like the `sscanf` function
 * 
 */
#define USE_OF_THE_TEST_DATA(...) \
    sscanf(_str, __VA_ARGS__)

/**
 * Finishes reading test data 
 * and frees all variables 
 * 
 */
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
        FREE_AND_NULL(DATA_DIR);                   \
    }

#endif // UNIT_TEST_DEFS_H
