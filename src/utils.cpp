#include "utils.h"

vector<string> split_str(const string &str, const string &dlm)
{
    char* strc = new char [strlen(str.c_str()) + 1];

    strcpy(strc, str.c_str());

    vector<string> strVec;

    char* tmp = strtok(strc, dlm.c_str());

    while(tmp != NULL)
    {
        strVec.push_back(string(tmp));

        tmp = strtok(NULL, dlm.c_str());
    }

    delete[] strc;

    return strVec;
}