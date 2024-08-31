#ifndef PARAMS_H
#define PARAMS_H
#include <queue>
#include <string>

template<class T>
class ParamsT{
    std::queue<T> que;
public:
    bool Empty(){return que.empty();}

    T Get(){
        if(!que.empty()){
            T value = que.front();
            que.pop();
            return value;
        }
        return T();
    }

    void Set(){
        return;
    }

    void Set(T value){ que.emplace(value);}

    template<class ...Args>
    void Set(T value,Args ...args){
        que.emplace(value);
        Set(args...);
    }
};

//新增参数类型可继承Params或者自定;
class Params
{
public:
    ParamsT<bool> Bool;
    ParamsT<long> Long;
    ParamsT<double> Double;
    ParamsT<std::string> String;
    ParamsT<void*> Data;
};

#endif // PARAMS_H
