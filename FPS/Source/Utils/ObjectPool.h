#pragma once
#include <vector>
#include <queue>
#include <memory>
#include <functional>

template<typename T>
class ObjectPool
{
private:
    std::vector<std::unique_ptr<T>> m_objects;
    std::queue<T*> m_available;
    std::function<std::unique_ptr<T>()> m_factory;
    size_t m_maxSize;

public:
    explicit ObjectPool(size_t maxSize = 100)
        : m_maxSize(maxSize)
    {
        m_objects.reserve(maxSize);
    }

    ObjectPool(size_t maxSize, std::function<std::unique_ptr<T>()> factory)
        : m_maxSize(maxSize)
        , m_factory(factory)
    {
        m_objects.reserve(maxSize);
        
        // Pre-allocate objects
        for (size_t i = 0; i < maxSize; ++i)
        {
            auto obj = m_factory();
            m_available.push(obj.get());
            m_objects.push_back(std::move(obj));
        }
    }

    ~ObjectPool() = default;

    // Get an object from the pool
    T* Acquire()
    {
        if (m_available.empty())
        {
            if (m_objects.size() < m_maxSize && m_factory)
            {
                // Create new object if pool not full and factory available
                auto obj = m_factory();
                T* ptr = obj.get();
                m_objects.push_back(std::move(obj));
                return ptr;
            }
            return nullptr; // Pool exhausted
        }

        T* obj = m_available.front();
        m_available.pop();
        return obj;
    }

    // Return an object to the pool
    void Release(T* obj)
    {
        if (obj)
        {
            // Reset object state if it has a Reset method
            if constexpr (requires { obj->Reset(); })
            {
                obj->Reset();
            }
            
            m_available.push(obj);
        }
    }

    // Get current pool statistics
    size_t GetTotalSize() const { return m_objects.size(); }
    size_t GetAvailableCount() const { return m_available.size(); }
    size_t GetUsedCount() const { return m_objects.size() - m_available.size(); }
    size_t GetMaxSize() const { return m_maxSize; }

    // Clear all objects
    void Clear()
    {
        while (!m_available.empty())
        {
            m_available.pop();
        }
        m_objects.clear();
    }

    // Pre-allocate objects with custom factory
    void PreAllocate(size_t count, std::function<std::unique_ptr<T>()> factory)
    {
        m_factory = factory;
        
        for (size_t i = 0; i < count && m_objects.size() < m_maxSize; ++i)
        {
            auto obj = m_factory();
            m_available.push(obj.get());
            m_objects.push_back(std::move(obj));
        }
    }

    // Iterator support for accessing all objects
    auto begin() { return m_objects.begin(); }
    auto end() { return m_objects.end(); }
    auto begin() const { return m_objects.begin(); }
    auto end() const { return m_objects.end(); }
};

// Specialized pool for simple types that don't need factory
template<typename T>
class SimpleObjectPool
{
private:
    std::vector<T> m_objects;
    std::queue<size_t> m_available;
    size_t m_nextIndex;

public:
    explicit SimpleObjectPool(size_t maxSize = 100)
        : m_nextIndex(0)
    {
        m_objects.resize(maxSize);
        
        // Add all indices to available queue
        for (size_t i = 0; i < maxSize; ++i)
        {
            m_available.push(i);
        }
    }

    T* Acquire()
    {
        if (m_available.empty())
        {
            return nullptr; // Pool exhausted
        }

        size_t index = m_available.front();
        m_available.pop();
        return &m_objects[index];
    }

    void Release(T* obj)
    {
        if (obj >= &m_objects[0] && obj < &m_objects[0] + m_objects.size())
        {
            size_t index = obj - &m_objects[0];
            m_available.push(index);
        }
    }

    size_t GetTotalSize() const { return m_objects.size(); }
    size_t GetAvailableCount() const { return m_available.size(); }
    size_t GetUsedCount() const { return m_objects.size() - m_available.size(); }
};
