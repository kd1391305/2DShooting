#pragma once

template <typename T>
class ObjectPool
{
public:
    ObjectPool(){}

    ObjectPool(std::size_t size)
    {
        for (std::size_t i = 0; i < size; ++i)
        {
            pool.push_back(new T());
        }
    }

    ~ObjectPool()
    {
        for (T* obj : pool)
        {
            delete obj;
        }
    }

    // プールから取得（自動返却）
    std::shared_ptr<T> acquire()
    {
        std::lock_guard<std::mutex> lock(mtx);

        if (pool.empty())
        {
            // 空なら新規生成
            return create_shared(new T());
        }

        T* obj = pool.back();
        pool.pop_back();
        return create_shared(obj);
    }

private:
    std::shared_ptr<T> create_shared(T* obj)
    {
        // weak_ptr にして循環参照を防ぐ
        std::weak_ptr<ObjectPool<T>> self = this->shared_from_this();

        return std::shared_ptr<T>(
            obj,
            T * ptr
            {
            if (auto pool = self.lock())
            {
                pool->release(ptr);
            }
            else
            {
                // プールが先に死んでいたら普通に delete
                delete ptr;
            }
            }
        );
    }

    void release(T* obj)
    {
        std::lock_guard<std::mutex> lock(mtx);
        pool.push_back(obj);
    }

private:
    std::vector<T*> pool;
    std::mutex mtx;
};