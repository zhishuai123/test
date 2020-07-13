// json.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include"json.h"
using namespace std;
//类名大写
//函数名下划线
//变量名驼峰命名法
class OBJECT {
    unordered_map<string,JSON> object;
public:
    OBJECT(unordered_map<string, JSON> obj) :object(obj) {};
    OBJECT(const OBJECT& obj) :object(obj.object) {};
    bool set(string key,const JSON& json);
    JSON get(string key);
    bool del(string key);
    ~OBJECT() {};
};
bool OBJECT::set(string key, const JSON& json)
{
    object[key] = json;
    return true;
}
JSON OBJECT::get(string key)
{
    if (object.find(key) != object.end())
        return object[key];
    return JSON_NULL(JSON_NONE);
}
bool OBJECT::del(string key)
{
    //不知道什么情况返回false
    object.erase(key);
    return true;
}

class ARRAY {
    vector<JSON> array;                
public:
    ARRAY(const ARRAY& arr);
    bool set_by_idx(U32 index, const JSON& json);
    bool add(const JSON& json);
    bool del_by_idx(U32 index);
    JSON get_by_idx(U32 index);
    U32 size();
    ~ARRAY() {};
};
bool ARRAY::set_by_idx(U32 index, const JSON& json)
{
    array[index] = json;
    return true;
}
bool ARRAY::del_by_idx(U32 index)
{
    if (index >= array.size())
        return false;
    array.erase(array.begin() + index);
    return true;
}
bool ARRAY::add(const JSON& json)
{
    array.push_back(json);
    return true;
}
JSON ARRAY::get_by_idx(U32 index)
{
    if (index >= array.size())
        return JSON_NULL(JSON_NONE);
    return array[index];
}
U32 ARRAY::size()
{
    return array.size();
}


class JSON {
    json_e type;
public:
    JSON(json_e type) :type(type) {};
    json_e get_type()const;
    virtual string to_string();
    virtual ~JSON() {};
};
json_e JSON::get_type() const
{
    return type;
}
string JSON::to_string()
{
    return "";
}


class JSON_NUMBER :public JSON {
    double num;
public:
    JSON_NUMBER(double num) :JSON(JSON_NUM), num(num) {}
    JSON_NUMBER(const JSON_NUMBER& jsonNum) :JSON(jsonNum.get_type()), num(jsonNum.num) {}     //拷贝构造

    JSON_NUMBER(string str);                                                                   //反序列化
    string to_string();                                                                     //序列化

    double get_num();
    bool set(double num);

    virtual ~JSON_NUMBER();
};
JSON_NUMBER::JSON_NUMBER(string str):JSON(JSON_NUM)
{
    num = stod(str);
}
string JSON_NUMBER::to_string()
{
    return std::to_string(num);
}

class JSON_BOOL :public JSON {
    bool boolValue;
public:
    JSON_BOOL(bool boolValue) :JSON(JSON_BOL), boolValue(boolValue) {}
    JSON_BOOL(const JSON_BOOL& jsonBool) :JSON(jsonBool.get_type()), boolValue(jsonBool.boolValue) {}     //拷贝构造

    JSON_BOOL(string str);                                                                   //反序列化
    virtual string to_string();                                                              //序列化

    bool get_bol();
    bool set(bool boolValue);

    virtual ~JSON_BOOL();

};
JSON_BOOL::JSON_BOOL(string str)
{
    if (str == "true" || str == "TRUE")
        boolValue = true;
    else if()
}


class JSON_STRING :public JSON {
    string str;
public:
    JSON_STRING(string str) :JSON(JSON_STR), str(str) {}
    JSON_STRING(const JSON_STRING& jsonStr) :JSON(jsonStr.get_type()), str(jsonStr.str) {}     //拷贝构造

    JSON_STRING(string str);                                                                //反序列化
    virtual string to_string();                                                          //序列化

    string get_str();
    bool set(string str);

    virtual ~JSON_STRING();

};

class JSON_ARRAY :public JSON {
    ARRAY arr;
public:
    JSON_ARRAY(ARRAY arr) :JSON(JSON_ARR), arr(arr) {}
    JSON_ARRAY(const JSON_ARRAY& jsonArr) :JSON(jsonArr.get_type()), arr(jsonArr.arr) {}     //拷贝构造

    JSON_ARRAY(string str);                                                                //反序列化
    virtual string to_string();                                                          //序列化

    ARRAY get_arr_val();
    ARRAY& get_arr();
    bool set(const ARRAY& arr);

    virtual ~JSON_ARRAY();
};

class JSON_OBJECT :public JSON {
    OBJECT obj;
public:
    JSON_OBJECT(OBJECT obj) :JSON(JSON_OBJ), obj(obj) {}
    JSON_OBJECT(const JSON_OBJECT& jsonObj) :JSON(jsonObj.get_type()), obj(jsonObj.obj) {}     //拷贝构造

    JSON_OBJECT(string str);                                                                //反序列化
    virtual string to_string();                                                          //序列化

    OBJECT get_obj_val();
    OBJECT& get_obj();
    bool set(const OBJECT& obj);

    virtual ~JSON_OBJECT() {};
};

class JSON_NULL :public JSON {
public:
    JSON_NULL(json_e type) :JSON(type){}
    JSON_NULL(string str);                                                                //反序列化
    virtual string to_string();                                                           //序列化
    virtual ~JSON_NULL() {};
};


//                                  //没有继承版
//class Object {
//    map<string, JSON> object;
//public:
//
//};
//class Array {
//    vector<JSON> array;
//public:
//
//};
//class JSON {
//    json_e type;
//    bool boolValue;
//    double num;
//    string str;
//    Array arr;
//    Object obj;
//public:
//    JSON(json_e type);
//    JSON(const JSON& json);
//    JSON(string str);
//    ~JSON();
//
//    bool set_num(double val);
//
//    bool set_bool(bool val);
//    bool set_str(string str);
//    bool set_arr(Array arr);
//    bool set_obj(Object obj);
//
//    double get_num_val();
//    bool get_bool_val();
//    string get_str_val();
//    Array get_arr_val();
//    Object get_obj_val();
//    json_e get_type_val();
//
//    string& get_str();
//    Array& get_arr();
//    Object& get_obj();
//
//
//
//};
int main()
{
    return 0;
}
