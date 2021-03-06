### 主要特点
- 线程没有栈，只需一个变量来保存线程退出的代码行数
- 线程退出不会保存局部变量
- 有阻塞条件，信号量等线程通信组件
### 如何使用
- 包含pt.h文件，如果要使用信号量，包含pt_sem.h文件（可能还有其他组件，暂时没去研究）
- 线程体必须使用PT_BEGIN(pt)和PT_END(pt)包裹起来，线程运行前，必须先调用PT_INIT(&pt)初始化线程
- 在main函数的大循环中调用各个线程函数，必须保证每个线程函数都有执行的机会
- 由于原始线程的实现使用了switch语句，所以在线程体中不能再使用switch语句
- 更多使用例子在 pt_test.c 文件中
### 实现原理
- 每个线程仅仅用了一个变量s来保存自己的返回地址，即代码行数，它目前为2个字节，如果能保证线程所在的代码行数不超过256也可以用一个字节
- PT_INIT(&pt)只是设置变量s为0，当首次进入线程时，switch语句就执行到case 0，这时线程是从头开始执行的，但是只要调用了阻塞、调度相关的函数，这些函数就会将判断条件代码之前的行数保存到s（s = LINE，其实不一定是代码行数，任何非零值都可以），然后植入case到这个行数的（case LINE:）语句，最后调用return结束运行，当再次进入这个线程时，会传入刚刚保存的变量s的值，这样switch语句就直接case到了条件判断之前，而不会再次从头开始执行，如果有阻塞条件，且条件为假，则直接返回；如果条件为真，则继续执行后面的代码，直到执行PT_END(pt)，此宏再次将变量s设置为0，也就是说下次进入这个线程又会从头开始执行。
