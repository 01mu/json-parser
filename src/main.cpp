/*
* json-parser
* github.com/01mu
*/

#include <iostream>
#include "lib/json-parser.cpp"

using namespace std;

int main()
{
    json other = json();

    string strings[10] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    /* initialize root object */
    other.init_object();

    /* create child object */
    other.add_object(strings[0]);

    /* add two strings to child object */
    other.add_string(strings[1], strings[2]);
    other.add_string(strings[3], strings[4]);

    /* end child object */
    other.end_object();

    /* create child array */
    other.add_array(strings[5]);

    /* add values to child array */
    for(int i = 0; i < 10; i++)
    {
        other.arr_add_number(i);
    }

    /* create array in array */
    other.arr_add_array();

    /* add values */
    for(int i = 0; i < 10; i++)
    {
        other.arr_add_number(i);

    }

    /* end array in array */
    other.end_array();

    /* end child array */
    other.end_array();

    /* add bool to root object */
    other.add_boolean(strings[6], false);

    /* end root initialized object */
    other.end_object();

    /* display */
    other.init_output();

    cout << endl << endl << "JSON:" << endl;

    /* setup key values */
    other.init_end();

    /* display */
    other.show_result();

    return 0;
}
