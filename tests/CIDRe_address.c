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

static void test_CIDRe_address(void)
{
    char *source = NULL;
    char *estimate = NULL;

    printf("\n%s\n", __func__);
    START_USING_TEST_DATA("data/")
    {
        USE_OF_THE_TEST_DATA("%ms = %ms", &source, &estimate)

        CIDRe cidr = CIDRe_create(source);
        char *actual = CIDRe_address(cidr);
        printf("%s=%s::%s\n", source, estimate, actual);
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

CU_TestInfo testing_CIDRe_address[] =
    {
        {"(CIDRe_address)", test_CIDRe_address},
        CU_TEST_INFO_NULL,
};
