// json.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<stack>
#include"json.h"
using namespace std;
//类名大写
//函数名下划线
//变量名驼峰命名法
json_e JSON::get_type() const
{
    return type;
}
string JSON::json_to_string()
{
    return "";
}

JSON& JSON::clone()
{
    return *this;
}
void JSON::set_type(json_e type)
{
    this->type = type;
}
JSON* JSON::copy()         //这个函数不该被执行
{
    return new JSON(JSON_NONE);
}

bool OBJECT::set(string key, shared_ptr<JSON> json)
{
    object[key] = json;
    return true;
}
shared_ptr<JSON> OBJECT::get(string key)
{
    if (object.find(key) != object.end())
        return object[key];
    return shared_ptr<JSON>(new JSON_NULL());
}
bool OBJECT::del(string key)
{
    //不知道什么情况返回false
    object.erase(key);
    return true;
}
OBJECT& OBJECT::operator=(const OBJECT& obj)                    //深拷贝
{
    //object = obj.object;         //好像不需要了 如果有错先看这里
    object.clear();
    for (auto i = obj.object.begin(); i != obj.object.end(); i++)
        object[i->first] = shared_ptr<JSON>((i->second)->copy());
    return *this;
}
unordered_map<string, shared_ptr<JSON>>& OBJECT::get_object()
{
    return object;
}

bool ARRAY::set_by_idx(U32 index, shared_ptr<JSON> json)
{
    JSON_ARRAY* json_tem = static_cast<JSON_ARRAY*>(&*json);
    if (index >= json_tem->get_arr().size())
        return false;
    arry[index] = json;
    return true;
}
bool ARRAY::del_by_idx(U32 index)
{
    if (index >= arry.size())
        return false;
    arry.erase(arry.begin() + index);
    return true;
}
bool ARRAY::add(shared_ptr<JSON> json)
{
    arry.push_back(json);
    return true;
}
shared_ptr<JSON> ARRAY::get_by_idx(U32 index)
{
    if (index >= arry.size())
        return shared_ptr<JSON>(new JSON_NULL());
    return arry[index];
}
U32 ARRAY::size()
{
    return arry.size();
}
ARRAY& ARRAY::operator=(const ARRAY& arr)                       //深拷贝
{
    arry.clear();
    for (auto i = arr.arry.begin(); i != arr.arry.end(); i++)
        arry.push_back(shared_ptr<JSON>((*i)->copy()));
    return *this;
}

string JSON_NUMBER::json_to_string()
{
    return std::to_string(num);
}
double JSON_NUMBER::get_num()
{
    return num;
}
bool JSON_NUMBER::set(double num)
{
    this->num = num;
    return true;
}
JSON_NUMBER* JSON_NUMBER::copy()
{
    return new JSON_NUMBER(num);
}


JSON_BOOL::JSON_BOOL(string str) :JSON(JSON_BOL), boolValue(true)
{
    if (str == "true")
        boolValue = true;
    else if (str == "fasle")
        boolValue = false;
}
string JSON_BOOL::json_to_string()
{
    return boolValue ? "true" : "false";
}
bool JSON_BOOL::get_bol()
{
    return boolValue;
}
bool JSON_BOOL::set(bool bol)
{
    boolValue = bol;
    return true;
}
JSON_BOOL* JSON_BOOL::copy()
{
    return new JSON_BOOL(boolValue);
}


string JSON_STRING::get_str()
{
    return str;
}
string JSON_STRING::json_to_string()
{
    return "\"" + str + "\"";
}
JSON_STRING* JSON_STRING::copy()
{
    return new JSON_STRING(str);
}
bool JSON_STRING::set(string str)
{
    this->str = str;
    return true;
}


string JSON_ARRAY::json_to_string()
{ 
    if (arr.size() == 0)
        return "[]";
    string str= "[";
    str += arr.get_by_idx(0)->json_to_string();
    for (int i = 1; i < arr.size(); i++)
        str += ("," + arr.get_by_idx(i)->json_to_string());
    str += "]";
    return str;
}
JSON_ARRAY* JSON_ARRAY::copy()
{
    return new JSON_ARRAY(arr);                                 //调用ARRAY的拷贝构造函数  拷贝构造和虚函数copy递归 在num string bool类型的JSON处终止递归
}
ARRAY& JSON_ARRAY::get_arr()
{
    return arr;
}
bool JSON_ARRAY::set(const ARRAY& arr)
{
    this->arr = arr;
    return true;
}
JSON_ARRAY::JSON_ARRAY(string str):JSON(JSON_ARR)
{
    string tem;                                         //存放要反序列化的子串
    stack<char> sta;                                   
    for (int i = 0; i <= str.size(); i++)                 //判断条件加了等于 因为i==str.size()时需要将最后一个字串反序列化，下标不会越界
    {
        if (str[i] == ','&&sta.empty()||i==str.size())                //如果遇到逗号,不用放到tem中
        {
            if (tem.size() == 0)                        //对错误情况暂不处理
                return;                         
            if (tem[0] == '{')
                arr.add(shared_ptr<JSON>(new JSON_OBJECT(tem.substr(1, tem.size() - 2))));
            else if (tem[0] == '[')
                arr.add(shared_ptr<JSON>(new JSON_ARRAY(tem.substr(1, tem.size() - 2))));
            else if (tem[0] == '"')
                arr.add(shared_ptr<JSON>(new JSON_STRING(tem.substr(1, tem.size() - 2))));
            else if (tem[0] == 't' && tem.size() == 4)
                arr.add(shared_ptr<JSON>(new JSON_BOOL(true)));
            else if (tem[0] == 'f' && tem.size() == 5)
                arr.add(shared_ptr<JSON>(new JSON_BOOL(false)));
            else if (tem[0] == 'n' && tem.size() == 4)
                arr.add(shared_ptr<JSON>(new JSON_NULL()));
            else
                arr.add(shared_ptr<JSON>(new JSON_NUMBER(tem)));
            tem.clear();
            continue;                          
        }
        if (str[i] == '{' || str[i] == '[')
            sta.push(str[i]);
        else if (str[i] == '}' || str[i] == ']')    //暂时没有考虑括号不匹配的错误情况
            sta.pop();
        else if (str[i] == '"')
        {
            if (sta.empty()||sta.top()!='"')
                sta.push(str[i]);
            else
                sta.pop();
        }
        tem += string(1,str[i]);
    }

}

string JSON_OBJECT::json_to_string()
{   if (obj.get_object().size() == 0)
        return "{}";
    string str = "{";
    auto i = obj.get_object().begin();
    str += ("\"" + i->first + "\":" + i->second->json_to_string());
    i++;
    for (; i != obj.get_object().end(); i++)
        str += (",\"" + i->first + "\":" + i->second->json_to_string());
    str += "}";
    return str;
}
JSON_OBJECT* JSON_OBJECT::copy()
{
    return new JSON_OBJECT(obj);                                //调用OBJECT的拷贝构造函数  拷贝构造和虚函数copy递归 在num string bool类型的JSON处终止递归
}
OBJECT& JSON_OBJECT::get_obj()
{
    return obj;
}
bool JSON_OBJECT::set(const OBJECT& obj)
{
    this->obj = obj;
    return true;
}
JSON_OBJECT::JSON_OBJECT(string str):JSON(JSON_OBJ)
{
    bool temIsKey = true;                                   //true表示tem里是key  false表示tem里是value的序列化结果
    string key;
    string tem;                                         //存放要反序列化的子串
    stack<char> sta;
    for (int i = 0; i <= str.size(); i++)                 //判断条件加了等于 因为i==str.size()时需要将最后一个字串反序列化，下标不会越界
    {
        if (!temIsKey&&str[i] == ',' && sta.empty() || i == str.size())                //如果遇到逗号,不用放到tem中
        {
            if (tem.size() == 0)                        //对错误情况暂不处理
                return;
            if (tem[0] == '{')
                obj.set(key, shared_ptr<JSON>(new JSON_OBJECT(tem.substr(1, tem.size() - 2))));
            else if (tem[0] == '[')
                obj.set(key, shared_ptr<JSON>(new JSON_ARRAY(tem.substr(1, tem.size() - 2))));
            else if (tem[0] == '"')
                obj.set(key, shared_ptr<JSON>(new JSON_STRING(tem.substr(1, tem.size() - 2))));
            else if (tem[0] == 't' && tem.size() == 4)
                obj.set(key, shared_ptr<JSON>(new JSON_BOOL(true)));
            else if (tem[0] == 'f' && tem.size() == 5)
                obj.set(key, shared_ptr<JSON>(new JSON_BOOL(false)));
            else if (tem[0] == 'n' && tem.size() == 4)
                obj.set(key, shared_ptr<JSON>(new JSON_NULL()));
            else
                obj.set(key, shared_ptr<JSON>(new JSON_NUMBER(tem)));
            tem.clear();
            temIsKey = true;
            continue;
        }

        if (str[i] == '{' || str[i] == '[')           //对stack操作  根据stack是否为空来判断当前子字符串是要递归的子串还是要在当前函数处理的子串
            sta.push(str[i]);
        else if (str[i] == '}' || str[i] == ']')    //暂时没有考虑括号不匹配的错误情况
            sta.pop();
        else if (str[i] == '"')
        {
            if (sta.empty() || sta.top() != '"')
                sta.push(str[i]);
            else
                sta.pop();
        }
        if (str[i] == ':'&&sta.empty())
        {
            key = tem.substr(1, tem.size() - 2);
            tem.clear();
            temIsKey = false;
            continue;
        }
        tem += string(1, str[i]);
    }
}

string JSON_NULL::json_to_string()
{
    return "null";
}
JSON_NULL* JSON_NULL::copy()
{
    return new JSON_NULL();
}


shared_ptr<JSON> string_to_json(string json)                  //反序列化 返回一个JSON对象  没有错误情况处理 输入不是JSON的时候 结果未知
{
    for (auto i = json.begin(); i != json.end();)
    {
        if (*i == '\n' || *i == ' ' || *i == '\t')
            i = json.erase(i);
        else
            i++;
    }
    if (!json.empty())
    {
        if (json[0] == '{')
            return shared_ptr<JSON>(new JSON_OBJECT(json.substr(1, json.size() - 2)));
        else if (json[0] == '[')
            return shared_ptr<JSON>(new JSON_ARRAY(json.substr(1, json.size() - 2)));
        else if (json[0] == '"')
            return shared_ptr<JSON>(new JSON_STRING(json.substr(1, json.size() - 2)));
        else if (json[0] == 't' && json.size() == 4)
            return (shared_ptr<JSON>(new JSON_BOOL(true)));
        else if (json[0] == 'f' && json.size() == 5)
            return (shared_ptr<JSON>(new JSON_BOOL(false)));
        else if (json[0] == 'n' && json.size() == 4)
            return shared_ptr<JSON>(new JSON_NULL());
        else
            return (shared_ptr<JSON>(new JSON_NUMBER(json)));
    }
    else
        return shared_ptr<JSON>(new JSON_NULL());
}


int main()
{
    /*
    //测试代码
    shared_ptr<JSON> json0 = shared_ptr<JSON>(new JSON_NULL());
    shared_ptr<JSON> json1 = shared_ptr<JSON>(new JSON_BOOL(true));
    shared_ptr<JSON> json2 = shared_ptr<JSON>(new JSON_STRING("Hello,JSON"));
    
    ARRAY arr;
    arr.add(json0);
    arr.add(json1);
    arr.add(json2);
    shared_ptr<JSON> json3 = shared_ptr<JSON>(new JSON_ARRAY(arr));
    OBJECT object;
    object.set("my key", json3);
    shared_ptr<JSON> json4 = shared_ptr<JSON>(new JSON_OBJECT(object));
    object.set("second key", shared_ptr<JSON>(json4->copy()));
    json4 = shared_ptr<JSON>(new JSON_OBJECT(object));

    static_cast<JSON_OBJECT*>(&*json4)->get_obj().set("third", shared_ptr<JSON>(new JSON_NULL()));
    string s = json4->json_to_string();
    cout << s << endl;
    //shared_ptr<JSON> json5 = shared_ptr<JSON>(new JSON_ARRAY(s.substr(1, s.size() - 2)));
    shared_ptr<JSON> json5 = shared_ptr<JSON>(new JSON_OBJECT(s.substr(1, s.size() - 2)));
    s = json5->json_to_string();
    cout << s<<endl;
    json5 = shared_ptr<JSON>(new JSON_OBJECT(s.substr(1, s.size() - 2)));
    cout << json5->json_to_string();
    */
    /*
     {
        "basic": {
            "enable": true,
            "ip": "200.200.3.61",
            "port": 389,
            "timeout": 10,
            "basedn": "aaa",
            "fd": -1,
            "maxcnt": 133333333333,
            "dns": ["200.200.0.1", "200.0.0.254"]
        },
        "advance": {
            "dns": [
                {"name":"huanan", "ip": "200.200.0.1"}, 
                {"name":"huabei", "ip": "200.0.0.254"}],
            "portpool": [130,131,132],
            "url": "http://200.200.0.4/main",
            "path": "/etc/sinfors",
            "value": 3.14
        }
    }
    一个测试用例
    
    */
    string input;
    string str;
    while (getline(cin, input))          //循环读入  windows下在标准输入中输入ctrl+z结束循环
        str += input;

    shared_ptr<JSON> json = string_to_json(str);        
    cout << json->json_to_string() << endl;
    static_cast<JSON_OBJECT*>(&*json)->get_obj().del("basic");
    cout << json->json_to_string();

    return 0;
}
