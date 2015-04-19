/**
 * author:  Feng Liang <liangfeng1987@gmail.com>
 * file:    Cmdline.cpp
 * create_time: 2013-03-30
** edit Time:       2014-03-18 21:24, Runwei Qiang
 * description: implementation of Cmdline.h
 */

#include "../include/Cmdline.h"

bool Cmdline::parse_param(string &s){
    if(s.size() > 1 && s.at(0) == '-'){
        s = s.substr(1);
    }
    else{
        return false;
    }
}             

Cmdline::Cmdline(int argc, char **argv){
    help = "[USAGE] ./main -input filename -output filename" \
            "-index path -ssf path -query path\n";
    // skip linFM command
    int index = 1;
    while(index < argc){
        string parameter(argv[index]);
        if(parse_param(parameter)){
            if(value.find(parameter) != value.end()){
                throw "the parameter " + parameter + " is duplicated";
            }
            if((index+1) < argc){
                string next_param(argv[index+1]);
                if(! parse_param(next_param)){
                    value[parameter] = next_param;
                    ++ index;
                }
                else{
                    throw "no value for parameter " + parameter;
                }
            }
            else{
                throw "no value for parameter " + parameter; 
            }
        }
        else{
            throw "cannot parse " + parameter;
        }
        index ++;
    }
}

bool Cmdline::has_param(const string &param){
    return value.find(param) != value.end();
}

void Cmdline::check_param(){
    if(! has_param("index")){
        throw "no index parameter specified...\n" + help;
    }
    if(! has_param("ssf")){
        throw "no ssf parameter specified...\n" + help;
    }
    if(! has_param("query")){
        throw "no query path parameter specified...\n" + help;
    }
    if(! has_param("expand")){
        throw "no expand query path parameter specified...\n" + help;
    }
    if(! has_param("output")){
        throw "no output parameter specified...\n" + help;
    }
}

const string& Cmdline::get_value(const string &param, const string &default_val){
    if(has_param(param)){
        return value[param];
    }
    else{
        return default_val;
    }
}

const int Cmdline::get_value(const string &param, const int default_val){
    if(has_param(param)){
        return atoi(value[param].c_str());
    }
    else{
        return default_val;
    }
}

const double Cmdline::get_value(const string &param, const double default_val){
    if(has_param(param)){
        return atof(value[param].c_str());
    }
    else{
        return default_val;
    }
}

const pair<int, int> Cmdline::get_value(const string &param, const int first, const int second){
    if(has_param(param)){
        string pattern = ",";
        string val = value[param];
        int pos = val.find(pattern, 0);
        int first_val = atoi(val.substr(0, pos).c_str());
        int second_val = atoi(val.substr(pos+1).c_str());
        return make_pair(first_val, second_val);
    }
    else{
        return make_pair(first, second);
    }
} 

const pair<double, double>  Cmdline::get_value(const string &param, const double first, const double second){
    if(has_param(param)){
        string pattern = ",";
        string val = value[param];
        int pos = val.find(pattern, 0);
        double first_val = atof(val.substr(0, pos).c_str());
        double second_val = atof(val.substr(pos+1).c_str());
        return make_pair(first_val, second_val);
    }
    else{
        return make_pair(first, second);
    }
}                                                                     
