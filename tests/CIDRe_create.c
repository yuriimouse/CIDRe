/**
 * @author Yurii Prudius
 **/
#include "defines.h"

#include <CUnit/Basic.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cidre/cidre.h>

static void test_CIDRe_create(void)
{
    char *source = NULL;
    char *estimate = NULL;

    fprintf(stderr, "\n%s\n", __func__);
    START_USING_TEST_DATA("data/")
    {
        USE_OF_THE_TEST_DATA("%ms = %ms", &source, &estimate);

        CIDRe cidr = CIDRe_create(source);
        char *buff = NULL;
        int res = asprintf(&buff, "%016llx", (long long unsigned int)cidr);
        fprintf(stderr, "%s(%s)::%s=%s\n", __func__, source, estimate, buff);
        CU_ASSERT_TRUE_FATAL(res >= 0);
        CU_ASSERT_STRING_EQUAL(estimate, buff);

        FREE_AND_NULL(buff);
        FREE_AND_NULL(estimate);
        FREE_AND_NULL(source);
    }

    FINISH_USING_TEST_DATA
}

CU_TestInfo testing_CIDRe_create[] =
    {
        {"(CIDRe_create)", test_CIDRe_create},
        CU_TEST_INFO_NULL,
};
