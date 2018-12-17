/*
* json-parser
* github.com/01mu
*/

#include <iostream>

#include "json-parser.cpp"

using namespace std;

int main()
{
    //const char * src = "https://www.reddit.com/r/self/.json";
    const char * src = "input";

    json thing = json(src, 0);

    cout << "JSON: " << endl;
    thing.show_result();

    cout << endl << "Stats: " << endl;
    thing.show_stats();

    return 0;
}
