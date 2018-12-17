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
