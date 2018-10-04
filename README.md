# json-parser
Parses valid JSON and displays values by object/array location.
## Parsing a file
### Input: json.txt (http://json.org/example.html)
```
{
    "glossary": {
        "title": "example glossary",
		"GlossDiv": {
            "title": "S",
			"GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
					"SortAs": "SGML",
					"GlossTerm": "Standard Generalized Markup Language",
					"Acronym": "SGML",
					"Abbrev": "ISO 8879:1986",
					"GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
						"GlossSeeAlso": ["GML", "XML"]
                    },
					"GlossSee": "markup"
                }
            }
        }
    }
}
```
### Output: results.txt
```
--------------------------------------------------------------------------------

JSON Parser
Version 0.1

Parses valid JSON.

--------------------------------------------------------------------------------

File: json.txt
Length: 706

--------------------------------------------------------------------------------

Key                 Value               Array Depth              Object Depth

                    {object start}      0                        1
glossary:           {object start}      0                        2
title:              "example glossa..."                                    
GlossDiv:           {object start}      0                        3
title:              "S"                                                    
GlossList:          {object start}      0                        4
GlossEntry:         {object start}      0                        5
ID:                 "SGML"                                                 
SortAs:             "SGML"                                                 
GlossTerm:          "Standard Gener..."                                    
Acronym:            "SGML"                                                 
Abbrev:             "ISO 8879:1986"                                        
GlossDef:           {object start}      0                        6
para:               "A meta-markup ..."                                    
GlossSeeAlso:       [array start]       1                        6
                    "GML"
                    "XML"
                    [array end]         0                        6
                    {object end}        0                        5
GlossSee:           "markup"                                               
                    {object end}        0                        4
                    {object end}        0                        3
                    {object end}        0                        2
                    {object end}        0                        1

--------------------------------------------------------------------------------

Location                                                                   Value

[glossary][GlossDiv][GlossList][GlossEntry][GlossDef][GlossSeeAlso][0]     GML
[glossary][GlossDiv][GlossList][GlossEntry][GlossDef][GlossSeeAlso][1]     XML
[glossary][GlossDiv][GlossList][GlossEntry][GlossDef][para]                A meta-markup language, used to create markup languages such as DocBook.
[glossary][GlossDiv][GlossList][GlossEntry][ID]                            SGML
[glossary][GlossDiv][GlossList][GlossEntry][SortAs]                        SGML
[glossary][GlossDiv][GlossList][GlossEntry][GlossTerm]                     Standard Generalized Markup Language
[glossary][GlossDiv][GlossList][GlossEntry][Acronym]                       SGML
[glossary][GlossDiv][GlossList][GlossEntry][Abbrev]                        ISO 8879:1986
[glossary][GlossDiv][GlossList][GlossEntry][GlossSee]                      markup
[glossary][GlossDiv][title]                                                S
[glossary][title]                                                          example glossary

--------------------------------------------------------------------------------

Stats

Key count:          15                  Decimal count:      0
Object count:       6                   Boolean count:      0
Array count:        1                   Max array depth:    1
String count:       11                  Max object depth:   6
Null count:         0                   Parse count:        50

--------------------------------------------------------------------------------
```
## Accessing a value by key and location
### main.cpp
```
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
```
### Output
```
example glossary
```
