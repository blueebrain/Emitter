### Emitter
#### 介绍
Emitter 类似QT信号槽，可以绑定函数到Command信号上，通过Emit对应的Command,触发函数回调。为了传递任意参数，定义了Params类；由于没有考虑线程安全和效率，使用时需要多注意。
### class Params
1. ParamsT 使用模板和std::queue对基础类型进行封装;
2. Set 是一个模板函数，可以同时设置一个或多个同一类型的变量：例如ParamsT::Set(true), ParamsT::Set(true,false,true);
3. Get 也是模板函数，获取一个存在的值，获取值的同时会从队列中将其移除;
4. Empty 返回ParamsT是否包含参数。如果为空，调用Get或获取"空值（0,NULL,nullptr,等）"。因此调用Get前必须调用该函数;
5. Params 是ParamsT的集合，提供了基础的Bool(bool),Long(long),Double(double),String(std::string)，VoidPtr(void*),基本囊括了所有参数类型;
6. 也可以将Params修改为Params*,以便通过继承来拓展新类型。
### class Command
1. 定义了一个static变量count(0)，当Command创建时，count加1。因此Command(unsigned long)最小值为1;
2. 不使用枚举的原因，是模仿Qt信号的倔强，可能是一个灾难性的决定。
### class Emitter
1. Emitter提供基本的Bind,UnBind,Emit的功能，支持回调函数单次触发(std::queue存储)和多发(std::map存储);
2. Bind 基础参数列表为: (参数1 subject,参数2 Command,参数3 observer,参数4 callback,参数5 单发/多发);返回值为绑定函数的ID，用于解除绑定。
3. Bind 支持静态函数绑定成员函数、普通函数、lambada表达式等; 同时也支持Params参数和无参模式;
4. Bind 也支持Emitter成员函数调用的形式进行绑定;
5. UnBind 支持静态调用和Emitter变量调用，UnBind仅支持移除多发，单发回调执行后自动移除;
6. Emit 接受Command,Params参数，触发信号优先调用绑定的单发回调，再执行多发回调。
