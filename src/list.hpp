#pragma once

#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <cstddef>
#include <cstdlib>
#include <cstring>

constexpr const size_t INIT_MEM = 32;

template <typename List>
struct ListIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    using value_type = typename List::EType;
    using pointer = value_type*;
    using reference = value_type&;

    using Self = ListIterator<List>;

    pointer ptr;

    auto operator++() -> Self&
    {
        ptr++;
        return *this;
    }

    auto operator++(int) -> Self
    {
        Self it = *this;
        ++(*this);
        return it;
    }

    auto operator--() -> Self&
    {
        ptr--;
        return *this;
    }

    auto operator--(int) -> Self
    {
        Self it = *this;
        --(*this);
        return it;
    }

    auto operator[](const size_t _idx) -> reference
    {
        return *(ptr+_idx);
    }

    auto operator->() -> pointer
    {
        return ptr;
    }

    auto operator*() -> reference
    {
        return *ptr;
    }

    auto operator==(const Self& _rhs) -> const bool
    {
        return ptr == _rhs.ptr;
    }

    auto operator!=(const Self& _rhs) -> const bool
    {
        return ptr != _rhs.ptr;
    }

    explicit ListIterator(const pointer _ptr)
     : ptr(_ptr)
    {}
};

template<typename T, const size_t ESize = sizeof(T)>
struct List {
    
    using EType = T;
    using Self = List<EType, ESize>;
    using Iterator = ListIterator<Self>;
    using ConstIterator = ListIterator<const Self>;
    using InitList = std::initializer_list<EType>;

    size_t size{0};
    size_t mem{INIT_MEM};
    EType* data{};

    auto cbegin() const
    {
        return ConstIterator(data);
    }

    auto cend() const
    {
        return ConstIterator(data+size);
    }

    auto begin() -> Iterator
    {
        return Iterator(data);
    }

    auto end() -> Iterator
    {
        return Iterator(data+size);
    }

    auto first() -> EType&
    {
        return *data;
    }

    auto last() -> EType&
    {
        return *(data+size-1);
    }

    auto alloc(const size_t _mem) -> EType*
    {
        // using malloc because I need a list that doesn't care about constructors and deconstructors
        return (_mem) ? static_cast<T*>(std::malloc((ESize * _mem))) : data;
    }

    auto clear() -> void
    {
        size = 0;
    }

    auto reset() -> void
    {
        this->clear();
        std::memset(data, 0, mem);
    }

    auto resize(const size_t _mem) -> void
    {
        if (!_mem) return;

        if (data == nullptr) {
            data = this->alloc(_mem);
            mem = _mem;
            return;
        }

        EType* new_data = this->alloc(_mem);
        std::copy(this->cbegin(), this->cend(), new_data);
        std::free(data);

        mem = _mem;
        data = new_data;
    }

    auto realloc() -> void
    {
        this->resize((mem * 2));
    }

    auto push(const T& _elem) -> void
    {
        if (size >= mem) this->realloc();
        data[size++] = _elem;
    }

    auto push(T&& _elem) -> void
    {
        if (size >= mem) this->realloc();
        data[size++] = std::move(_elem);
    }

    auto push(Self&& _list) -> void
    {
        if (_list.size+size >= mem) this->resize(_list.size+size * 2);
        for ( auto&& elem : _list ) data[size++] = std::move(elem);
    }

    auto push(InitList&& _list) -> void
    {
        if (_list.size()+size >= mem) this->resize(_list.size()+size * 2); 
        for ( auto&& elem : _list ) data[size++] = std::move(elem);
    }

    auto push(Self& _list) -> void
    {
        if (_list.size+size >= mem) this->resize(_list.size+size * 2);
        for ( auto&& elem : _list ) data[size++] = std::move(elem);
    }

    auto push(InitList& _list) -> void
    {
        if (_list.size()+size >= mem) this->resize(_list.size()+size * 2); 
        for ( auto&& elem : _list ) data[size++] = std::move(elem);
    }

    template<typename... Args>
    auto place(Args&&... _args) -> void
    {
        if (size >= mem) this->realloc();
        data[size++] = EType(std::forward<Args>(_args)...);
    }

    auto pop() -> EType
    {
        return *(data+(--size));
    }

    auto at(const size_t _idx) -> T&
    {
        if (_idx > size) std::abort(); // (Carter) TODO add proper OOB protection
        return data[_idx];
    }

    auto operator[](const size_t _idx) -> T&
    {
        return this->at(_idx);
    }

    auto operator*() -> T&
    {
        return this->first();
    }

    auto friend operator<<(Self& _list, const EType& _elem) -> void
    {
        _list.push(_elem);
    }

    auto friend operator<<(Self& _list, EType&& _elem) -> void
    {
        _list.push(std::move(_elem));
    }

    auto friend operator>>(Self& _list, const EType& _var) -> void
    {
        _var = _list.pop();
    }

    explicit List()
     : data(this->alloc(mem))
    {}

    explicit List(const size_t _size)
     : size(_size), data(this->alloc(mem))
    {
        if (size > mem) this->realloc();
    }

    List(const Self& _list)
     : size(_list.size), mem(_list.mem), data(this->alloc(mem))
    {
        std::copy(_list.cbegin(), _list.cend(), data);
    }

    List(const InitList& _list)
     : size(_list.size()), mem(size), data(this->alloc(mem))
    {
        std::copy(_list.begin(), _list.end(), data);
    }

    ~List()
    {
        std::free(data);
        data = nullptr;
        size = 0;
        mem = 0;    
    }
};
