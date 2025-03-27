#pragma once
#pragma once
#include <cstddef>
#include <iostream>
#include <memory>
#include <algorithm>
#include<utility>
#include <initializer_list>

namespace sjl
{
	template<class T>
	class vector {
	public:
		using value_type = T; //容器中存储的元素的类型  
		using size_type = std::size_t; // 容器大小或者索引的无符号整数类型
		using difference_type = std::ptrdiff_t; //两个迭代器之间距离的有符号整数类型
		using reference = value_type&; // 提供对元素的直接访问 允许修改元素
		using const_reference = const value_type&; // 只读取元素
		using iterator = value_type*;//指向容器元素的非常量指针 支持读写元素
		using const_iterator = const value_type*; // 仅支持读操作

	private:
		T* m_start; //第一个元素的指针
		T* m_finish; // 最后一个元素的下一个位置
		T* m_end_of_storage;//内存快末尾
		//内存分配
		T* allocate(size_type n) //申请能容纳n个T类型对象的内存
		{
			return static_cast<T*> (operator new(n * sizeof(value_type)));
			//static_cast 类型转换 对象到T的转换
		}
		//内存释放
		void  deallocate(T* p)
		{
			operator delete(p);
		}

	public:

		vector() noexcept//默认构造函数 创建空的vector 
			:m_start(nullptr), m_finish(nullptr), m_end_of_storage(nullptr) {}
		//通过委托构造调用范围构造函数 复制 init中的元素
		vector(std::initializer_list<T> init):vector(init.begin(),init.end()){}

		//填充构造函数
		explicit vector(size_type n, const T& val = T())
		{
			m_start = allocate(n);
			m_finish = m_end_of_storage = m_start + n;
			std::uninitialized_fill_n(m_start, n, val);//构造n个val的副本
		}
		//范围构造函数  防止整数类型误匹配     类型过滤   
		template <class InputIt, typename = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
		vector(InputIt first, InputIt last)
		{
			size_type n = std::distance(first, last);
			m_start = allocate(n);
			try {
				std::uninitialized_copy(first, last, m_start);
				m_finish = m_end_of_storage = m_start + n;
			}
			catch (...) {
				deallocate(m_start);
				throw;
			}

		}
		//拷贝构造函数
		vector(const vector& other) : vector(other.begin(), other.end()) {}
		//移动构造函数
		vector(vector&& other) noexcept //：高效转移资源所有权  右值引用避免深拷贝
		{
			m_start = other.m_start;
			m_finish = other.m_finish;
			m_end_of_storage = other.m_end_of_storage;
			other.m_start = other.m_finish = other.m_end_of_storage = nullptr;
		}
		~vector()
		{
			clear();
			deallocate(m_start);
		}

		void swap(vector& other) noexcept
		{
			std::swap(m_start, other.m_start);
			std::swap(m_finish, other.m_finish);
			std::swap(m_end_of_storage, other.m_end_of_storage);
		}
		void reserve(size_type n) {
			if (capacity() >= n) return;

			// 计算旧元素数量
			const size_type old_size = size();

			// 分配新内存
			T* new_start = allocate(n);
			T* new_finish = new_start;

			// 移动旧元素到新内存（C++17兼容实现）
			new_finish = std::uninitialized_move(begin(), end(), new_start);
			//将对象从就内存移动到新内存 避免拷贝开销  
			// 清理旧内存（先析构后释放）
			std::destroy(begin(), end());
			deallocate(m_start);

			// 更新指针
			m_start = new_start;
			m_finish = new_start + old_size;
			m_end_of_storage = new_start + n;
		}
		vector& operator =(const vector& other) //拷贝赋值运算符
		{
			if (this != &other) {
				vector tmp(other); //创建临时拷贝 创建临时底线tmp深拷贝other 然后交换当前对象和tmp的资源
				swap(tmp); //交换当前对象与临时对象   临时对象在作用域结束时析构 自动释放
			}
			return *this;
		}
		vector& operator=(vector&& other) noexcept
		{
			if (this != &other) {
				clear();
				deallocate(m_start);
				//转移资源   直接窃取右值引用 other的资源
				m_start = other.m_start;
				m_finish = other.m_finish;
				m_end_of_storage = other.m_end_of_storage;
				other.m_start = other.m_finish = other.m_end_of_storage = nullptr;
			}
			return *this;
		}

		// ---- 元素访问
		reference operator[](size_type n) noexcept //返回索引n处元素的引用 允许修改
		{
			return m_start[n];
		}
		const_reference operator[](size_type n) const noexcept
		{
			return m_start[n];
		}
		reference front()  noexcept //返回首元素的引用    快速访问首尾元素
		{
			return *m_start;
		}
		const_reference front() const  noexcept
		{
			return *m_start;
		}
		reference back() noexcept
		{
			return *(m_finish - 1);
		}
		const_reference back() const noexcept
		{
			return *(m_finish - 1);
		}
		T* data() noexcept
		{
			return m_start;
		}
		const T* data() const noexcept
		{
			return m_start; //返回首元素的指针
		}

		//迭代器
		iterator  begin() noexcept
		{
			return m_start;
		}
		const_iterator begin() const noexcept
		{
			return m_start;
		}
		iterator end() noexcept
		{
			return m_finish;
		}
		const_iterator end() const noexcept
		{
			return m_finish;
		}

		//容量
		bool empty() const noexcept
		{
			return m_start == m_finish;
		}
		size_type size()const noexcept
		{
			return m_finish - m_start;
		}
		size_type capacity()const noexcept
		{
			return m_end_of_storage - m_start;
		}

		//修改器
		void clear() noexcept
		{
			//析构所有元素
			for (iterator it = begin(); it != end(); ++it) {
				it->~T();
			}
			deallocate(m_start);
			m_finish = m_start = m_end_of_storage = nullptr;
		}

		template <typename... Args> // 允许接受任意数量 任意类型的参数 用于直接构造容器中的元素
		iterator emplace(iterator pos, Args&&... args) {
			// 记录插入位置偏移量
			const size_type offset = pos - begin();

			// 检查是否需要扩容
			if (size() >= capacity()) {
				reserve(capacity() ? capacity() * 2 : 1);
			}

			// 更新插入位置（扩容可能改变内存）
			pos = begin() + offset;

			// 移动后续元素腾出空间
			if (pos != end()) {
				std::move_backward(pos, end(), end() + 1);
			}

			// 构造新元素
			::new (pos) T(std::forward<Args>(args)...);
			++m_finish;
			return pos;
		}

		// 批量插入（安全版）
		iterator insert(iterator pos, size_type n, const T& val) {
			if (n == 0) return pos;

			// 记录原始偏移量
			const size_type offset = pos - begin();

			// 检查容量
			if (size() + n > capacity()) {
				reserve(size() + n);  // 简单扩容策略
			}

			// 更新插入位置
			pos = begin() + offset;

			// 移动现有元素
			std::move_backward(pos, end(), end() + n);

			// 填充新元素
			std::fill_n(pos, n, val);
			m_finish += n;
			return pos;
		}
		iterator erase(iterator pos)
		{
			return erase(pos, pos + 1);
		}
		iterator erase(iterator first, iterator last)
		{
			if (first != last) {
				iterator new_end = std::move(last, end(), first);
				while (m_finish != new_end) {
					--m_finish;
					m_finish->~T();
				}
			}
			return first;
		}
		void resize(size_type n) {
			if (n > size()) {
				// 使用默认值填充新增元素
				resize(n, T());
			}
			else if (n < size()) {
				// 直接截断，保留前 n 个元素
				erase(begin() + n, end());
			}
		}
		void resize(size_type n, const value_type& val)
		{
			if (n > size()) {
				insert(end(), n - size(), val);
			}
			else if (n < size()) {
				erase(begin() + n, end());
			}
		}
		void push_back(const T& val) { emplace(end(), val); }
		void push_back(T&& val) {
			emplace(end(), std::move(val));
		}
		void pop_back()
		{
			if (!empty()) {
				--m_finish;
				m_finish->~T();
			}
		}

		void show() const //打印当前vector内容
		{
			for (const auto& val : *this) std::cout << val << " ";
			std::cout << "\nsize=" << size() << ", capacity=" << capacity() << "\n";
		}
	};
}
