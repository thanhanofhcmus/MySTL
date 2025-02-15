#pragma once

#include "List.h"

template <typename T, typename Container_t = List<T>>
class Queue
{
public:

    // using value_t           = typename Container_t::value_t;
    // using pointer_t         = typename Container_t::pointer_t;
    // using const_pointer_t   = typename Container_t::const_pointer_t;
    // using reference_t       = typename Container_t::reference_t;

public:

    using iterator          = typename Container_t::iterator;
    using const_iterator    = typename Container_t::const_iterator;

    iterator begin() { return m_Underlying.begin(); }
    const_iterator begin() const { return m_Underlying.begin(); }

    iterator end() { return m_Underlying.end(); }
    const_iterator end() const { return m_Underlying.end(); }

public:

    // Constructors - destructor

    Queue() = default;

    explicit Queue(const Container_t& container)
        : m_Underlying(container) {}

    Queue(const Queue&) = default;
    Queue(Queue&&) = default;

    // Assignment operators

    Queue& operator=(const Queue&) = default;
    Queue& operator=(Queue&&) = default;

    // Capacity

    std::size_t size() const { return m_Underlying.size(); }

    bool empty() const { return m_Underlying.empty(); }

    // Accessors

    T& front() { return m_Underlying.front(); }
    const T& front() const { return m_Underlying.front(); }

    T& back() { return m_Underlying.back(); }
    const T& back() const { return m_Underlying.back(); }

    // Modifiers

    void clear() { m_Underlying.clear(); }

    void push(const T& val) { m_Underlying.push_back(val); }
    void push(T&& val) { m_Underlying.push_back(std::move(val)); }

    template <typename... Args>
    void emplace(Args&&... args) { m_Underlying.emplace_back(std::forward<Args>(args)...); }

    void pop() { m_Underlying.pop_front(); }

private:

    Container_t m_Underlying;
};
