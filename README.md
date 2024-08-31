# Emitter
Emitter 类似QT信号槽，可以绑定函数到Command信号上，通过Emit对应的Command,触发函数回调。为了传递任意参数，定义了Params类；由于没有考虑线程安全和效率，使用时需要多注意。
