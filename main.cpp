#include <iostream>

//int main() {
//    std::cout << "Hello, World!" << std::endl;
//    return 0;
//}

#include <cstdio>
#include <string>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using namespace std;
using namespace rocksdb;

//数据库存储的路径
std::string kDBPath = "../rocksdb_simple_example";

int main() {
    DB *db;
    Options options;

    //增加并发和水平压实风格
    options.IncreaseParallelism();

    options.create_if_missing = true;

    //open DB
    Status s = DB::Open(options, kDBPath, &db);
    assert(s.ok());

    //Put key-value
    s = db->Put(WriteOptions(), "key1", "value");
    assert(s.ok());

    std::string value;
    //get value
    s = db->Get(ReadOptions(), "key1", &value);
    assert(s.ok());
    assert(value == "value");

    //将一组更新原子化
    {
        WriteBatch batch;
        batch.Delete("key1");
        batch.Put("key2", value);
        s = db->Write(WriteOptions(), &batch);
    }

    s = db->Get(ReadOptions(), "key1", &value);
    assert(s.IsNotFound());

    db->Get(ReadOptions(), "key2", &value);
    assert(value == "value");

    {
        PinnableSlice pinnable_val;
        db->Get(ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
        assert(pinnable_val == "value");
    }

    {
        std::string string_val;
        //如果无法锁定值，则将该值复制到其内部缓冲区
        //内部缓冲区可以在构造时设置
        PinnableSlice pinnable_val(&string_val);
        db->Get(ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
        assert(pinnable_val == "value");
        //如果值没被固定，则一定在内部缓冲区里
        assert(pinnable_val.IsPinned() || string_val == "value");
    }


    PinnableSlice pinnable_val;
    db->Get(ReadOptions(), db->DefaultColumnFamily(), "key1", &pinnable_val);
    assert(s.IsNotFound());
    //每次使用之后和每次重新使用之前重置PinnableSlice
    pinnable_val.Reset();
    db->Get(ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
    assert(pinnable_val == "value");
    pinnable_val.Reset();
    //在这之后，pinnable_val指向的Slice无效。

    cout <<"value:" << value << endl;
    delete db;

    return 0;
}