#include <iostream>
#include "vector.h"
using namespace std;
#include <cassert>

/*-------默认函数---------*/
void test_default_constructor() 
{
    sjl::vector<int> v;
    assert(v.size() == 0);
    assert(v.capacity() == 0);
    assert(v.empty());
}
/*------填充构造函数------*/
void test_fill_constructor() 
{
    sjl::vector<int> v(5, 10);
    assert(v.size() == 5);
    for (size_t i = 0; i < 5; ++i) {
        assert(v[i] == 10);
    }
}
/*------范围构造函数-------*/
void test_range_constructor() 
{
    int arr[] = { 1, 2, 3 };
    sjl::vector<int> v(arr, arr + 3);
    assert(v.size() == 3);
    assert(v[0] == 1 && v[1] == 2 && v[2] == 3); 
}
/*-----初始化列表构造函数------*/
void test_initializer_list_constructor() 
{
    sjl::vector<int> v{ 4, 5, 6 };
    assert(v.size() == 3);
    assert(v[0] == 4 && v[1] == 5 && v[2] == 6);
}


/*-------拷贝赋值---------*/
void test_copy_assignment() 
{
    sjl::vector<int> v1{ 1, 2, 3 };
    sjl::vector<int> v2;
    v2 = v1;
    assert(v2.size() == 3);
    assert(v2[1] == 2);
    v1[0] = 5;
    assert(v2[0] == 1); 
}
/*-----------移动赋值--------*/
void test_move_assignment() 
{
    sjl::vector<int> v1{ 1, 2, 3 };
    sjl::vector<int> v2;
    v2 = std::move(v1);
    assert(v1.empty() && v2.size() == 3); 
}
/*---front back*/
void test_front_back() 
{
    sjl::vector<std::string> v{ "a", "b", "c" };
    assert(v.front() == "a");
    assert(v.back() == "c");
    v.back() = "z";
    assert(v[2] == "z");
}

/*----迭代器-------*/
void test_iterators() 
{
    sjl::vector<int> v{ 1, 2, 3 };
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }
    assert(sum == 6); 
}

/*-----reserve------*/
void test_reserve() 
{
    sjl::vector<int> v;
    v.reserve(10);
    assert(v.capacity() >= 10);
    v.push_back(1);
    v.reserve(5); 
    assert(v.capacity() >= 10);
}
/*----resize----*/
void test_resize() 
{
    sjl::vector<int> v{ 1, 2 };
    v.resize(5, 10); // 扩展
    assert(v.size() == 5 && v[4] == 10);
    v.resize(3); // 收缩
    assert(v.size() == 3 && v[2] == 10); 
}
/*----push_back 扩容-----*/
void test_push_back() 
{
    sjl::vector<int> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }
    assert(v.size() == 10 && v[9] == 9); 
}
/*------emplace 插入-----*/
void test_emplace() 
{
    sjl::vector<std::pair<int, int>> v;
    v.emplace(v.end(), 1, 2);
    assert(v[0].first == 1 && v[0].second == 2);
}
/*------insert------*/
void test_insert() {
    sjl::vector<int> v{ 1, 4 };
    v.insert(v.begin() + 1, 2, 5);
    assert(v.size() == 4 && v[1] == 5 && v[2] == 5); 
}
/*------clear------*/
void test_clear() {
    sjl::vector<int> v{ 1, 2, 3 };
    v.clear();
    assert(v.empty() && v.capacity() == 0); 
}
int main() {
   
    test_default_constructor();
    test_fill_constructor();
    test_range_constructor();
    test_initializer_list_constructor();



    test_copy_assignment();
    test_move_assignment();
    test_iterators();
    test_reserve();
    test_front_back();
    test_resize();
    test_push_back();
    test_emplace();
    test_insert();
    test_clear();
    return 0;
}