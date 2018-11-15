# useful-code-modules
The collection of useful code modules.

##时钟：clk.c、clk.h
功能：秒脉冲驱动日历时钟。可以选择日历时钟的格式，可以使用时间戳。
使用：所有时钟用到的变量常量都在clk.c文件中，外面只能通过clk.h中声明的函数接口来访问。使用时，应先调用clk_init()函数初始化各时钟日期变量，接着每一秒种调用一次clk_update()函数，然后通过提供的接口访问各时钟日期变量。
##命令行：cli.c、cli.h、cmds.c、cmds.h
功能：通过串口提供一个简单的命令行。改变MAX_HIS_CMD、ARG_MAX_LEN和ARG_MAX_NUM的值可调整内存的用量。
使用：只需配置两个接口，第一，在cli.h中定义cli_printf()函数，定义的函数要实现printf()一样的功能。第二，串口每收到一个字符都要向cmd_line(char)函数发送这个字符，模块在识别到回车键后，会进行命令解析并且执行命令，然后返回。可在cmds.c文件中添加新命令，然后必须在cmds.h文件中声明。然后在cli.c文件中，找到cmd_list数组，向其中添加新命令。
##软件定时器：timer.c、timer.h
功能：提供软件定时器功能，结构功能简单，适用于定时器需求数量较小的系统（几个以内），具体占用内存大小可根据定义的定时器结构体算出，可以更改结构体中的tick变量类型，从而获取更长的定时时间（也意味着占用更多的内存空间）。当增加定时器时，定时器负荷会线性增加，这是因为每一次tick到来都会查询所有定时器的tick数是否为0，如果为0，则调用他们的回调函数。定时器的回调函数应该尽可能的短，最好小于一个tick的时间，否则会丢失tick，造成定时不精确。如果要求定时器很精确，可以考虑在ISR中调用回调函数，这样相当于立即执行回调函数，但是此时回调函数的执行时间一定要短。
使用：TMR_MAX_TMR_NUM（在timer.h文件中）定义了最大可以使用的定时器数量，最多256个（定时器ID编号为0~255）。调用函数timer_create()创建各定时器之前应先调用timer_init()函数初始化定时器列表。每发生一次tick时调用函数timer_tick()，它会通知timer_task()函数一个tick时间到。如果没有操作系统，timer_task()函数可在大循环中调用，它会不断地检查一个tick通知是否到来，如果有操作系统，timer_task()函数就在任务中调用，可用信号量等方式通知它一个tick到来。
改进（未实现）：如果定时器的回调函数的执行时间超过了一个tick，那么这个tick将会丢失，这会造成定时不准确，为了克服这一点，可以考虑记录到来的tick数，而不是记录一个tick，这样每次timer_task()函数不是减去一个tick而是实际的tick数。从而提高定时精度，但是这种方法只适用于中断产生的tick，在大循环中产生的tick不会有任何改进。如果定时器的剩余定时tick数小于了到来的tick数，那么也会丢失相应的tick数，所以不能保证定时的绝对精确，但是这种方法还是有效的改进了定时精度。
##软件Delta定时器：timer_delta.c、timer_delta.h
功能：和普通定时器模块不同，Delta定时器存储的不是实际的tick数，而是各个定时器tick数的差值，这样每个tick到来就只需检查第一个定时器的tick是否用完，不用每个tick都去把所有定时器的tick减一，当想要实现比较多的定时器时，用此方法效率高，一个小缺点是，由于使用了链表，所以占用的内存要比普通定时器多些。
使用：可以使用静态数组或者动态内存分配为每个定时器创建存储空间，其他使用方式和上面的普通定时器一样。
定时器基本特点：不管是普通定时器还是delta定时器，当定时器数量变多时，定时精度都有可能下降，在这方面delta定时器的表现要好得多。定时器的回调函数不应该执行过长时间，如果回调函数的执行时间超过了一个tick，那么这个tick就会丢失，从而影响定时器精度，所以建议回调函数的执行时间不要超过一个tick的时间。当各个定时器的定时时间一样时，系统负荷最重，因为当定时时间到时，需要在同一时间执行完所有定时器的回调函数，所以应该尽量避免定时器的定时时间一样。
##原始线程：pt.h、 pt_test.c、pt_test.h
功能：提供原始线程功能，线程可以阻塞在一个事件上，直到事件发生再继续运行。
使用：线程体必须使用PT_BEGIN(pt)和PT_END(pt)包裹起来，具体用法见pt_test.c文件。功能的实现全部都在pt.h头文件中，另外两个文件只是提供使用范例。实际使用中，只需要包含pt.h文件即可。运行线程前，应先调用pt_init()函数初始化各个线程，然后调用线程函数，一般在main函数的大循环中调用，必须保证每个线程函数都有执行的机会。由于原始线程是通过执行return语句实现的阻塞返回，而且没有自己的堆栈，局部变量都会丢失，所以在线程中不应该期望有任何局部变量保持不变。同时由于原始线程的实现使用了switch语句，所以在线程体中不能再使用switch语句。
实现原理：每个线程仅仅用了一个变量s来保存自己的返回地址：代码行数（目前为2个字节，如果能保证线程所在的代码行数不超过256行也可以用一个字节）。线程没有自己的堆栈，所以占用内存相当少。pt_init()函数只是设置变量s为0，当首次进入线程时，switch语句就执行case 0，这时线程是从头开始执行的，但是只要调用了阻塞、调度相关的函数，这些函数就会将判断条件代码之前的行数保存到s（s = __LINE__，其实不一定是代码行数，任何非零值都可以），然后植入case到这个行数的（case __LINE__:）语句，最后调用return结束运行，当再次进入这个线程时，会传入刚刚保存的变量s的值，这样switch语句就直接case到了条件判断之前，而不会再次从头开始执行，如果判断条件为假，则直接返回，不会执行后面的函数。如果判断条件为真，则继续执行后面的函数，直到执行PT_END(pt)，此宏定义再次将变量s设置为0，也就是说下次进入这个线程又会从头开始执行。
CRC
