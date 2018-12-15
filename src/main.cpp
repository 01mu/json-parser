/*
* json-parser
* github.com/01mu
*/

#include "json-parser.h"

int main(int argc, char ** argv)
{
    json thing = json(argv[1], atoi(argv[2]));

    thing.show_result();

    return 0;
}
