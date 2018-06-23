/*
* JSON Parser
* Version 0.1
*
* Parses valid JSON.
*
* github.com/01mu
*/

#pragma once

#include "stdafx.h"

#include <ctype.h>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#define NUMBER 0
#define OBJECT 1
#define ARRAY 2
#define STRING 3
#define NULL 4
#define BOOL 5

using namespace std;

/*
* forward declarations
*/

class json;
struct json_object;
struct json_array;

/*
* json_vector data type
*/

struct json_array_item
{
    int type;

    union
    {
        json_object * object;
        json_array * array;
        int * null;
        bool * boolean;
        double * double_val;
        string * string_val;
    } value;
};

/*
* typedef
*/

typedef vector<json_array_item> json_vector;

typedef pair<string, json_array *> pair_array;
typedef pair<string, string> pair_string;
typedef pair<string, int> pair_null;
typedef pair<string, double> pair_number;
typedef pair<string, bool> pair_bool;
typedef pair<string, json_object *> pair_object;

typedef map<string, json_array *> map_array;
typedef map<string, string> map_string;
typedef map<string, int> map_null;
typedef map<string, double> map_number;
typedef map<string, bool> map_bool;
typedef map<string, json_object *> map_object;

/*
* json array
*/

struct json_array
{
    string key;
    json_array * parent;
    json_vector array;
};

/*
* json object
*/

struct json_object
{
    string key;
    json_object * parent;
    vector<string> array_keys, string_keys, null_keys, number_keys, boolean_keys, object_keys;
    map_array arrays;
    map_string strings;
    map_null nulls;
    map_number numbers;
    map_bool booleans;
    map_object objects;
};

/*
* file
*/

ofstream fout("results.txt");

/*
* class
*/

class json
{
private:
    /*
    * parsing variables
    */

    int index = 0;                                      // index position for parsing
    string file;                                        // file name
    string json_text;                                   // json text to parse

    int array_depth_max = 0;                            // max array depth
    int array_depth = 0;                                // current array depth

    int object_depth_max = 0;                           // max object depth
    int object_depth = 0;                               // current object depth

    int len;                                            // file length

    int WIDTH = 35;                                     // width between key and value
    int NUM_WIDTH = 30;                                 // width between depths
    int JWIDTH = 15;                                    // final width
    int key_width = 0;                                  // width between location and value

    int string_count = 0;                               // string count
    int bool_count = 0;                                 // bool count
    int array_count = 0;                                // array count
    int object_count = 0;                               // object count
    int decimal_count = 0;                              // decimal count
    int key_count = 0;                                  // key count
    int parse_count = 0;                                // parse function call count
    int null_count = 0;                                 // null count

    /*
    * data variables
    */

    json_object * base_object = new json_object;        // first object
    json_array * base_vector = new json_array;          // first vector

    json_object * latest_object;                        // most recent object where parsed values will be assigned
    json_array * latest_vector;                         // most recent vector

    stack<string> object_keys_stack;                    // path stack helper

    string last_obj_key;                                // used for path
    string last_key_name;                               // reference to last key for value storage

    map<string, string> final_map;                      // map in which locations and values are stored
    vector<string> locations;                           // key names

    bool first_object = 0;                              // used to track initialization of base object
    bool first_vector = 0;                              // base vector

    int first_type = 0;                                 // whether the file is contained as an array or object

public:
    /*
    * constructor 
    */

    json(string file);                                  // parse file, assign to final map, and print results.txt

    /*
    * parsing functions
    */

    void parse();                                       // main function to parse json objects
    void check_key();                                   // parse value derived from value key
    void show_title();                                  // write to results.txt
    void show_stats();                                  // show counts and write to results.txt

    string get_file(string file_name);                  // get file name
    string get_string();                                // parse string (for key or string value)
    string get_boolean(char type);                      // parse bool
    string get_decimal();                               // parse decimal

    void remove_blank_leading();                        // remove leading blanks from string input
    void remove_blank_trail();                          // remove trailing blanks
    bool remove_blank();                                // remove blanks encountered during parsing

    void update_arr_depth(char type);                   // update array depth
    void update_obj_depth(char type);                   // update object depth
    string make_ellipses(string str, char type);        // compress long strings to fit formatting
    string get(string location);                        // get from final map

    void update_display_width(string key);              // update fout width

    /*
    * for objects
    */

    void add_object(string key);                        // add object to current object
    void add_string(string key, string value);          // add string to current object
    void add_number(string key, double value);          // add number to current object
    void add_boolean(string key, bool value);           // add boolean to current object
    void add_array(string key);                         // add array structure to current object
    void add_null(string key);                          // add null to current object

    void end_object();                                  // point back to object's parent when parsing of it ends

    /*
    * for arrays
    */

    void arr_add_object();                              // add object to current array
    void arr_add_string(string value);                  // add string to current array
    void arr_add_number(double value);                  // add number to current array
    void arr_add_boolean(bool value);                   // add bool to current array
    void arr_add_array();                               // add array to current array
    void arr_add_null();                                // add null to current array

    void end_array();                                   // point back to array's parent when parsing of it ends

    /*
    * check
    */

    void traverse_object(int mode);                     // traverse object and write path to value, 0 = of array, 1 = root object
    void traverse_array(int mode);                      // traverse array
    void reset();                                       // redirect latest pointers to base structures for results.txt output
    void pop_object_stack();                            // pop path stack
};

json::json(string file)
{
    fout << setiosflags(ios::left);

    json_text = get_file(file);

    this->file = file;
    len = json_text.size();

    show_title();

    remove_blank_leading();
    remove_blank_trail();

    char next = json_text.at(index);

    if (next == '[' || next == '{')
    {
        if (next == '[')
        {
            first_type = ARRAY;
        }
        else
        {
            first_type = OBJECT;
        }

        while (index != len)
        {
            parse();
        }

        reset();

        if (first_type == OBJECT)
        {
            traverse_object(0);
        }
        else
        {
            object_keys_stack.push("");

            traverse_array(0);
        }

        key_width += 5;

        fout << endl << "------------------------------------------------------------------------------------------------------------------------" << endl << endl;
        fout << setw(key_width) << "- Location -" << "- Value -" << endl << endl;

        for (int i = 0; i < final_map.size(); i++)
        {
            string location = locations.at(i);
            fout << setw(key_width) << location << final_map.at(location) << endl;
        }

        show_stats();
    }
    else
    {
        fout << json_text;
    }
}

void json::parse()
{
    parse_count++;

    char next = json_text.at(index);

    if (next == ':')
    {
        check_key();
    }
    else if (isdigit(next) || next == '-' || next == '+' || next == '.')
    {
        string value = get_decimal();

        decimal_count++;

        fout << setw(WIDTH) << " " << setw(WIDTH) << value << setw(NUM_WIDTH) << " " << setw(NUM_WIDTH) << " " << endl;

        arr_add_number(stod(value));
    }
    else if (next == 'n')
    {
        null_count++;
        index = index + 4;

        fout << setw(WIDTH) << " " << setw(WIDTH) << "null" << endl;

        arr_add_null();
    }
    else if (next == 't')
    {
        string str = get_boolean('t');

        bool_count++;

        fout << setw(WIDTH) << " " << setw(WIDTH) << str << setw(NUM_WIDTH) << " " << setw(NUM_WIDTH) << " " << endl;

        arr_add_boolean(true);
    }
    else if (next == 'f')
    {
        string str = get_boolean('f');

        bool_count++;

        fout << setw(WIDTH) << " " << setw(WIDTH) << str << setw(NUM_WIDTH) << " " << setw(NUM_WIDTH) << " " << endl;

        arr_add_boolean(false);
    }
    else if (next == '"')
    {
        string str = get_string();

        if (json_text.at(index) == ':')
        {
            key_count++;

            fout << setw(WIDTH) << make_ellipses(str, 'k');

            last_key_name = str;
        }
        else
        {
            string_count++;

            fout << setw(WIDTH) << " " << make_ellipses(str, 'v') << endl;

            arr_add_string(str);
        }
    }
    else if (next == '{')
    {
        index++;
        object_count++;

        update_obj_depth('i');

        fout << setw(WIDTH) << " " << setw(WIDTH) << "{object start}" << setw(NUM_WIDTH) << array_depth << object_depth << endl;

        arr_add_object();
    }
    else if (next == '}')
    {
        index++;

        update_obj_depth('d');

        fout << setw(WIDTH) << " " << setw(WIDTH) << "{object end}" << setw(NUM_WIDTH) << array_depth << object_depth << endl;

        end_object();
    }
    else if (next == '[')
    {
        index++;
        array_count++;

        update_arr_depth('i');

        fout << setw(WIDTH) << " " << setw(WIDTH) << "[array start]" << setw(NUM_WIDTH) << array_depth << object_depth << endl;

        arr_add_array();
    }
    else if (next == ']')
    {
        index++;

        update_arr_depth('d');

        fout << setw(WIDTH) << " " << setw(WIDTH) << "[array end]" << setw(NUM_WIDTH) << array_depth << object_depth << endl;

        end_array();
    }
    else if (next == '\n' || next == ' ' || next == '\t')
    {
        index++;
    }
    else if (next == ',')
    {
        index++;
    }

    remove_blank();
}

void json::check_key()
{
    index++;

    char next = json_text.at(index);

    if (next == '"')
    {
        string str = get_string();

        string_count++;

        fout << setw(WIDTH) << make_ellipses(str, 'v') << setw(NUM_WIDTH) << " " << setw(JWIDTH) << " " << endl;

        add_string(last_key_name, str);
    }
    else if (next == 't')
    {
        string str = get_boolean('t');

        bool_count++;

        fout << setw(WIDTH) << str << setw(NUM_WIDTH) << " " << setw(JWIDTH) << " " << endl;

        add_boolean(last_key_name, true);
    }
    else if (next == 'f')
    {
        string str = get_boolean('f');

        bool_count++;

        fout << setw(WIDTH) << str << setw(NUM_WIDTH) << " " << setw(JWIDTH) << " " << endl;

        add_boolean(last_key_name, false);
    }
    else if (next == 'n')
    {
        null_count++;
        index = index + 4;

        fout << setw(WIDTH) << "null" << endl;

        add_null(last_key_name);
    }
    else if (isdigit(next) || next == '-' || next == '+' || next == '.')
    {
        string value = get_decimal();

        decimal_count++;

        fout << setw(WIDTH) << value << setw(NUM_WIDTH) << " " << setw(JWIDTH) << " " << endl;

        add_number(last_key_name, stod(value));
    }
    else if (next == '{')
    {
        object_count++;
        index++;

        update_obj_depth('i');

        fout << setw(WIDTH) << "{object start}" << setw(NUM_WIDTH) << array_depth << object_depth << endl;

        add_object(last_key_name);
    }
    else if (next == '[')
    {
        array_count++;
        index++;

        update_arr_depth('i');

        fout << "" << setw(WIDTH) << "[array start]" << setw(NUM_WIDTH) << array_depth << object_depth << endl;

        add_array(last_key_name);
    }
    else if (next == '\n' || next == ' ' || next == '\t')
    {
        check_key();
    }

    remove_blank();
}

string json::get_file(string file_name)
{
    ifstream in_file;
    in_file.open(file_name);

    stringstream str_stream;
    str_stream << in_file.rdbuf();

    string str = str_stream.str();

    return str;
}

string json::get_string()
{
    int start = index + 1;
    int quote_count = 0;

    char prev_char = '\\';
    int qc = 0;

    while (prev_char == '\\') {
        char check = json_text.at(index);

        quote_count++;

        if (check == '"')
        {
            if (qc > 0)
            {
                prev_char = json_text.at(index - 1);
            }

            qc++;
        }

        index++;
    }

    return json_text.substr(start, quote_count - 2);
}

string json::get_boolean(char type)
{
    string return_string;

    if (type == 't')
    {
        return_string = json_text.substr(index, 4);
        index = index + 4;
    }
    else
    {
        return_string = json_text.substr(index, 5);
        index = index + 5;
    }

    return return_string;
}

string json::get_decimal()
{
    int offset = 0;
    int start = index;

    char num_char = json_text.at(start);

    bool is_neg;

    if (json_text.at(index) == '-' || json_text.at(index) == '+')
    {
        if (json_text.at(index) == '-')
        {
            is_neg = true;
        }
        
        index++;
    }

    start = index;
    num_char = json_text.at(start);

    if (json_text.at(index) == '0' && json_text.at(index + 1) != 0 && isdigit(json_text.at(index + 1)))
    {
        while (num_char == '0')
        {
            index++;
            start++;

            num_char = json_text.at(index);
        }
    }

    while (isdigit(num_char))
    {
        index++;

        num_char = json_text.at(index);

        offset++;
    }

    if (json_text.at(index) == '.')
    {
        num_char = json_text.at(index + 1);

        while (isdigit(num_char))
        {
            index++;

            num_char = json_text.at(index);

            offset++;
        }
    }

    if (json_text.at(index) == 'e')
    {
        index++;
        offset++;

        if (json_text.at(index) == '-' || json_text.at(index) == '+')
        {
            index++;
            offset++;
        }

        num_char = json_text.at(index);

        while (isdigit(num_char))
        {
            index++;

            num_char = json_text.at(index);

            offset++;
        }
    }

    string ret = json_text.substr(start, offset);

    if (is_neg == true)
    {
        ret = "-" + json_text.substr(start, offset);
    }

    return ret;
}

bool json::remove_blank()
{
    bool found = false;

    if (index + 1 < len)
    {
        char space_check = json_text.at(index);

        while (isspace(space_check) || space_check == '\n' || space_check == '\t')
        {
            index++;

            space_check = json_text.at(index);

            found = true;
        }
    }

    return found;
}

void json::remove_blank_leading()
{
    char space_check = json_text.at(index);

    while (isspace(space_check) || space_check == '\n' || space_check == '\t')
    {
        index++;

        space_check = json_text.at(index);
    }
}

void json::remove_blank_trail()
{
    char space_check = json_text.at(len - 1);

    while (space_check == ' ' || space_check == '\n' || space_check == '\t')
    {
        len--;

        space_check = json_text.at(len);
    }
}

void json::show_title()
{
    fout << "------------------------------------------------------------------------------------------------------------------------" << endl << endl;
    fout << "JSON Parser" << endl;
    fout << "Version 0.1" << endl << endl;
    fout << "Parses valid JSON." << endl << endl;
    fout << "------------------------------------------------------------------------------------------------------------------------" << endl << endl;
    fout << "File: " << file << endl;
    fout << "Length: " << len << endl << endl;
    fout << "------------------------------------------------------------------------------------------------------------------------" << endl << endl;
    fout << setw(WIDTH) << "- Key -" << setw(WIDTH) << "- Value -" << setw(NUM_WIDTH) << "- Array Depth -" << "- Object Depth -" << endl << endl;
}

void json::show_stats()
{
    fout << endl << "------------------------------------------------------------------------------------------------------------------------" << endl << endl;
    fout << setw(WIDTH) << "Key count: " << setw(WIDTH) << key_count;
    fout << setw(WIDTH) << "Decimal count: " << decimal_count << endl;
    fout << setw(WIDTH) << "Object count: " << setw(WIDTH) << object_count;
    fout << setw(WIDTH) << "Boolean count: " << bool_count << endl;
    fout << setw(WIDTH) << "Array count: " << setw(WIDTH) << array_count;
    fout << setw(WIDTH) << "Max array depth: " << array_depth_max << endl;
    fout << setw(WIDTH) << "String count: " << setw(WIDTH) << string_count;
    fout << setw(WIDTH) << "Max object depth: " << object_depth_max << endl;
    fout << setw(WIDTH) << "Null count: " << setw(WIDTH) << null_count;
    fout << setw(WIDTH) << "Parse count: " << parse_count << endl << endl;
    fout << "------------------------------------------------------------------------------------------------------------------------" << endl;
}

void json::update_obj_depth(char type)
{
    if (type == 'i')
    {
        object_depth++;

        if (object_depth > object_depth_max)
        {
            object_depth_max = object_depth;
        }
    }
    else
    {
        object_depth--;
    }
}

void json::update_arr_depth(char type)
{
    if (type == 'i')
    {
        array_depth++;

        if (array_depth > array_depth_max)
        {
            array_depth_max = array_depth;
        }
    }
    else
    {
        array_depth--;
    }
}

string json::make_ellipses(string str, char type)
{
    string res;

    if (type == 'v')
    {
        int max = WIDTH - 6;

        if ((int)str.length() > max)
        {
            res = "\"" + str.substr(0, max) + "...\"";
        }
        else
        {
            res = "\"" + str + "\"";
        }
    }
    else
    {
        int max = WIDTH - 5;

        if ((int)str.length() > max)
        {
            res = str.substr(0, max) + "...:";
        }
        else
        {
            res = str + ":";
        }
    }

    return res;
}

void json::end_object()
{
    if (latest_object->parent)
    {
        latest_object = latest_object->parent;
    }
}

void json::end_array()
{
    if (latest_vector->parent)
    {
        latest_vector = latest_vector->parent;
    }
}

void json::add_object(string key)
{
    json_object * newest = new json_object;
    newest->parent = latest_object;
    newest->key = key;

    latest_object->object_keys.push_back(key);
    latest_object->objects.insert(pair_object(key, newest));

    latest_object = newest;
}

void json::add_string(string key, string value)
{
    latest_object->string_keys.push_back(key);
    latest_object->strings.insert(pair_string(key, value));
}

void json::add_number(string key, double value)
{
    latest_object->number_keys.push_back(key);
    latest_object->numbers.insert(pair_number(key, value));
}

void json::add_boolean(string key, bool value)
{
    latest_object->boolean_keys.push_back(key);
    latest_object->booleans.insert(pair_bool(key, value));
}

void json::add_array(string key)
{
    json_array * newest = new json_array;
    newest->parent = latest_vector;
    newest->key = key;

    latest_object->array_keys.push_back(key);
    latest_object->arrays.insert(pair_array(key, newest));

    latest_vector = newest;
}

void json::add_null(string key)
{
    latest_object->null_keys.push_back(key);
    latest_object->nulls.insert(pair_bool(key, 0));
}

void json::arr_add_object()
{
    if (!first_object && first_type == OBJECT)
    {
        latest_object = base_object;
        first_object = 1;
    }
    else
    {
        json_object * newest = new json_object;
        newest->parent = latest_object;

        json_array_item temp;

        temp.type = OBJECT;
        temp.value.object = newest;

        latest_vector->array.push_back(temp);

        latest_object = newest;
    }
}

void json::arr_add_string(string value)
{
    string * put = new string;
    *put = value;

    json_array_item temp;

    temp.type = STRING;
    temp.value.string_val = put;

    latest_vector->array.push_back(temp);
}

void json::arr_add_number(double value)
{
    double * put = new double;
    *put = value;

    json_array_item temp;

    temp.type = NUMBER;
    temp.value.double_val = put;

    latest_vector->array.push_back(temp);
}

void json::arr_add_boolean(bool value)
{
    bool * put = new bool;
    *put = value;

    json_array_item temp;

    temp.type = BOOL;
    temp.value.boolean = put;

    latest_vector->array.push_back(temp);
}

void json::arr_add_array()
{
    if (!first_vector && first_type == ARRAY)
    {
        latest_vector = base_vector;
        first_vector = 1;
    }
    else
    {
        json_array * newest = new json_array;
        newest->parent = latest_vector;

        json_array_item temp;

        temp.type = ARRAY;
        temp.value.array = newest;

        latest_vector->array.push_back(temp);

        latest_vector = newest;
    }
}

void json::arr_add_null()
{
    int * put = new int;
    *put = 0;

    json_array_item temp;

    temp.type = NULL;
    temp.value.null = put;

    latest_vector->array.push_back(temp);
}

void json::traverse_object(int mode)
{
    string last_key = "";

    if (latest_object->key.length() > 0)
    {
        if (object_keys_stack.size() > 0)
        {
            last_key = object_keys_stack.top() + "[" + latest_object->key + "]";
        }
        else
        {
            last_key = "[" + latest_object->key + "]";
        }
    }

    vector<string> string_keys = latest_object->string_keys;
    vector<string> number_keys = latest_object->number_keys;
    vector<string> bool_keys = latest_object->boolean_keys;
    vector<string> null_keys = latest_object->null_keys;
    vector<string> object_keys = latest_object->object_keys;
    vector<string> array_keys = latest_object->array_keys;

    int string_count = latest_object->string_keys.size();
    int number_count = latest_object->number_keys.size();
    int bool_count = latest_object->boolean_keys.size();
    int null_count = latest_object->null_keys.size();
    int object_count = latest_object->object_keys.size();
    int array_count = latest_object->array_keys.size();

    map_string strings = latest_object->strings;
    map_number numbers = latest_object->numbers;
    map_bool bools = latest_object->booleans;
    map_null nulls = latest_object->nulls;
    map_object objects = latest_object->objects;
    map_array arrays = latest_object->arrays;

    string res = "";

    if (mode == 0)
    {
        object_keys_stack.push(last_key);
    }

    for (int i = 0; i < 6; i++)
    {
        int check;

        switch (i)
        {
        case(STRING):
        {
            check = string_count;
            break;
        }
        case(NUMBER):
        {
            check = number_count;
            break;
        }
        case(BOOL):
        {
            check = bool_count;
            break;
        }
        case(NULL):
        {
            check = null_count;
            break;
        }
        case(OBJECT):
        {
            check = object_count;
            break;
        }
        case(ARRAY):
        {
            check = array_count;
            break;
        }
        }

        for (int j = 0; j < check; j++)
        {
            string idx;
            string key = object_keys_stack.top();

            switch (i)
            {
            case(STRING):
            {
                idx = string_keys.at(j);

                string value = strings.at(idx);
                string key_string = key + "[" + idx + "]";

                final_map.insert(pair<string, string>(key_string, value));
                update_display_width(key_string);
                break;
            }
            case(NUMBER):
            {
                idx = number_keys.at(j);

                string value = to_string(numbers.at(idx));
                string key_string = key + "[" + idx + "]";

                final_map.insert(pair<string, string>(key_string, value));
                update_display_width(key_string);
                break;
            }
            case(BOOL):
            {
                idx = bool_keys.at(j);

                string value = to_string(bools.at(idx));
                string key_string = key + "[" + idx + "]";

                final_map.insert(pair<string, string>(key_string, value));
                update_display_width(key_string);
                break;
            }
            case(NULL):
            {
                idx = null_keys.at(j);

                string value = to_string(nulls.at(idx));
                string key_string = key + "[" + idx + "]";

                final_map.insert(pair<string, string>(key_string, value));
                update_display_width(key_string);
                break;
            }
            case(OBJECT):
            {
                idx = object_keys.at(j);
                latest_object = objects.at(idx);

                traverse_object(0);

                latest_object = latest_object->parent;
                break;
            }
            case(ARRAY):
            {
                idx = array_keys.at(j);
                latest_vector = arrays.at(idx);

                string mod = object_keys_stack.top();
                object_keys_stack.push(mod + "[" + idx + "]");

                if (mode == 0)
                {
                    traverse_array(0);
                    pop_object_stack();
                }
                else
                {
                    traverse_array(1);
                }
                
                break;
            }
            }
        }
    }

    if (mode == 0)
    {
        pop_object_stack();
    }
}

void json::traverse_array(int mode)
{
    string key = object_keys_stack.top();
    json_vector to_traverse = latest_vector->array;

    int array_size = latest_vector->array.size();

    for (int i = 0; i < array_size; i++)
    {
        string index = to_string(i);
        string key_string = key + "[" + index + "]";

        int type = to_traverse.at(i).type;

        switch (type)
        {
        case(NUMBER):
        {
            double number = *to_traverse.at(i).value.double_val;
            string value = to_string(number);

            final_map.insert(pair<string, string>(key_string, value));
            update_display_width(key_string);
            break;
        }
        case(OBJECT):
        {
            json_object * object = to_traverse.at(i).value.object;
            latest_object = object;

            object_keys_stack.push(key + "[" + index + "]");

            traverse_object(1);
            pop_object_stack();

            latest_object = latest_object->parent;
            break;
        }
        case(ARRAY):
        {
            json_array * array = to_traverse.at(i).value.array;
            latest_vector = array;

            object_keys_stack.push(key_string);

            traverse_array(0);
            pop_object_stack();
            break;
        }
        case(STRING):
        {
            string value = *to_traverse.at(i).value.string_val;

            final_map.insert(pair<string, string>(key_string, value));
            update_display_width(key_string);
            break;
        }
        case(NULL):
        {
            int null = *to_traverse.at(i).value.null;
            string value = to_string(null);

            final_map.insert(pair<string, string>(key_string, value));
            update_display_width(key_string);
            break;
        }
        case(BOOL):
        {
            int boolean = *to_traverse.at(i).value.boolean;
            string value = to_string(boolean);

            final_map.insert(pair<string, string>(key_string, value));
            update_display_width(key_string);
            break;
        }
        }
    }

    latest_vector = latest_vector->parent;

    if (mode == 1)
    {
        pop_object_stack();
    }
}

void json::pop_object_stack()
{
    if (object_keys_stack.size() > 0)
    {
        object_keys_stack.pop();
    }
}

void json::reset()
{
    latest_object = base_object;
    latest_vector = base_vector;
}

void json::update_display_width(string key)
{
    int new_width = key.length();

    if (new_width > key_width)
    {
        key_width = new_width;
    }

    locations.push_back(key);
}

string json::get(string location)
{
    return final_map.at(location);
}
