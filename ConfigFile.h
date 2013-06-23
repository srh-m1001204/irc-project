#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

class ConfigAttribute {
public:
    ConfigAttribute(string name, string value) : name(name), value(value) {};
    ~ConfigAttribute() {};
    string name;
    string value;
};

class ConfigFile {
public:
    ConfigFile(string path);
    ~ConfigFile();

    vector<ConfigAttribute> attribute;

    bool    Load();
    bool    Save();

    void    AddAttribute(string name, string value);

    bool    SetValueOf(string name, string value);
    string  GetValueOf(string name);
private:
    string  path;

    void    ParseAndAddAttribute(string attr);
    void    NormalizeString(string &content);
};

#endif
