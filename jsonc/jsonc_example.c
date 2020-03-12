#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <json-c/json.h>
#include <json-c/json_tokener.h>

/*
    > install the json-c lib first:
    $ git clone https://github.com/json-c/json-c.git
    > !Note: after install json-c lib, should run cmd 'sudo /sbin/ldconfig' to update the lib cache.
*/

int main(void)
{
    /* making the json string:
    {
        "string": "test",
        "number": 666,
        "big_number": 14294967296,
        "bool": false,
        "object": {
            "a": "test",
             "b": true,
	},
        "array": ["a", "b", "c"]
    }
    */
    json_object *json = json_object_new_object();
    
    json_object_object_add(json, "string", json_object_new_string("test"));
    json_object_object_add(json, "number", json_object_new_int(666));
    json_object_object_add(json, "big_number", json_object_new_int64(14294967296));
    json_object_object_add(json, "bool", json_object_new_boolean(0));
    
    json_object *json2 = json_object_new_object();
    
    json_object_object_add(json2, "a", json_object_new_string("test"));
    json_object_object_add(json2, "b", json_object_new_boolean(1));
    json_object_object_add(json, "object", json2);
    
    json_object *array = json_object_new_array();
    json_object_array_add(array, json_object_new_string("a"));
    json_object_array_add(array, json_object_new_string("b"));
    json_object_array_add(array, json_object_new_string("c"));
    json_object_object_add(json, "array", array);
    
    /* 'json_string' pointer and 'json' object are using the same piece of memory.
     * therefore, you can't release the original json object after you convert it
     * to a string.
     */
    const char *json_string = json_object_to_json_string(json);
    printf("%s\n", json_string);
    
    /* decode the 'json_string', will allocate new memory space. */
    struct json_object *json_obj = json_tokener_parse(json_string);
    
    /* the json string conversion to a json object allocates new memory, so you 
     * can now free the json string's memory.
     */
    json_object_put(json);
    
    struct json_object *string_obj = json_object_object_get(json_obj, "string");
    if (json_object_get_type(string_obj) != json_type_string) {
        printf("wrong json type!\n");
        goto exit;
    }
    printf("string: %s\n", json_object_get_string(string_obj));
    
    struct json_object *number_obj = json_object_object_get(json_obj, "number");
    printf("number: %d\n", json_object_get_int(number_obj));
    
    struct json_object *big_number_obj = json_object_object_get(json_obj, "big_number");
    printf("big_number: %ld\n", json_object_get_int64(big_number_obj));
    
    struct json_object *bool_obj = json_object_object_get(json_obj, "bool");
    printf("bool: %d\n", json_object_get_boolean(bool_obj));
    
    struct json_object *object_obj = json_object_object_get(json_obj, "object");
    struct json_object *a_obj = json_object_object_get(object_obj, "a");
    struct json_object *b_obj = json_object_object_get(object_obj, "b");
    printf("a: %s\n", json_object_get_string(a_obj));
    printf("b: %d\n", json_object_get_boolean(b_obj));
    
    struct json_object *array_obj = json_object_object_get(json_obj, "array");
    
    int length = json_object_array_length(array_obj);
    int index;
    for (index = 0; index < length; index++) {
        json_object *tmp = json_object_array_get_idx(array_obj, index);
        if (json_object_get_type(tmp) != json_type_string) {
            printf("wrong json type in array !\n");
            goto exit;
        }
        printf("array: %s\n", json_object_get_string(tmp));
    }

exit:
    json_object_put(json_obj);
    
    return 0;
}
