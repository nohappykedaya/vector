
# sjl::vector - C++ STL风格动态数组容器

一个高性能的STL风格动态数组模板容器实现，支持标准vector的核心功能，包含完整的异常安全保证和内存管理机制。

## 🚀 功能特性

### 核心功能
- **多种构造方式**  
  支持默认构造、填充构造、范围构造、拷贝构造及移动语义
  ```cpp
  sjl::vector<int> v1;                // 默认构造
  sjl::vector<int> v2(5, 10);         // 填充5个10
  sjl::vector<int> v3(v2.begin(), v2.end()); // 范围构造
  ```

- **高效内存管理**  
  使用`operator new/delete`直接内存操作，支持：
  - 自动扩容策略（2倍增长）
  - 预留内存空间（`reserve()`）
  - 内存复用（`shrink_to_fit()`）

- **完整元素访问**  
  ```cpp
  v.front();     // 首元素
  v.back();      // 尾元素
  v[0];          // 下标访问
  v.data();      // 原始指针访问
  ```

- **迭代器支持**  
  原生指针实现的迭代器，兼容STL算法
  ```cpp
  for (auto it = v.begin(); it != v.end(); ++it) { /*...*/ }
  std::sort(v.begin(), v.end());
  ```

### 高级特性
- **异常安全保证**  
  - 基本异常安全（Basic Exception Safety）  
  - 强异常安全（Strong Exception Safety）关键操作

- **完美转发支持**  
  使用`emplace()`实现原位构造，避免临时对象拷贝
  ```cpp
  v.emplace(v.begin(), 1, 2); // 直接在首部构造元素
  ```

- **移动语义优化**  
  移动构造/赋值实现零拷贝资源转移
  ```cpp
  sjl::vector<int> v4 = std::move(v3); // O(1)时间复杂度
  ```

## 📦 快速开始

### 依赖要求
- C++17或更高标准(建议C17 虽然我连c11都不熟 但是里面用到了一个 std::uninitialized_copy 不然运行不了)
- 支持C++模板的编译器（测试环境：MSVC 2022  / GCC 11+）

### 使用方法
直接克隆或者vs打开

2. **基本使用示例**
   ```cpp
   #include <iostream>
   #include "vector.h"

   int main() {
       sjl::vector<std::string> names;
       
       // 添加元素
       names.push_back("Alice");
       names.emplace_back("Bob");  // 原位构造
       
       // 容量操作
       names.reserve(100);
       std::cout << "Capacity: " << names.capacity() << std::endl;
       
       // 遍历输出
       for (const auto& name : names) {
           std::cout << name << std::endl;
       }
       return 0;
   }
   ```


核心方法
| 方法                  | 说明                          | 时间复杂度     |
|-----------------------|-----------------------------|-------------|
| `resize(n)`           | 调整元素数量                    | O(n)        |
| `emplace(pos, args...)`| 原位构造元素                    | O(n)        |
| `insert(pos, val)`    | 插入元素                      | O(n)        |
| `erase(pos)`          | 删除元素                      | O(n)        |

容量操作
| 方法                | 说明                          |
|--------------------|-----------------------------|
| `shrink_to_fit()`  | 压缩内存至刚好容纳当前元素           |
| `reserve(n)`       | 预留至少容纳n个元素的内存空间         |








加油加油 一步一步走
