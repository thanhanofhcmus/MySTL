#pragma once

#include <initializer_list>
#include "Iterator.h"

template <typename T>
struct List_Node
{
    // List_Node's data has to be in this order so it could be
    // reinterpret-casted correctly to List_Node_No_Value.

    List_Node* prev;
    List_Node* next;
    T data;

    List_Node()
        : data(T()), prev(nullptr), next(nullptr) {}

    List_Node(List_Node* _prev, List_Node* _next)
        : prev(_prev), next(_next) {}

    List_Node(const T& _data, List_Node* _prev, List_Node* _next)
        :  data(_data), prev(_prev), next(_next) {}

    List_Node(T&& _data, List_Node* _prev, List_Node* _next)
        : data(std::move(_data)), prev(_prev), next(_next) {}
};

template <typename T>
struct List_Node_No_Value
{
    List_Node<T>* prev;
    List_Node<T>* next;

    List_Node_No_Value()
        : prev(nullptr), next(nullptr) {}

    List_Node_No_Value(List_Node<T>* _prev, List_Node<T>* _next)
        : prev(_prev), next(_next) {}
};

template <typename T>
class List
{
public:

    using value_t           = T;
    using pointer_t         = T*;
    using const_pointer_t   = const T*;
    using reference_t       = T&;
    using node_t            = List_Node<T>;
    using nodeptr_t         = List_Node<T>*;

public:

    using iterator          = BidirectionalIterator<List<T>>;
    using const_iterator    = ConstBidirectionalIterator<List<T>>;

    iterator begin() { return iterator(m_Head); }
    const_iterator begin() const { return const_iterator(m_Head); }

    iterator end() 
    {
        m_End = List_Node_No_Value<T>(m_Tail, nullptr);
        return (nodeptr_t)&m_End; 
    }

    const_iterator end() const
    {
        m_End = List_Node_No_Value<T>(m_Tail, nullptr);
        return (nodeptr_t)&m_End; 
    }

public:

	// Constructors - destructor

    List()
        :m_Head(nullptr), m_Tail(nullptr), m_Size(0) {}

    List(size_t count, const T& val = T());

    List(std::initializer_list<T> iList);

    List(const List& copy);

    List(List&& move);
    
    ~List();

	// assignment operators

    List& operator=(const List& copy);

    List& operator=(List&& move);

    // Capacity

    size_t size() const { return m_Size; }

    bool empty() const { return (m_Size == 0); }

    // Accessors

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    // Modifiers

    void clear();

    void erase(const_iterator pos);
    void erase(const_iterator begin, const_iterator end);

    void insert(const_iterator pos, const T& val);
    void insert(const_iterator pos, T&& val);

    template <typename... Args>
    void emplace(const_iterator pos, Args&&... args);

    void push_back(const T& val);
    void push_back(T&& val);

    void push_front(const T& val);
    void push_front(T&& val);

    template <typename... Args>
    void emplace_back(Args&&... args);

    template <typename... Args>
    void emplace_front(Args&&... args);

    void pop_back();

    void pop_front();

    // Operations

    // void merge(List& other)
    // void merge(List&& other)

    // size_t remove(const T& compareData);
    
    // template<typename Predicate>
    // size_t remove_if(Predicate pre);

    // size_t unique();
    // template<typename Predicate>
    // size_t unique(Predicate pre);

    // void reverse();

    // void sort();

private:

    void link_node(bool atTail, nodeptr_t val);

private:

    nodeptr_t m_Head;
    nodeptr_t m_Tail;
    size_t m_Size;

    mutable List_Node_No_Value<T> m_End;
};

template <typename T>
void List<T>::link_node(bool atTail, nodeptr_t val)
{
    if (m_Head == nullptr)
        m_Head = m_Tail = val;
    else
    {
        if (atTail)
        {
            m_Tail->next = val;
            m_Tail = val;
        }
        else
        {
            m_Head->prev = val;
            m_Head = val;
        }   
    }
    m_Size++;
}

template <typename T>
List<T>::List(size_t count, const T& val)
    :List()
{
    for (size_t i = 0; i < count; ++i)
        push_back(val);
}

template <typename T>
List<T>::List(std::initializer_list<T> iList)
    :List()
{
    const T* begin = iList.begin();

    for (size_t i = 0; i < iList.size(); ++i)
        push_back(std::move(begin[i]));
}

template <typename T>
List<T>::List(const List<T>& copy)
    :List()
{
    nodeptr_t copyP = copy.m_Head;
    
    while (copyP != nullptr)
    {
        reference_t data = copyP->data;
        push_back(data);
        copyP = copyP->next;
    }
}

template <typename T>
List<T>::List(List<T>&& move)
    :m_Size(move.m_Size), m_Head(move.m_Head), m_Tail(move.m_Tail)
{
    move.m_Size = 0;
    move.m_Head = nullptr;
    move.m_Tail = nullptr;
}

template <typename T>
List<T>::~List<T>()
{
    clear();
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& copy)
{
    if (this != &copy)
    {
        clear();

        nodeptr_t copyP = copy.m_Head;
        
        while (copyP != nullptr)
        {
            reference_t data = copyP->data;
            push_back(data);
            copyP = copyP->next;
        }
    }

    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List<T>&& move)
{
    if (this != &move)
    {
        clear();

        m_Size = move.m_Size;
        m_Head = move.m_Head;
        m_Tail = move.m_Tail;

        move.m_Size = 0;
        move.m_Head = nullptr;
        move.m_Tail = nullptr;
        
    }
    
    return *this;
}

template <typename T>
T& List<T>::back()
{
    assert(!empty());

    return m_Tail->data;
}

template <typename T>
const T& List<T>::back() const
{
    assert(!empty());

    return m_Tail->data;
}

template <typename T>
T& List<T>::front()
{
    assert(!empty());

    return m_Head->data;
}

template <typename T>
const T& List<T>::front() const
{
    assert(!empty());

    return m_Head->data;
}

template <typename T>
void List<T>::clear()
{
    nodeptr_t iter = m_Head;
    nodeptr_t next = nullptr;

    while (iter != nodeptr_t(&m_End))
    {
        next = iter->next;
        delete iter;
        iter = next;
    }

    m_Size = 0;
    m_Head = nullptr;
    m_Tail = nullptr;
}

template <typename T>
void List<T>::erase(const_iterator pos)
{
    if (empty())
        return;

    nodeptr_t curr = pos.m_ProxyData;
    nodeptr_t prev = curr->prev;
    nodeptr_t next = curr->next;

    delete curr;
    prev->next = next;
    next->prev = prev;
    m_Size--;
}

template <typename T>
void List<T>::erase(const_iterator begin, const_iterator end)
{
    if (empty() || !begin.m_ProxyData)
        return;

    nodeptr_t iter = begin.m_ProxyData;
    nodeptr_t first = iter->prev;
    nodeptr_t last = (end.m_ProxyData) ? (end.m_ProxyData)->next : nodeptr_t(&m_End);
    nodeptr_t next = nullptr;

    while (iter != last)
    {
        next = iter->next;
        delete iter;
        iter = next;
        m_Size--;
    }

    if (first)
        first->next = last;
    else
        m_Head->next = last;
    
    if (last)
        last->prev = first;
    else
        m_Tail = first;
    
}

template <typename T>
void List<T>::insert(const_iterator pos, const T& val)
{
    nodeptr_t curr = pos.m_ProxyData;
    if (curr == nullptr)
    {
        push_back(val);
        return;
    }

    nodeptr_t newNode = new node_t(val, curr, curr->next);

    curr->next = newNode;
    m_Size++;
}

template <typename T>
void List<T>::insert(const_iterator pos, T&& val)
{
    nodeptr_t curr = pos.m_ProxyData;
    if (curr == nullptr)
    {
        push_back(val);
        return;
    }

    nodeptr_t newNode = (nodeptr_t)::operator new(sizeof(node_t));

    new(newNode) node_t(T(std::forward<T>(val)), curr, curr->next);
    curr->next = newNode;
    m_Size++;
}

template <typename T>
template <typename... Args>
void List<T>::emplace(const_iterator pos, Args&&... args)
{
    nodeptr_t curr = pos.m_ProxyData;
    if (curr == nullptr)
    {
        push_back(args...);
        return;
    }

    nodeptr_t newNode = (nodeptr_t)::operator new(sizeof(node_t));

    new(newNode) node_t(T(std::forward<Args>(args)...), curr, curr->next);
    curr->next = newNode;
    m_Size++;
}

template <typename T>
void List<T>::push_back(const T& val)
{
    nodeptr_t newNode = new node_t(val, m_Tail, nodeptr_t(&m_End));

    link_node(true, newNode);
}

template <typename T>
void List<T>::push_back(T&& val)
{
    nodeptr_t newNode = (nodeptr_t)::operator new(sizeof(node_t));

    new(newNode) node_t(T{ std::forward<T>(val) }, m_Tail, nodeptr_t(&m_End));

    link_node(true, newNode);
}

template <typename T>
void List<T>::push_front(const T& val)
{
    nodeptr_t newNode = new node_t(val, nullptr, m_Head);

    link_node(false, newNode);
}

template <typename T>
void List<T>::push_front(T&& val)
{
    nodeptr_t newNode = (nodeptr_t)::operator new(sizeof(node_t));

    new(newNode) node_t(T(std::forward<T>(val)), nullptr, m_Head);

    link_node(false, newNode);
}

template <typename T>
template <typename... Args>
void List<T>::emplace_back(Args&&... args)
{
    nodeptr_t newNode = (nodeptr_t)::operator new(sizeof(node_t));

    new(newNode) node_t(T(std::forward<Args>(args)...), m_Tail, nodeptr_t(&m_End));

    link_node(true, newNode);
}

template <typename T>
template <typename... Args>
void List<T>::emplace_front(Args&&... args)
{
    nodeptr_t newNode = (nodeptr_t)::operator new(sizeof(node_t));

    new(newNode) node_t(T(std::forward<Args>(args)...), nullptr, m_Head);

    link_node(false, newNode);
}

template <typename T>
void List<T>::pop_back()
{
    if (empty())
        return;

    nodeptr_t prevTail = m_Tail->prev;
    prevTail->next = nullptr;
    delete m_Tail;
    m_Tail = prevTail;
    m_Size--;
}

template <typename T>
void List<T>::pop_front()
{
    if (empty())
        return;

    nodeptr_t nextHead = m_Head->next;
    nextHead->prev = nullptr;
    delete m_Head;
    m_Head = nextHead;
    m_Size--;
}