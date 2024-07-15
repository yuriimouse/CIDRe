/**
 * @author Yurii Prudius
 **/
#include "defines.h"

#include <CUnit/Basic.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <cidre/cidre.h>

static void test_CIDRe_mask(void)
{
    char *source = NULL;
    int mask = 0;
    char *estimate = NULL;

    printf("\n%s\n", __func__);
    START_USING_TEST_DATA("data/")
    {
        USE_OF_THE_TEST_DATA("%ms : %d = %ms", &source, &mask, &estimate)

        CIDRe cidr = CIDRe_create(source);
        char *actual = CIDRe_string(CIDRe_mask(cidr, mask));
        printf("%s:%d=%s::%s\n", source, mask, estimate, actual);
        if (actual)
        {
            CU_ASSERT_STRING_EQUAL(estimate, actual);
        }
        else
        {
            CU_ASSERT_STRING_EQUAL(estimate, "NULL");
        }

        FREE_AND_NULL(actual);
        FREE_AND_NULL(estimate);
        FREE_AND_NULL(source);
    }

    FINISH_USING_TEST_DATA
}

CU_TestInfo testing_CIDRe_mask[] =
    {
        {"(CIDRe_mask)", test_CIDRe_mask},
        CU_TEST_INFO_NULL,
};
