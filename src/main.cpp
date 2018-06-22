#include "stdafx.h"

#include <iostream>
#include "parser.h"

using namespace std;

int main()
{
    json thing = json("json.txt");

    cout << thing.get("[glossary][title]");

    return 0;
}
