#pragma once

#include "control_block.h"
#include "weak_ptr.h"
#include "other_functions.h"

template<typename T>
struct weak_ptr;

template<typename T>
struct shared_ptr {
    shared_ptr() noexcept
            : control(nullptr), Tpoint(nullptr) {}

    explicit shared_ptr(std::nullptr_t) noexcept
            : shared_ptr() {}

    template<class U>
    explicit shared_ptr(U *p)
            : shared_ptr(p, std::default_delete<U>()) {}

    template<class U, class D>
    shared_ptr(U *p, D& d)
            : Tpoint(p) {
        try {
            control = new deleter_c_block<U, D>(p, d);
        } catch (...) {
            d(p);
            throw;
        }
        control->counter++;
    }

    shared_ptr(const shared_ptr &r) noexcept
            : control(r.control), Tpoint(r.Tpoint) {
        if (control != nullptr)
            control->counter++;
    }

    template<class U>
    shared_ptr(const shared_ptr<U> &r) noexcept
            : control(r.control), Tpoint(r.Tpoint) {
        if (control != nullptr)
            control->counter++;
    }

    shared_ptr(shared_ptr &&r) noexcept
            : shared_ptr() {
        swap(r);
    }

    template<class U>
    shared_ptr(const shared_ptr<U> &r, T *p) noexcept
            : control(r.control), Tpoint(p) {
        if (control != nullptr)
            control->counter++;
    }

    template<class U>
    explicit shared_ptr(const weak_ptr<U> &r)
            : control(r.control), Tpoint(r.Tpoint) {
        if (control != nullptr && control->counter != 0)
            control->counter++;
    }

    void reset() noexcept {
        release();
    }

    template<class U>
    void reset(U *p) {
        return reset(p, std::default_delete<U>());
    }

    template<class U, class D>
    void reset(U *p, D d) {
        shared_ptr temp(p, std::move(d));
        swap(temp);
    }

    size_t use_count() const noexcept {
        return control != nullptr ? control->counter : 0;
    }

    T *get() const noexcept {
        return Tpoint;
    }

    ~shared_ptr() {
        release();
    }

    explicit operator bool() const noexcept {
        return Tpoint != nullptr;
    }

    T *operator->() const noexcept {
        return Tpoint;
    }

    T &operator*() const noexcept {
        return *Tpoint;
    }

    shared_ptr &operator=(const shared_ptr &r) noexcept {
        if (this != &r) {
            shared_ptr temp(r);
            swap(temp);
        }
        return *this;
    }

    template<class U>
    shared_ptr &operator=(const shared_ptr<U> &r) noexcept {
        shared_ptr temp(r);
        swap(temp);
        return *this;
    }

    shared_ptr &operator=(shared_ptr &&r) noexcept {
        shared_ptr temp(std::move(r));
        swap(temp);
        return *this;
    }

    template<class U>
    shared_ptr &operator=(shared_ptr<U> &&r) noexcept {
        shared_ptr temp(std::move(r));
        swap(temp);
        return *this;
    }

private:
    explicit shared_ptr(smart_c_block<T> *cb)
            : control(cb), Tpoint(cb->get()) {
        control->counter++;
    }

    void swap(shared_ptr &r) noexcept {
        std::swap(control, r.control);
        std::swap(Tpoint, r.Tpoint);
    }

    void release() {
        Tpoint = nullptr;
        if (control != nullptr) {
            control->counter--;
            if (control->counter == 0) {
                control->delete_object();
                if (control->weak_counter == 0) {
                    delete control;
                    control = nullptr;
                }
            }
        }
    }

    friend class weak_ptr<T>;
    template <typename U>
    friend class weak_ptr;
    template <typename U>
    friend class shared_ptr;
    template<class T, class... Args>
    friend shared_ptr<T> make_shared(Args &&... args);

    control_block *control;
    T *Tpoint;
};
