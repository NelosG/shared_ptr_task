#pragma once

struct control_block {

    size_t counter = 0;

    size_t weak_counter = 0;

    virtual void delete_object() = 0;
    virtual ~control_block() = default;
};


template<typename T, typename D>
struct deleter_c_block : control_block, D {

    T *ptr;

    deleter_c_block(T *p, D d) : ptr(p), D(std::move(d)) {}

    void delete_object() override {
        static_cast<D &>(*this)(ptr);
    }
};


template<typename T>
struct smart_c_block : control_block {

    typename std::aligned_storage<sizeof(T), alignof(T)>::type data;

    template<typename ...Args>
    explicit smart_c_block(Args &&...args) {
        new(&data) T(std::forward<Args>(args)...);
    }

    void delete_object() override {
        reinterpret_cast<T *>(&data)->~T();
    }

    T *get() {
        return reinterpret_cast<T *>(&data);
    }
};
