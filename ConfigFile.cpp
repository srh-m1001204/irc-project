#include "ConfigFile.h"

ConfigFile::ConfigFile(string path) {
    this->path = path;
}
ConfigFile::~ConfigFile() {
}

bool ConfigFile::Load() {
    FILE *file = 0;
    if ((file = fopen(path.c_str(), "r")) == NULL) {
        perror("no config file found!");
        return false;
    }
    char buffer[1024];
    fread(buffer, sizeof(char), 1024, file);
    string content = string(buffer);

    NormalizeString(content);

    size_t pos1 = 0, pos2;
    string part;
    while ((pos2 = content.find(";", pos1)) != string::npos) {
        part = content.substr(pos1, pos2-pos1);
        ParseAndAddAttribute(part);
        pos1 = pos2 + 1;
    }
    fclose(file);
    return true;
}
bool ConfigFile::Save() {
    FILE *file = 0;
    if ((file = fopen(path.c_str(), "w")) == NULL) {
        perror("failed to save config file");
        return false;
    }
    stringstream sContent;
    for (vector<ConfigAttribute>::iterator it=attribute.begin(); it!=attribute.end(); it++)
        sContent << it->name << "=" << it->value << ";\n";
    fputs(sContent.str().c_str(), file);
    fclose(file);
    return true;
}
void ConfigFile::AddAttribute(string name, string value) {
    attribute.push_back(ConfigAttribute(name, value));
}
bool ConfigFile::SetValueOf(string name, string value) {
    for(vector<ConfigAttribute>::iterator it=attribute.begin(); it!=attribute.end(); it++) {
        if (it->name == name) {
            it->value = value;
            return true;
        }
    } return false;
}
string ConfigFile::GetValueOf(string name) {
    for(vector<ConfigAttribute>::iterator it=attribute.begin(); it!=attribute.end(); it++) {
        if (it->name == name)
            return it->value;
    } return "";
}

void ConfigFile::ParseAndAddAttribute(string attr) {
    size_t pos;
    if ((pos = attr.find("=")) == string::npos) {
        perror("couldn't parse config attribute...");
        return;
    }
    string name = attr.substr(0, pos);
    string value = attr.substr(pos+1, attr.length() - pos-1);
    AddAttribute(name, value);
}
void ConfigFile::NormalizeString(string &content) {
    size_t pos;
    while ((pos = content.find("\n")) != string::npos) {
        content.erase(pos, 1);
    }
    while ((pos = content.find(" ")) != string::npos) {
        content.erase(pos, 1);
    }
}
