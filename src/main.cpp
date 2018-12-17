/*
* json-parser
* github.com/01mu
*/

#include <iostream>

#include "json-parser.cpp"

using namespace std;

int main()
{
    const char * src = "https://www.reddit.com/r/self/.json";

    json thing = json(src, 0);

    thing.show_result();

    return 0;
}
