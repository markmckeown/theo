#ifndef MAIN_H_
#define MAIN_H_

#include <unistd.h>

#define CGREEN_TEST_MAIN(MODULE) \
int main(int argc, char *argv[]) \
{ \
    int res = 0; \
    TestSuite *suite = MODULE(); \
    if ((argc - optind) > 0) { \
         res = run_single_test(suite, argv[optind], create_text_reporter()); \
    } else { \
        res = run_test_suite(suite, create_text_reporter()); \
    } \
    return res; \
}

#endif // end of MAIN_H_
