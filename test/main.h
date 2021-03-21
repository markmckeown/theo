/**
    Copyright 2021 WANdisco

    This file is part of Libtheo.

    Libtheo is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef MAIN_H_
#define MAIN_H_

#include <unistd.h>

#define CGREEN_TEST_MAIN(MODULE) \
extern int g_start_mocking; \
int main(int argc, char *argv[]) \
{ \
    int res = 0; \
    TestSuite *suite = MODULE(); \
    g_start_mocking = 1; \
    if ((argc - optind) > 0) { \
         res = run_single_test(suite, argv[optind], create_text_reporter()); \
    } else { \
        res = run_test_suite(suite, create_text_reporter()); \
    } \
    g_start_mocking = 0; \
    return res; \
}

#endif // end of MAIN_H_
