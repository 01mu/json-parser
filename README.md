# json-parser
Parses valid JSON and displays values by object/array location.
## Usage
### Parsing a JSON file and returning simplified output
Source: http://json.org/example.html
```
{"widget": {
    "debug": "on",
    "window": {
        "title": "Sample Konfabulator Widget",
        "name": "main_window",
        "width": 500,
        "height": 500
    },
    "image": {
        "src": "Images/Sun.png",
        "name": "sun1",
        "hOffset": 250,
        "vOffset": 250,
        "alignment": "center"
    },
    "text": {
        "data": "Click Here",
        "size": 36,
        "style": "bold",
        "name": "text1",
        "hOffset": 250,
        "vOffset": 100,
        "alignment": "center",
        "onMouseUp": "sun1.opacity = (sun1.opacity / 100) * 90;"
    }
}}
```
```c++
#include <iostream>
#include "json-parser.cpp"

using namespace std;

int main()
{
    const char * src = "input";

    json thing = json(src, 0);

    cout << "JSON: " << endl;
    thing.show_result();

    cout << endl << "Stats: " << endl;
    thing.show_stats();

    return 0;
}
```
```
JSON:
[widget][window][width]        500.000000
[widget][window][height]       500.000000
[widget][window][title]        Sample Konfabulator Widget
[widget][window][name]         main_window
[widget][image][hOffset]       250.000000
[widget][image][vOffset]       250.000000
[widget][image][src]           Images/Sun.png
[widget][image][name]          sun1
[widget][image][alignment]     center
[widget][text][size]           36.000000
[widget][text][hOffset]        250.000000
[widget][text][vOffset]        100.000000
[widget][text][data]           Click Here
[widget][text][style]          bold
[widget][text][name]           text1
[widget][text][alignment]      center
[widget][text][onMouseUp]      sun1.opacity = (sun1.opacity / 100) * 90;
[widget][debug]                on

Stats:
Key count: 22
Decimal count: 7
Object count: 5
Boolean count: 0
Array count: 0
Max array depth: 0
String count: 11
Max object depth: 3
Null count: 0
Parse count: 66
```
### Getting JSON from a URL
```c++
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
```
```
[data][dist]                                                  25.000000
[data][children][0][data][gilded]                             0.000000
[data][children][0][data][pwls]                               6.000000
[data][children][0][data][downs]                              0.000000
[data][children][0][data][ups]                                688.000000
[data][children][0][data][score]                              688.000000

(...)

[data][children][24][data][is_video]                          0
[data][children][24][kind]                                    t3
[data][modhash]
[data][after]                                                 t3_a6vt1g
[data][before]                                                0
[kind]                                                        Listing
```
### Creating JSON from values
```c++
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
```
```
{"a":{"b":"c","d":"e"},"f":[0.000000,1.000000,2.000000,3.000000,
4.000000,5.000000,6.000000,7.000000,8.000000,9.000000,[0.000000,
1.000000,2.000000,3.000000,4.000000,5.000000,6.000000,7.000000,
8.000000,9.000000]],"g":0}

JSON:
[a][b]         c
[a][d]         e
[f][0]         0.000000
[f][1]         1.000000
[f][2]         2.000000
[f][3]         3.000000
[f][4]         4.000000
[f][5]         5.000000
[f][6]         6.000000
[f][7]         7.000000
[f][8]         8.000000
[f][9]         9.000000
[f][10][0]     0.000000
[f][10][1]     1.000000
[f][10][2]     2.000000
[f][10][3]     3.000000
[f][10][4]     4.000000
[f][10][5]     5.000000
[f][10][6]     6.000000
[f][10][7]     7.000000
[f][10][8]     8.000000
[f][10][9]     9.000000
[g]            0
```
