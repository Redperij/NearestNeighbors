#include <iostream>
#include "test.h"

using namespace std;


int main()
{
    cout << "Started." << endl;

    int failed_tests = run_tests();

    cout << "\n\n##############################################################" <<endl;
    if(!failed_tests)
        cout << "All tests succeeded." << endl;
    else
        cout << failed_tests << " tests failed." << endl;
    
    return 0;
}
