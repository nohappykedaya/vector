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
		using value_type = T; //�����д洢��Ԫ�ص�����  
		using size_type = std::size_t; // ������С�����������޷�����������
		using difference_type = std::ptrdiff_t; //����������֮�������з�����������
		using reference = value_type&; // �ṩ��Ԫ�ص�ֱ�ӷ��� �����޸�Ԫ��
		using const_reference = const value_type&; // ֻ��ȡԪ��
		using iterator = value_type*;//ָ������Ԫ�صķǳ���ָ�� ֧�ֶ�дԪ��
		using const_iterator = const value_type*; // ��֧�ֶ�����

	private:
		T* m_start; //��һ��Ԫ�ص�ָ��
		T* m_finish; // ���һ��Ԫ�ص���һ��λ��
		T* m_end_of_storage;//�ڴ��ĩβ
		//�ڴ����
		T* allocate(size_type n) //����������n��T���Ͷ�����ڴ�
		{
			return static_cast<T*> (operator new(n * sizeof(value_type)));
			//static_cast ����ת�� ����T��ת��
		}
		//�ڴ��ͷ�
		void  deallocate(T* p)
		{
			operator delete(p);
		}

	public:

		vector() noexcept//Ĭ�Ϲ��캯�� �����յ�vector 
			:m_start(nullptr), m_finish(nullptr), m_end_of_storage(nullptr) {}
		//ͨ��ί�й�����÷�Χ���캯�� ���� init�е�Ԫ��
		vector(std::initializer_list<T> init):vector(init.begin(),init.end()){}

		//��乹�캯��
		explicit vector(size_type n, const T& val = T())
		{
			m_start = allocate(n);
			m_finish = m_end_of_storage = m_start + n;
			std::uninitialized_fill_n(m_start, n, val);//����n��val�ĸ���
		}
		//��Χ���캯��  ��ֹ����������ƥ��     ���͹���   
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
		//�������캯��
		vector(const vector& other) : vector(other.begin(), other.end()) {}
		//�ƶ����캯��
		vector(vector&& other) noexcept //����Чת����Դ����Ȩ  ��ֵ���ñ������
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

			// �����Ԫ������
			const size_type old_size = size();

			// �������ڴ�
			T* new_start = allocate(n);
			T* new_finish = new_start;

			// �ƶ���Ԫ�ص����ڴ棨C++17����ʵ�֣�
			new_finish = std::uninitialized_move(begin(), end(), new_start);
			//������Ӿ��ڴ��ƶ������ڴ� ���⿽������  
			// ������ڴ棨���������ͷţ�
			std::destroy(begin(), end());
			deallocate(m_start);

			// ����ָ��
			m_start = new_start;
			m_finish = new_start + old_size;
			m_end_of_storage = new_start + n;
		}
		vector& operator =(const vector& other) //������ֵ�����
		{
			if (this != &other) {
				vector tmp(other); //������ʱ���� ������ʱ����tmp���other Ȼ�󽻻���ǰ�����tmp����Դ
				swap(tmp); //������ǰ��������ʱ����   ��ʱ���������������ʱ���� �Զ��ͷ�
			}
			return *this;
		}
		vector& operator=(vector&& other) noexcept
		{
			if (this != &other) {
				clear();
				deallocate(m_start);
				//ת����Դ   ֱ����ȡ��ֵ���� other����Դ
				m_start = other.m_start;
				m_finish = other.m_finish;
				m_end_of_storage = other.m_end_of_storage;
				other.m_start = other.m_finish = other.m_end_of_storage = nullptr;
			}
			return *this;
		}

		// ---- Ԫ�ط���
		reference operator[](size_type n) noexcept //��������n��Ԫ�ص����� �����޸�
		{
			return m_start[n];
		}
		const_reference operator[](size_type n) const noexcept
		{
			return m_start[n];
		}
		reference front()  noexcept //������Ԫ�ص�����    ���ٷ�����βԪ��
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
			return m_start; //������Ԫ�ص�ָ��
		}

		//������
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

		//����
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

		//�޸���
		void clear() noexcept
		{
			//��������Ԫ��
			for (iterator it = begin(); it != end(); ++it) {
				it->~T();
			}
			deallocate(m_start);
			m_finish = m_start = m_end_of_storage = nullptr;
		}

		template <typename... Args> // ��������������� �������͵Ĳ��� ����ֱ�ӹ��������е�Ԫ��
		iterator emplace(iterator pos, Args&&... args) {
			// ��¼����λ��ƫ����
			const size_type offset = pos - begin();

			// ����Ƿ���Ҫ����
			if (size() >= capacity()) {
				reserve(capacity() ? capacity() * 2 : 1);
			}

			// ���²���λ�ã����ݿ��ܸı��ڴ棩
			pos = begin() + offset;

			// �ƶ�����Ԫ���ڳ��ռ�
			if (pos != end()) {
				std::move_backward(pos, end(), end() + 1);
			}

			// ������Ԫ��
			::new (pos) T(std::forward<Args>(args)...);
			++m_finish;
			return pos;
		}

		// �������루��ȫ�棩
		iterator insert(iterator pos, size_type n, const T& val) {
			if (n == 0) return pos;

			// ��¼ԭʼƫ����
			const size_type offset = pos - begin();

			// �������
			if (size() + n > capacity()) {
				reserve(size() + n);  // �����ݲ���
			}

			// ���²���λ��
			pos = begin() + offset;

			// �ƶ�����Ԫ��
			std::move_backward(pos, end(), end() + n);

			// �����Ԫ��
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
				// ʹ��Ĭ��ֵ�������Ԫ��
				resize(n, T());
			}
			else if (n < size()) {
				// ֱ�ӽضϣ�����ǰ n ��Ԫ��
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

		void show() const //��ӡ��ǰvector����
		{
			for (const auto& val : *this) std::cout << val << " ";
			std::cout << "\nsize=" << size() << ", capacity=" << capacity() << "\n";
		}
	};
}
