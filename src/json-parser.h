/*
* json-parser
* github.com/01mu
*/

#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <ctype.h>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

using namespace std;

class json;
struct json_object;
struct json_array;

struct json_array_item
{
    int type;
    shared_ptr<json_object> object_val;
    shared_ptr<json_array> array_val;
    int null_val;
    bool bool_val;
    double double_val;
    string string_val;
};

typedef vector<shared_ptr<json_array_item>> json_vector;

typedef pair<string, shared_ptr<json_array>> pair_array;
typedef pair<string, string> pair_string;
typedef pair<string, int> pair_null;
typedef pair<string, double> pair_number;
typedef pair<string, bool> pair_bool;
typedef pair<string, shared_ptr<json_object>> pair_object;

typedef map<string, shared_ptr<json_array>> map_array;
typedef map<string, string> map_string;
typedef map<string, int> map_null;
typedef map<string, double> map_number;
typedef map<string, bool> map_bool;
typedef map<string, shared_ptr<json_object>> map_object;

struct json_array
{
    string key;
    shared_ptr<json_array> parent;
    json_vector array;
};

struct json_object
{
    string key;
    shared_ptr<json_object> parent;
    vector<string> array_keys;
    vector<string> string_keys;
    vector<string> null_keys;
    vector<string> number_keys;
    vector<string> boolean_keys;
    vector<string> object_keys;
    map_array arrays;
    map_string strings;
    map_null nulls;
    map_number numbers;
    map_bool booleans;
    map_object objects;
};

class json
{
private:
    static const int NUMBER = 0;
    static const int OBJECT = 1;
    static const int ARRAY = 2;
    static const int STRING = 3;
    static const int NULL_ = 4;
    static const int BOOL = 5;

    string json_output = "";

    bool print_history;
    int index = 0;

    string file;
    string json_text;

    int array_depth_max = 0;
    int array_depth = 0;

    int object_depth_max = 0;
    int object_depth = 0;

    int len;

    int WIDTH = 20;
    int NUM_WIDTH = 25;
    int JWIDTH = 10;
    int key_width = 10;

    int string_count = 0;
    int bool_count = 0;
    int array_count = 0;
    int object_count = 0;
    int decimal_count = 0;
    int key_count = 0;
    int parse_count = 0;
    int null_count = 0;

    shared_ptr<json_object> base_object;
    shared_ptr<json_array> base_vector;

    shared_ptr<json_object> latest_object;
    shared_ptr<json_array> latest_vector;

    stack<string> object_keys_stack;

    string last_obj_key;
    string last_key_name;

    map<string, string> final_map;
    vector<string> locations;

    bool first_object = 0;
    bool first_vector = 0;

    int first_type = 0;

public:
    json(const char * file, bool print_history);

    void parse();
    void check_key();
    void show_title();
    void show_stats();
    void show_result();

    string get_file(string file_name);
    string get_string();
    string get_boolean(char type);
    string get_decimal();

    void remove_blank_leading();
    void remove_blank_trail();
    bool remove_blank();

    void update_arr_depth(char type);
    void update_obj_depth(char type);
    string make_ellipses(string & str, char type);
    string get(string location);

    void update_display_width(string key);

    void add_object(string & key);
    void add_string(string & key, string & value);
    void add_number(string & key, double value);
    void add_boolean(string & key, bool value);
    void add_array(string & key);
    void add_null(string & key);

    void end_object();

    void arr_add_object();
    void arr_add_string(string & value);
    void arr_add_number(double value);
    void arr_add_boolean(bool value);
    void arr_add_array();
    void arr_add_null();

    void end_array();

    void traverse_object(int mode);
    void traverse_array();
    void reset();
    void pop_object_stack();

    void output_json_file();
    void output_json_obj();
    void output_json_arr();

    bool file_exists(const char * name);
};

namespace json_ns
{
    static size_t callback(void * contents, size_t sz, size_t nb, void * userp)
    {
        ((std::string*)userp)->append((char*)contents, sz * nb);
        return sz * nb;
    }
}

#endif
