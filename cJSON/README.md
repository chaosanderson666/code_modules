### 内存问题
- cJSON_Parse 函数会分配内存，使用后需要释放
- cJSON_PrintUnformatted 与 cJSON_Print() 函数会分配内存，使用后需要释放
- cJSON_Delete 释放 cJSON 结构体占用的内存，cJSON_free 用户释放 cJSON_Print 占用的内存
### 其他
- 更多的介绍在官方 README 中，使用例子在 test_cjson.c 中
- $ gcc -o test cJSON.c test_cjson.c -lm
