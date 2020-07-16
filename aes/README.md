### 主要特点
- 通过串口提供了一个简单的命令行实现
- 使用这个命令行模块仅仅需要调用一个接口函数
- 支持调整命令长度，参数个数等，这些值会影响占用的内存大小
- 支持执行历史命令，可调整存储历史命令的个数
### 如何使用？
- 包含 cli.h 头文件
- 在串口每次收到一个字符时，仅仅需要将接受到的字符传给如下函数即可
``` c
void cmd_line(char c)；
```
- 最后定义打印函数，下面是一个定义的例子
``` c
#define cli_printf(format, ...) \
do { \
    printf(""format"", ##__VA_ARGS__); \
} while(0)
```
### 添加新命令
- 你可以将命令实现在 cmds.c 中，也推荐这么做，但是也可以实现在任何地方，但是要让 cli.c 能够引用它
- 然后将新命令注册在 cli.c 文件中的命令列表中
``` c
static const cmd_list_t cmd_list[] = {
        { .name = "?",      .handle = help,         .help = "<no param> print the help info." },
        { .name = "c",      .handle = clear_window, .help = "<no param> clear the command window." },
        ......
};
```
