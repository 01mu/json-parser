/*
* json-parser
* github.com/01mu
*/

#include "json-parser.h"

json::json(const char * file, bool print_history)
{
    int web;
    string text;

    cout << setiosflags(ios::left);

    if(file_exists(file))
    {
        text = file;
        web = 0;
    }
    else
    {
        CURL *curl;
        CURLcode res;

        curl = curl_easy_init();

        if(curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, file);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, json_ns::callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &text);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }

        web = 1;
    }

    if(web)
    {
        json_text = text;
    }
    else
    {
        json_text = get_file(text);
    }

    if(print_history)
    {
        cout << setw(WIDTH) << "Key" << setw(WIDTH) << "Value"
            << setw(NUM_WIDTH) << "Array Depth" << "Object Depth"
            << endl << endl;
    }

    this->print_history = print_history;
    this->file = file;

    len = json_text.size();

    remove_blank_leading();
    remove_blank_trail();

    char next = json_text.at(index);

    if(next == '[' || next == '{')
    {
        if(next == '[')
        {
            first_type = ARRAY;
        }
        else
        {
            first_type = OBJECT;
        }

        while(index != len)
        {
            parse();
        }

        reset();

        if(first_type == OBJECT)
        {
            traverse_object(0);
        }
        else
        {
            object_keys_stack.push("[0]");

            traverse_array();
        }
    }
    else
    {
        cout << json_text;
    }

    reset();
}

void json::parse()
{
    parse_count++;

    char next = json_text.at(index);

    if(next == ':')
    {
        check_key();
    }
    else if(isdigit(next) || next == '-' || next == '+' || next == '.')
    {
        string value = get_decimal();

        decimal_count++;

        if(print_history)
        {
            cout << setw(WIDTH) << " " << setw(WIDTH) << value
                << setw(NUM_WIDTH) << " " << setw(NUM_WIDTH) << " " << endl;
        }

        arr_add_number(stod(value));
    }
    else if(next == 'n')
    {
        null_count++;
        index = index + 4;

        if(print_history)
        {
            cout << setw(WIDTH) << " " << setw(WIDTH) << "null" << endl;
        }

        arr_add_null();
    }
    else if(next == 't')
    {
        string str = get_boolean('t');

        bool_count++;

        if(print_history)
        {
            cout << setw(WIDTH) << " " << setw(WIDTH) << str << setw(NUM_WIDTH)
                << " " << setw(NUM_WIDTH) << " " << endl;
        }

        arr_add_boolean(true);
    }
    else if(next == 'f')
    {
        string str = get_boolean('f');

        bool_count++;

        if(print_history)
        {
            cout << setw(WIDTH) << " " << setw(WIDTH) << str << setw(NUM_WIDTH)
                << " "<< setw(NUM_WIDTH) << " " << endl;
        }

        arr_add_boolean(false);
    }
    else if(next == '"')
    {
        string str = get_string();

        if(json_text.at(index) == ':')
        {
            key_count++;

            if(print_history)
            {
                cout << setw(WIDTH) << make_ellipses(str, 'k');
            }

            last_key_name = str;
        }
        else
        {
            string_count++;

            if(print_history)
            {
                cout << setw(WIDTH) << " " << make_ellipses(str, 'v') << endl;
            }

            arr_add_string(str);
        }
    }
    else if(next == '{')
    {
        index++;
        object_count++;

        update_obj_depth('i');

        if(print_history)
        {
            cout << setw(WIDTH) << " " << setw(WIDTH) << "{object start}"
                << setw(NUM_WIDTH) << array_depth << object_depth << endl;
        }

        arr_add_object();
    }
    else if(next == '}')
    {
        index++;

        update_obj_depth('d');

        if(print_history)
        {
            cout << setw(WIDTH) << " " << setw(WIDTH) << "{object end}"
                << setw(NUM_WIDTH) << array_depth << object_depth << endl;
        }

        end_object();
    }
    else if(next == '[')
    {
        index++;
        array_count++;

        update_arr_depth('i');

        if(print_history)
        {
            cout << setw(WIDTH) << " " << setw(WIDTH) << "[array start]"
                << setw(NUM_WIDTH) << array_depth << object_depth << endl;
        }

        arr_add_array();
    }
    else if(next == ']')
    {
        index++;

        update_arr_depth('d');

        if(print_history)
        {
            cout << setw(WIDTH) << " " << setw(WIDTH) << "[array end]"
                << setw(NUM_WIDTH) << array_depth << object_depth << endl;
        }

        end_array();
    }
    else if(next == '\n' || next == ' ' || next == '\t')
    {
        index++;
    }
    else if(next == ',')
    {
        index++;
    }

    remove_blank();
}

void json::check_key()
{
    index++;

    char next = json_text.at(index);

    if(next == '"')
    {
        string str = get_string();

        string_count++;

        if(print_history)
        {
            cout << setw(WIDTH) << make_ellipses(str, 'v') << setw(NUM_WIDTH)
                << " " << setw(JWIDTH) << " " << endl;
        }

        add_string(last_key_name, str);
    }
    else if(next == 't')
    {
        string str = get_boolean('t');

        bool_count++;

        if(print_history)
        {
            cout << setw(WIDTH) << str << setw(NUM_WIDTH) << " " << setw(JWIDTH)
                << " " << endl;
        }

        add_boolean(last_key_name, true);
    }
    else if(next == 'f')
    {
        string str = get_boolean('f');

        bool_count++;

        if(print_history)
        {
            cout << setw(WIDTH) << str << setw(NUM_WIDTH) << " " << setw(JWIDTH)
                << " " << endl;
        }

        add_boolean(last_key_name, false);
    }
    else if(next == 'n')
    {
        null_count++;
        index = index + 4;

        if(print_history)
        {
            cout << setw(WIDTH) << "null" << endl;
        }

        add_null(last_key_name);
    }
    else if(isdigit(next) || next == '-' || next == '+' || next == '.')
    {
        string value = get_decimal();

        decimal_count++;

        if(print_history)
        {
            cout << setw(WIDTH) << value << setw(NUM_WIDTH) << " "
                << setw(JWIDTH) << " " << endl;
        }

        add_number(last_key_name, stod(value));
    }
    else if(next == '{')
    {
        object_count++;
        index++;

        update_obj_depth('i');

        if(print_history)
        {
            cout << setw(WIDTH) << "{object start}" << setw(NUM_WIDTH)
                << array_depth << object_depth << endl;
        }

        add_object(last_key_name);
    }
    else if(next == '[')
    {
        array_count++;
        index++;

        update_arr_depth('i');

        if(print_history)
        {
            cout << "" << setw(WIDTH) << "[array start]" << setw(NUM_WIDTH)
                << array_depth << object_depth << endl;
        }

        add_array(last_key_name);
    }
    else if(next == '\n' || next == ' ' || next == '\t')
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

    while(prev_char == '\\')
    {
        char check = json_text.at(index);

        quote_count++;

        if(check == '"')
        {
            if(qc > 0)
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

    if(type == 't')
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

    bool is_neg = false;

    if(json_text.at(index) == '-' || json_text.at(index) == '+')
    {
        if(json_text.at(index) == '-')
        {
            is_neg = true;
        }

        index++;
    }

    start = index;
    num_char = json_text.at(start);

    char next = json_text.at(index + 1);

    if(json_text.at(index) == '0' &&  next != 0 && isdigit(next))
    {
        while(num_char == '0')
        {
            index++;
            start++;

            num_char = json_text.at(index);
        }
    }

    while(isdigit(num_char))
    {
        index++;

        num_char = json_text.at(index);

        offset++;
    }

    if(json_text.at(index) == '.')
    {
        num_char = json_text.at(index + 1);

        while(isdigit(num_char))
        {
            index++;

            num_char = json_text.at(index);

            offset++;
        }
    }

    if(json_text.at(index) == 'e')
    {
        index++;
        offset++;

        if(json_text.at(index) == '-' || json_text.at(index) == '+')
        {
            index++;
            offset++;
        }

        num_char = json_text.at(index);

        while(isdigit(num_char))
        {
            index++;

            num_char = json_text.at(index);

            offset++;
        }
    }

    string ret = json_text.substr(start, offset);

    if(is_neg == true)
    {
        ret = "-" + json_text.substr(start, offset);
    }

    return ret;
}

bool json::remove_blank()
{
    bool found = false;

    if(index + 1 < len)
    {
        char check = json_text.at(index);

        while(isspace(check) || check == '\n' || check == '\t')
        {
            index++;

            check = json_text.at(index);

            found = true;
        }
    }

    return found;
}

void json::remove_blank_leading()
{
    char check = json_text.at(index);

    while(isspace(check) || check == '\n' || check == '\t')
    {
        index++;

        check = json_text.at(index);
    }
}

void json::remove_blank_trail()
{
    char check = json_text.at(len - 1);

    while(check == ' ' || check == '\n' || check == '\t')
    {
        len--;

        check = json_text.at(len);
    }
}

void json::show_result()
{
    key_width += 5;

    for(int i = 0; i < final_map.size(); i++)
    {
        string location = locations.at(i);

        cout << setw(key_width) << location << final_map.at(location)
            << endl;
    }
}

void json::show_stats()
{
    cout << "Key count: " << key_count << endl;
    cout << "Decimal count: " << decimal_count << endl;
    cout << "Object count: " << object_count << endl;
    cout << "Boolean count: " << bool_count << endl;
    cout << "Array count: " << array_count << endl;
    cout << "Max array depth: " << array_depth_max << endl;
    cout << "String count: "  << string_count << endl;
    cout << "Max object depth: " << object_depth_max << endl;
    cout << "Null count: " << null_count << endl;
    cout << "Parse count: " << parse_count << endl;
}

void json::update_obj_depth(char type)
{
    if(type == 'i')
    {
        object_depth++;

        if(object_depth > object_depth_max)
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
    if(type == 'i')
    {
        array_depth++;

        if(array_depth > array_depth_max)
        {
            array_depth_max = array_depth;
        }
    }
    else
    {
        array_depth--;
    }
}

string json::make_ellipses(string & str, char type)
{
    string res;

    if(type == 'v')
    {
        int max = WIDTH - 6;

        if((int)str.length() > max)
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

        if((int)str.length() > max)
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
    if(latest_object->parent)
    {
        latest_object = latest_object->parent;
    }
}

void json::end_array()
{
    if(latest_vector->parent)
    {
        latest_vector = latest_vector->parent;
    }
}

void json::add_object(string & key)
{
    shared_ptr<json_object> newest (new json_object);
    newest->parent = latest_object;
    newest->key = key;

    latest_object->object_keys.push_back(key);
    latest_object->objects.insert(pair_object(key, newest));

    latest_object = newest;
}

void json::add_string(string & key, string & value)
{
    latest_object->string_keys.push_back(key);
    latest_object->strings.insert(pair_string(key, value));
}

void json::add_number(string & key, double value)
{
    latest_object->number_keys.push_back(key);
    latest_object->numbers.insert(pair_number(key, value));
}

void json::add_boolean(string & key, bool value)
{
    latest_object->boolean_keys.push_back(key);
    latest_object->booleans.insert(pair_bool(key, value));
}

void json::add_array(string & key)
{
    shared_ptr<json_array> newest (new json_array);
    newest->parent = latest_vector;
    newest->key = key;

    latest_object->array_keys.push_back(key);
    latest_object->arrays.insert(pair_array(key, newest));

    latest_vector = newest;
}

void json::add_null(string & key)
{
    latest_object->null_keys.push_back(key);
    latest_object->nulls.insert(pair_bool(key, 0));
}

void json::arr_add_string(string & value)
{
    shared_ptr<json_array_item> temp (new json_array_item);

    temp->type = STRING;
    temp->string_val = value;

    latest_vector->array.push_back(temp);
}

void json::arr_add_number(double value)
{
    shared_ptr<json_array_item> temp (new json_array_item);

    temp->type = NUMBER;
    temp->double_val = value;

    latest_vector->array.push_back(temp);
}

void json::arr_add_boolean(bool value)
{
    shared_ptr<json_array_item> temp (new json_array_item);

    temp->type = BOOL;
    temp->bool_val = value;

    latest_vector->array.push_back(temp);
}

void json::arr_add_null()
{
    shared_ptr<json_array_item> temp (new json_array_item);

    temp->type = NULL_;
    temp->null_val = 0;

    latest_vector->array.push_back(temp);
}

void json::arr_add_object()
{
    if(first_object == 0 && first_type == OBJECT)
    {
        shared_ptr<json_object> bo (new json_object);
        latest_object = bo;
        base_object = bo;
        first_object = 1;
    }
    else
    {
        shared_ptr<json_object> newest (new json_object);
        newest->parent = latest_object;

        shared_ptr<json_array_item> temp (new json_array_item);

        temp->type = OBJECT;
        temp->object_val = newest;

        latest_vector->array.push_back(temp);

        latest_object = newest;
    }
}

void json::arr_add_array()
{
    if(first_vector == 0 && first_type == ARRAY)
    {
        shared_ptr<json_array> bv (new json_array);
        latest_vector = bv;
        base_vector = bv;
        first_vector = 1;
    }
    else
    {
        shared_ptr<json_array> newest (new json_array);
        newest->parent = latest_vector;

        shared_ptr<json_array_item> temp (new json_array_item);

        temp->type = ARRAY;
        temp->array_val = newest;

        latest_vector->array.push_back(temp);

        latest_vector = newest;
    }
}

void json::traverse_object(int mode)
{
    string last_key = "";

    if(latest_object->key.length() > 0)
    {
        if(object_keys_stack.size() > 0)
        {
            string stack_top = object_keys_stack.top();
            string key = latest_object->key;

            last_key = stack_top + "[" + key + "]";
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

    if(mode == 0)
    {
        object_keys_stack.push(last_key);
    }

    for(int i = 0; i < 6; i++)
    {
        int check;

        switch(i)
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
        case(NULL_):
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

        for(int j = 0; j < check; j++)
        {
            string idx;
            string key = object_keys_stack.top();

            switch(i)
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
            case(NULL_):
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

                traverse_array();
                break;
            }
            }
        }
    }

    if(mode == 0)
    {
        pop_object_stack();
    }
}

void json::traverse_array()
{
    string key = object_keys_stack.top();
    json_vector to_traverse = latest_vector->array;

    int array_size = latest_vector->array.size();

    for(int i = 0; i < array_size; i++)
    {
        string index = to_string(i);
        string key_string = key + "[" + index + "]";

        json_array_item item = *to_traverse.at(i);
        int type = item.type;

        switch(type)
        {
        case(NUMBER):
        {
            double number = item.double_val;
            string value = to_string(number);

            final_map.insert(pair<string, string>(key_string, value));
            update_display_width(key_string);
            break;
        }
        case(OBJECT):
        {
            shared_ptr<json_object> object = item.object_val;
            latest_object = object;

            object_keys_stack.push(key + "[" + index + "]");

            traverse_object(1);
            pop_object_stack();

            latest_object = latest_object->parent;
            break;
        }
        case(ARRAY):
        {
            shared_ptr<json_array> array = item.array_val;
            latest_vector = array;
            object_keys_stack.push(key_string);

            traverse_array();
            break;
        }
        case(STRING):
        {
            string value = item.string_val;

            final_map.insert(pair<string, string>(key_string, value));
            update_display_width(key_string);
            break;
        }
        case(NULL_):
        {
            int null = item.null_val;
            string value = to_string(null);

            final_map.insert(pair<string, string>(key_string, value));
            update_display_width(key_string);
            break;
        }
        case(BOOL):
        {
            int boolean = item.bool_val;
            string value = to_string(boolean);

            final_map.insert(pair<string, string>(key_string, value));
            update_display_width(key_string);
            break;
        }
        }
    }

    pop_object_stack();

    if(latest_vector->parent)
    {
        latest_vector = latest_vector->parent;
    }
}

void json::pop_object_stack()
{
    if(object_keys_stack.size() > 0)
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

    if(new_width > key_width)
    {
        key_width = new_width;
    }

    locations.push_back(key);
}

string json::get(string location)
{
    return final_map.at(location);
}

void json::output_json_file()
{
    if(first_type == OBJECT)
    {
        json_output += "{";

        output_json_obj();

        json_output += "}";
    }
    else
    {
        json_output += "[";

        output_json_arr();

        json_output += "]";
    }

    std::ofstream out("json-parser-new");

    out << json_output;
    out.close();
}

void json::output_json_obj()
{
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

    int counts[6] = {string_count, number_count, bool_count, null_count,
        object_count, array_count};

    int accessed = 0;
    int value_count = 0;

    for(int i = 0; i < 6; i++)
    {
        if(counts[i] > 0)
        {
            accessed++;
        }
    }

    for(int i = 0; i < 6; i++)
    {
        int check;

        switch(i)
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
        case(NULL_):
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

        if(check > 0)
        {
            value_count++;
        }

        for(int j = 0; j < check; j++)
        {
            string idx;
            string value;

            switch(i)
            {
            case(STRING):
            {
                idx = string_keys.at(j);
                value = strings.at(idx);
                json_output += "\"" + idx + "\":\"" + value + "\"";
                break;
            }
            case(NUMBER):
            {
                idx = number_keys.at(j);
                value = to_string(numbers.at(idx));
                json_output += "\"" + idx + "\":" + value;
                break;
            }
            case(BOOL):
            {
                idx = bool_keys.at(j);
                value = to_string(bools.at(idx));
                json_output +=  + "\"" + idx + "\":" + value;
                break;
            }
            case(NULL_):
            {
                idx = null_keys.at(j);
                value = to_string(nulls.at(idx));
                json_output += "\"" + idx + "\":" + "null";
                break;
            }
            case(OBJECT):
            {
                idx = object_keys.at(j);
                latest_object = objects.at(idx);
                json_output += "\"" + idx + "\":{";
                output_json_obj();
                json_output += "}";
                latest_object = latest_object->parent;
                break;
            }
            case(ARRAY):
            {
                idx = array_keys.at(j);
                latest_vector = arrays.at(idx);
                json_output += "\"" + idx + "\":[";
                output_json_arr();
                json_output += "]";
                latest_vector = latest_vector->parent;
                break;
            }
            }

            if(j != check - 1)
            {
                json_output += ",";
            }
        }

        if(check > 0 && value_count != accessed)
        {
           json_output += ",";
        }
    }
}

void json::output_json_arr()
{
    json_vector to_traverse = latest_vector->array;
    int array_size = latest_vector->array.size();

    for(int i = 0; i < array_size; i++)
    {
        string index = to_string(i);
        string value;

        json_array_item item = *to_traverse.at(i);
        int type = item.type;

        switch(type)
        {
        case(NUMBER):
        {
            double number = item.double_val;
            value = to_string(number);

            json_output +=  value;
            break;
        }
        case(OBJECT):
        {
            shared_ptr<json_object> object = item.object_val;
            latest_object = object;
            json_output +=  "{";
            output_json_obj();
            json_output +=  "}";
            latest_object = latest_object->parent;
            break;
        }
        case(ARRAY):
        {
            shared_ptr<json_array> array = item.array_val;
            latest_vector = array;
            json_output +=  "[";
            output_json_arr();
            json_output +=  "]";
            latest_vector = latest_vector->parent;
            break;
        }
        case(STRING):
        {
            value = item.string_val;
            json_output +=  "\"" + value + "\"";
            break;
        }
        case(NULL_):
        {
            int null = item.null_val;
            value = to_string(null);
            json_output += value;
            break;
        }
        case(BOOL):
        {
            int boolean = item.bool_val;
            value = to_string(boolean);
            json_output +=  value;
            break;
        }
        }

        if(i != array_size - 1)
        {
            json_output +=  ",";
        }
    }
}

bool json::file_exists(const char * name)
{
    std::ifstream infile(name);
    return infile.good();
}
