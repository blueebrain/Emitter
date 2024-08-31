#include <QCoreApplication>
#include "emitter.h"

class Object{
public://Command
    Command cmd_1;
    Command cmd_2;
    Command cmd_3;
public:
    void TestParams(Params p){
        while(!p.Bool.Empty()){
            qDebug()<<p.Bool.Get();
        }
        while(!p.String.Empty()){
            qDebug()<<p.String.Get();
        }
    }
    void TestVoid(){
        qDebug()<<"void params";
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Object obj;
    Emitter em;
    em.Bind(obj.cmd_1,&obj,&Object::TestParams);
    Emitter::Bind(&em,obj.cmd_2,&obj,&Object::TestVoid,true);
    em.Bind(obj.cmd_3,[](Params p){
        qDebug()<<"lambada-params";
        while(!p.String.Empty()){
            qDebug()<<p.String.Get();
        }

    });
    em.Bind(obj.cmd_3,[](){
        qDebug()<<"lambada";
    });
    Params p;
    p.Bool.Set(true,false,true);
    p.String.Set("str1","str2","strn");

    em.Emit(obj.cmd_1,p);
    em.Emit(obj.cmd_2,p);
    em.Emit(obj.cmd_2,p);
    em.Emit(obj.cmd_3);

    return a.exec();
}
