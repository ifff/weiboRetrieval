/**************************************************************
**
** Author:          Liang Feng
** Mail:            liangfeng1987@gmail.com
** FileName:        Cmdline.h
** Create Time:     2012-12-10 22:03
** Edit Time:       2014-03-18 21:24, Runwei Qiang
** Description:     This is a util class to read all parameters
**
**************************************************************/

#ifndef CMDLINE_H_
#define CMDLINE_H_

#include <map>
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

class Cmdline{
    private:
        map<string, string> value;
        string help;

        bool parse_param(string &s);
    public:
        Cmdline(int argc, char **argv);
        bool has_param(const string &param);
        void check_param();
        const string& get_value(const string &param, const string &default_val);
        const int get_value(const string &param, const int default_val);
        const double get_value(const string &param, const double default_val);
        const pair<int, int> get_value(const string &param, const int first, const int second);
        const pair<double, double> get_value(const string &param, const double first, const double second);
};

#endif /*CMDLINE_H_*/
