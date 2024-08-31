#ifndef EMITTER_H
#define EMITTER_H
#include "params.h"
#include <functional>
#include <map>
//模板必须声明和定义在头文件
//否则会找不到定义


//用于绑定和触发回调
class Command{
    using IdType = unsigned long;
private:
    static IdType count;
    IdType value{0};
public:
    IdType operator () (){
        return this->value;
    }
    bool operator == (const Command& other){
        return (this->value == other.value);
    }
    bool operator < (const Command& other) const{
        return (this->value < other.value);
    }
    Command(){
        value=++count;
    }
};

/*
Emitter用于绑定回调函数;
当使用Emit激发command时，执行对应的回调函数
由于Emit通过Params类型传参,因此回调函数应仅有一个Params参数的成员函数;
*/
class Emitter
{
private:
    using IdType = unsigned long;
    using Func = std::function<void(Params)>;
    using FuncMap  = std::map<IdType,Func>;
    using FuncQueue  = std::queue<Func>;
    IdType funcId{0};
    std::map<Command,FuncMap> multiShot;
    std::map<Command,FuncQueue> singleShot;
private:
    unsigned long Push(Command command,Func func){
        auto it = this->multiShot.find(command);
        ++funcId;
        if(it==this->multiShot.end()){
            this->multiShot.emplace(command,FuncMap{{funcId,func}});
        }
        else{
            it->second.emplace(funcId,func);
        }
        return funcId;
    }
    unsigned long Enqueue(Command command,Func func){
        auto it = this->singleShot.find(command);
        if(it==this->singleShot.end()){
            FuncQueue que;
            que.emplace(func);
            this->singleShot.emplace(command,que);
        }
        else{
            it->second.emplace(func);
        }
        return 0;
    }
public:
    //绑定成员函数
    template<class T>
    static unsigned long Bind(Emitter* subject,Command command,T* observer,void(T::*callback)(Params),bool do_once=false){
        if(subject==nullptr||observer==nullptr) return 0;
        Func func = [=](Params params){
            (observer->*callback)(params);
        };
        if(do_once){
            return subject->Enqueue(command,func);
        }
        else{
            return subject->Push(command,func);
        }
    }

    //绑定无参成员函数
    template<class T>
    static unsigned long Bind(Emitter* subject,Command command,T* observer,void(T::*callback)(),bool do_once=false){
        if(subject==nullptr||observer==nullptr) return 0;
        Func func = [=](Params params){
            (observer->*callback)();
        };
        if(do_once){
            return subject->Enqueue(command,func);
        }
        else{
            return subject->Push(command,func);
        }
    }

    //绑定一般函数
    static unsigned long Bind(Emitter* subject,Command command,void(*callback)(Params),bool do_once=false){
        if(subject==nullptr) return 0;
        Func func = [=](Params params){
            callback(params);
        };
        if(do_once){
            return subject->Enqueue(command,func);
        }
        else{
            return subject->Push(command,func);
        }
    }

    //绑定无参函数
    static unsigned long Bind(Emitter* subject,Command command,void(*callback)(),bool do_once=false){
        if(subject==nullptr) return 0;
        Func func = [=](Params params){
            callback();
        };
        if(do_once){
            return subject->Enqueue(command,func);
        }
        else{
            return subject->Push(command,func);
        }
    }

    //绑定成员函数
    template<class T>
    unsigned long Bind(Command command,T* observer,void(T::*callback)(Params),bool do_once=false){
        return Emitter::Bind(this,command,observer,callback,do_once);
    }

    //绑定无参成员函数
    template<class T>
    unsigned long Bind(Command command,T* observer,void(T::*callback)(),bool do_once=false){
        return Emitter::Bind(this,command,observer,callback,do_once);
    }

    //绑定一般函数
    unsigned long Bind(Command command,void(*callback)(Params),bool do_once=false){
        return Emitter::Bind(this,command,callback,do_once);
    }

    //绑定无参函数
    unsigned long Bind(Command command,void(*callback)(),bool do_once=false){
        return Emitter::Bind(this,command,callback,do_once);
    }

    //触发回调
    static void Emit(Emitter* subject,Command command,Params params = Params()){
        auto itor = subject->singleShot.find(command);
        if(itor!=subject->singleShot.end()){
            while(!itor->second.empty()){
                Func func = itor->second.front();
                func(params);
                itor->second.pop();
            }
            subject->singleShot.erase(itor);
        }
        auto it = subject->multiShot.find(command);
        if(it!=subject->multiShot.end()){
            for(auto& fp:it->second){
                if(fp.second) fp.second(params);
            }
        }
    }

    //触发回调
    void Emit(Command command,Params params= Params()){
        Emitter::Emit(this,command,params);
    }

    //解除command的对应绑定
    static void UnBind(Emitter* subject,Command command,IdType id){
        auto it = subject->multiShot.find(command);
        if(it != subject->multiShot.end()){
            auto itor = it->second.find(id);
            if(itor!=it->second.end()){
                it->second.erase(itor);
                if(it->second.empty()){
                    subject->multiShot.erase(it);
                }
                return;
            }
        }
    }

    //解除command的对应绑定
    void UnBind(Command command,IdType id){
        Emitter::UnBind(this,command,id);
    }

    //解绑对应command
    static void UnBind(Emitter* subject,Command command){
        auto it = subject->multiShot.find(command);
        if(it != subject->multiShot.end()){
            subject->multiShot.erase(it);
        }
    }

    //解绑对应command
    void UnBind(Command command){
        Emitter::UnBind(this,command);
    }
};

#endif // EMITTER_H
