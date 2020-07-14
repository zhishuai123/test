#include<string>
#include<vector>
#include<unordered_map>
#include<map>
using namespace std;
/**
 *  ���룺
 *  1. ���JSON�ӿ���Ϊʲô������Ƶģ�
 *  2. ��Щ������ʲô�ص㣬����ôӰ���API��Ʒ��
 *  3. ����û���ô����ЩAPI��������
 *  4. ��ô���API�������û���������ˬ��
 *  5. ��ô���API��������ЩAPI�����׵��ԡ����ԣ�
 */
typedef enum json_e {
    JSON_NONE,
    JSON_BOL,           //BOOL����
    JSON_NUM,           //��ֵ����
    JSON_STR,           //�ַ�������
    JSON_ARR,           //��������
    JSON_OBJ,           //��������
} json_e;

typedef unsigned int BOOL;
typedef unsigned int U32;

//typedef struct Value JSON;
//typedef struct KeyValue KeyValue;
//typedef struct Object Object;
//typedef struct Array Array;
class JSON {
    json_e type;
public:
    JSON(json_e type) :type(type) {};
    json_e get_type()const;
    void set_type(json_e type);
    virtual string json_to_string();

    virtual JSON& clone();
    virtual JSON* copy();                           //֮���private ���ñ�¶���ⲿ ָ����ܲ����ͷ�
    virtual ~JSON() {};
};//typedef struct Value JSON;

class OBJECT {
    unordered_map<string, shared_ptr<JSON>> object;
public:
    OBJECT() {}
    OBJECT(unordered_map<string, shared_ptr<JSON>> obj) {
        for (auto i = obj.begin(); i != obj.end(); i++)
            object[i->first] = shared_ptr<JSON>((i->second)->copy());
    }
    OBJECT(const OBJECT& obj){
        for (auto i = obj.object.begin(); i != obj.object.end(); i++)
            object[i->first] = shared_ptr<JSON>((i->second)->copy());
    }
    bool set(string key, shared_ptr<JSON> json);                            //
    shared_ptr<JSON> get(string key);
    bool del(string key);
    OBJECT& operator=(const OBJECT& obj);
    unordered_map<string, shared_ptr<JSON>>& get_object();
    ~OBJECT() {};
};
class ARRAY {
    vector<shared_ptr<JSON>> arry;
public:
    ARRAY(const ARRAY& arr){
        for (auto i = arr.arry.begin(); i != arr.arry.end(); i++)
            arry.push_back(shared_ptr<JSON>((*i)->copy()));
    }
    ARRAY(const vector<shared_ptr<JSON>> arr){
        for (auto i = arr.begin(); i != arr.end(); i++)
            arry.push_back(shared_ptr<JSON>((*i)->copy()));
    }
    ARRAY() {}
    bool set_by_idx(U32 index, shared_ptr<JSON> json);
    bool add(shared_ptr<JSON> json);
    bool del_by_idx(U32 index);
    shared_ptr<JSON> get_by_idx(U32 index);
    ARRAY& operator=(const ARRAY& arr);
    U32 size();
    ~ARRAY() {};
};

class JSON_NUMBER :public JSON {
    double num;
public:
    JSON_NUMBER(double num) :JSON(JSON_NUM), num(num) {}
    JSON_NUMBER(const JSON_NUMBER& jsonNum) :JSON(jsonNum.get_type()), num(jsonNum.num) {}     //��������

    JSON_NUMBER(string str) : JSON(JSON_NUM) { num = stod(str); }                                                                  //�����л�
    string json_to_string();                                                                        //���л�

    double get_num();
    bool set(double num);
    virtual JSON_NUMBER* copy();                                                           //����һ�������JSON�����ָ��
    /*virtual JSON_NUMBER& clone();*/

    virtual ~JSON_NUMBER() {};
};
class JSON_BOOL :public JSON {
    bool boolValue;
public:
    JSON_BOOL(bool boolValue) :JSON(JSON_BOL), boolValue(boolValue) {}
    JSON_BOOL(const JSON_BOOL& jsonBool) :JSON(jsonBool.get_type()), boolValue(jsonBool.boolValue) {}     //��������

    JSON_BOOL(string str);                                                                                //�����л�
    virtual string json_to_string();                                                                           //���л�

    bool get_bol();
    bool set(bool boolValue);
    virtual JSON_BOOL* copy();
    virtual ~JSON_BOOL() {};

};
class JSON_STRING :public JSON {
    string str;
public:
    JSON_STRING(string str) :JSON(JSON_STR), str(str) {}                                        //�����л�  
    JSON_STRING(const JSON_STRING& jsonStr) :JSON(jsonStr.get_type()), str(jsonStr.str) {}     //��������


    virtual string json_to_string();                                                                //���л�

    string get_str();
    bool set(string str);

    virtual JSON_STRING* copy();

    /*virtual JSON_STRING& clone();*/
    virtual ~JSON_STRING() {};

};
class JSON_ARRAY :public JSON {
    ARRAY arr;
public:
    JSON_ARRAY(ARRAY arr) :JSON(JSON_ARR), arr(arr) {}
    JSON_ARRAY(const JSON_ARRAY& jsonArr) :JSON(jsonArr.get_type()), arr(jsonArr.arr) {}     //��������

    JSON_ARRAY(string str);                                                                   //�����л�
    virtual string json_to_string();                                                          //���л�

    ARRAY& get_arr();                                                                   //���������� ʹ��ҪС��
    bool set(const ARRAY& arr);

    virtual JSON_ARRAY* copy();
    /* virtual JSON_ARRAY& clone();*/
    virtual ~JSON_ARRAY() {};
};
class JSON_OBJECT :public JSON {
    OBJECT obj;
public:
    JSON_OBJECT(OBJECT obj) :JSON(JSON_OBJ), obj(obj) {}
    JSON_OBJECT(const JSON_OBJECT& jsonObj) :JSON(jsonObj.get_type()), obj(jsonObj.obj) {}     //��������

    JSON_OBJECT(string str);                                                                //�����л�
    virtual string json_to_string();                                                          //���л�

    OBJECT& get_obj();
    bool set(const OBJECT& obj);

    virtual JSON_OBJECT* copy();
    /*virtual JSON_OBJECT& clone();*/
    virtual ~JSON_OBJECT() {};
};
class JSON_NULL :public JSON {
public:
    JSON_NULL() :JSON(JSON_NONE) {}
    JSON_NULL(string str);                                                                //�����л�
    virtual string json_to_string();//���л�

    virtual JSON_NULL* copy();
    /*virtual JSON_NULL& clone();    */
    virtual ~JSON_NULL() {};
};

