/*
* json-parser
* github.com/01mu
*/

#include "json-parser.h"

static size_t callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool file_exists(const char * name)
{
    std::ifstream infile(name);
    return infile.good();
}

int main(int argc, char ** argv)
{
    string text;
    int web;

    if(file_exists(argv[1]))
    {
        text = argv[1];
        web = 0;
    }
    else
    {
        CURL *curl;
        CURLcode res;

        curl = curl_easy_init();

        if(curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &text);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }

        web = 1;
    }

    json thing = json(text, 0, web);
    thing.show_result();

    return 0;
}
