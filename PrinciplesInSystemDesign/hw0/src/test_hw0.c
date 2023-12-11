#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

int sumArg(char * arg_str)
{
	static int sum;
	sum += atoi(arg_str);
	return sum;
}

void test_sumArg(void)
{
    CU_ASSERT(sumArg("5") == 5);
    CU_ASSERT(sumArg("5") == 10);
    CU_ASSERT(sumArg("5") == 15);
}

int init_suite1(void)
{
    return 0;
}

int clean_suite1(void)
{
    return 0;
}

int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "test_SumArg()", test_sumArg)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

