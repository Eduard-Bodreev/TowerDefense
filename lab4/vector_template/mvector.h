#pragma once
#ifndef __MVECTOR__
#define __MVECTOR__

#include <stdexcept>

template<class T>
class mvectorIt;

template<class T>
class mvectorConstIt;


template <class T>
class mvector {
	friend class mvectorIt<T>;

private:
	static const int _QUOTA = 10;
	size_t _cur, _max;
	T* _arr;

private:
	void resize();

public:
	mvector();
	mvector(size_t size);
	mvector(std::initializer_list<T> init_list);
	mvector(const mvector<T>& other);  
	~mvector();

	size_t size() const noexcept { return _cur; };
	void push_back(const T& el) noexcept;
	bool empty() const noexcept { return _cur == 0; };
	void clear() noexcept;


	T& operator= (const T& other) noexcept; // assignment
	T& operator[] (size_t ind);             // l-value
	const T& operator[] (size_t ind) const; // r-value
	bool operator==(const mvector<T>&);
	bool operator!=(const mvector<T>&);

	// iterator methods
	typedef mvectorIt<T> Iterator;
	Iterator begin() const noexcept { return Iterator(_arr); }
	Iterator end() const noexcept   { return Iterator(_arr + _cur); }

	typedef mvectorConstIt<T> ConstIterator;
	ConstIterator cbegin()  const noexcept { return ConstIterator(_arr); }
	ConstIterator cend()  const noexcept   { return ConstIterator(_arr + _cur); }
};


template<class T>
class mvectorIt {
private:
	T* _cur;
public:
	mvectorIt() : _cur(0) {};
	mvectorIt(T* a) : _cur(a) {};
	bool operator!= (const mvectorIt&) const noexcept;
	bool operator== (const mvectorIt&) const noexcept;
	T& operator*()  const;
	T* operator->() const;
	mvectorIt& operator++() noexcept {
		++_cur;
		return *this;
	};
	mvectorIt operator++(int) noexcept {
		mvectorIt<T> res(*this);
		++_cur;
		return res;
	}
	mvectorIt& operator--() noexcept {
		--_cur;
		return *this;
	};
	mvectorIt operator--(int) noexcept {
		mvectorIt<T> res(*this);
		--_cur;
		return res;
	}
};


template<class T>
class mvectorConstIt {
private:
	const T* _cur;
public:
	mvectorConstIt() : _cur(0) {};
	mvectorConstIt(T* a) : _cur(a) {};
	bool operator!= (const mvectorConstIt&) const noexcept;
	bool operator== (const mvectorConstIt&) const noexcept;
	const T& operator*()  const;
	const T* operator->() const;
	mvectorConstIt& operator++() noexcept {
		++_cur;
		return *this;
	};
	mvectorConstIt operator++(int) noexcept {
		mvectorConstIt<T> res(*this);
		++_cur;
		return res;
	}
	mvectorConstIt& operator--() noexcept {
		--_cur;
		return *this;
	};
	mvectorConstIt operator--(int) noexcept {
		mvectorConstIt<T> res(*this);
		--_cur;
		return res;
	}
};

#endif __MVECTOR__

template<class T>
inline void mvector<T>::resize() {
	T* new_arr = new T[_max += _QUOTA];
	for (size_t i = 0; i < _cur; i++)
		new_arr[i] = _arr[i];
	delete[] _arr;
	_arr = new_arr;
}


template<class T>
inline mvector<T>::mvector() :
	_cur(0),
	_max(_QUOTA),
	_arr(new T[_max]) {};

template<class T>
inline mvector<T>::mvector(size_t size) :
	_cur(0),
	_max(size),
	_arr(new T[_max]) {};

template<class T>
inline mvector<T>::mvector(std::initializer_list<T> init_list) :
	_cur(init_list.size()),
	_max(_cur + _QUOTA),
	_arr(new T[_max]) {
	int i = 0;
	for (auto value : init_list)
		_arr[i++] = value;
};


template<class T>
inline mvector<T>::mvector(const mvector<T>& other) : 
	_cur(other.size()), 
	_max(other.size() + _QUOTA),  
	_arr(new T[_max]) {
	for (size_t i = 0; i < _cur; i++)
		_arr[i] = other[i];
};

template<class T>
inline mvector<T>::~mvector() {
	delete[] _arr;
}

template<class T>
inline void mvector<T>::push_back(const T& el) noexcept {
	if (_cur == _max)
		resize();
	_arr[_cur++] = el;
}

template<class T>
inline void mvector<T>::clear() noexcept {
	delete[] _arr;
	_arr = new T[_max];
	_cur = 0;
}

template<class T>
inline T& mvector<T>::operator=(const T& other) noexcept {
	if (this != &other) {
		delete[] _arr;
		_cur = other.size();
		_arr = new T[_max = _cur + _QUOTA];
		for (size_t i = 0; i < size; i++)
			_arr[i] = other[i];	
	}
	return *this;
}

template<class T>
inline T& mvector<T>::operator[](size_t ind) {
	if (ind < 0 || ind >= _max)
		throw std::out_of_range("Illegal index!");
	return _arr[ind];	
}

template<class T>
inline const T& mvector<T>::operator[](size_t ind) const {
	if (ind < 0 || ind >= _max)
		throw std::out_of_range("Illegal index!");
	return _arr[ind];	
}

template<class T>
inline bool mvector<T>::operator==(const mvector<T>& other) {
	if (_cur != other._cur)
		return false;
	for (size_t i = 0; i < _cur; i++)
		if (_arr[i] != other._arr[i])
			return false;
	return true;
}

template<class T>
inline bool mvector<T>::operator!=(const mvector<T>& other) {
	return !(*this == other);
}

template<class T>
inline bool mvectorIt<T>::operator!=(const mvectorIt& other) const noexcept {
	return _cur != other._cur;
}

template<class T>
inline bool mvectorIt<T>::operator==(const mvectorIt& other) const noexcept {
	return _cur == other._cur;
}

template<class T>
inline T& mvectorIt<T>::operator*() const {
	if (_cur)
		return *_cur;
	throw std::out_of_range("Illegal value of iterator!");
}

template<class T>
inline T* mvectorIt<T>::operator->() const {
	if (_cur)
		return _cur;
	throw std::out_of_range("Illegal value of iterator!");
}

template<class T>
inline bool mvectorConstIt<T>::operator!=(const mvectorConstIt& other) const noexcept {
	return _cur != other._cur;
}

template<class T>
inline bool mvectorConstIt<T>::operator==(const mvectorConstIt& other) const noexcept {
	return _cur == other._cur;
}

template<class T>
inline const T& mvectorConstIt<T>::operator*() const {
	if(_cur) return *_cur;
	throw std::out_of_range("Illegal value of iterator!");
}


template<class T>
inline const T* mvectorConstIt<T>::operator->() const {
	if(_cur) return _cur;
	throw std::out_of_range("Illegal value of iterator!");
	return _cur;
}