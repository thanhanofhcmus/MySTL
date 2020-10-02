#pragma once

#include "List.h"

template <typename T, typename Container_t = List<T>>
class Stack
{
public:

    // using value_t           = typename Container_t::value_t;
    // using pointer_t         = typename Containter_t::pointer_t;
    // using const_pointer_t   = typename Containter_t::const_pointer_t;
    // using reference_t       = typename Containter_t::reference_t;

public:

    using iterator          = typename Container_t::iterator;
    using const_iterator    = typename Container_t::const_iterator;

    iterator begin() { return m_Underlying.begin(); }
    const_iterator begin() const { return m_Underlying.begin(); }

    iterator end() { return m_Underlying.end(); }
    const_iterator end() const { return m_Underlying.end(); }

public:

    // Constructors - destructor

    Stack() = default;

    explicit Stack(const Container_t& containter)
        : m_Underlying(containter) {}

    Stack(const Stack&) = default;
    Stack(Stack&&) = default;

    // Assignment operators

    Stack& operator=(const Stack&) = default;
    Stack& operator=(Stack&&) = default;

    // Capacity

    size_t size() const { return m_Underlying.size(); }

    bool empty() const { return m_Underlying.empty(); }

    // Accessor

    T& top() { return m_Underlying.back(); }
    const T& top() const { return m_Underlying.back(); }

    // Modifiers

    void clear() { m_Underlying.clear(); }

    void push(const T& val) { m_Underlying.push_back(val); }
    void push(T&& val) { m_Underlying.push_back(std::move(val)); }

    template <typename... Args>
    void emplace(Args&&... args) { m_Underlying.emplace_back(std::forward<Args>(args)...); }

    void pop() { m_Underlying.pop_back(); }

private:

    Container_t m_Underlying;
};