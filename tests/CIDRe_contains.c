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

static void test_CIDRe_contains(void)
{
    char *big = NULL;
    char *small = NULL;
    int estimate = 0;

    printf("\n%s\n", __func__);
    START_USING_TEST_DATA("data/")
    {
        USE_OF_THE_TEST_DATA("%ms : %ms = %d", &big, &small, &estimate)

        int actual = CIDRe_contains(CIDRe_create(big), CIDRe_create(small));
        printf("%s:%s=%d::%d\n", big, small, estimate, actual);
        CU_ASSERT_EQUAL(estimate, actual);

        // FREE_AND_NULL(actual);
        // FREE_AND_NULL(estimate);
        FREE_AND_NULL(small);
        FREE_AND_NULL(big);
    }

    FINISH_USING_TEST_DATA
}

CU_TestInfo testing_CIDRe_contains[] =
    {
        {"(CIDRe_contains)", test_CIDRe_contains},
        CU_TEST_INFO_NULL,
};
