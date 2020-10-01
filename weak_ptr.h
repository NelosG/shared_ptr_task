#pragma once

#include "shared_ptr.h"

template<typename T>
struct shared_ptr;

template<typename T>
struct weak_ptr {

    weak_ptr() noexcept
            : control(nullptr), Tpoint(nullptr) {}

    weak_ptr(const weak_ptr &r) noexcept
            : control(r.control), Tpoint(r.Tpoint) {
        if (control != nullptr)
            control->weak_counter++;
    }

    weak_ptr(weak_ptr &&r) noexcept
            : weak_ptr() {
        swap(r);
    }

    template<class U>
    weak_ptr(const weak_ptr<U> &r) noexcept
            : control(r.control), Tpoint(r.Tpoint) {
        if (control != nullptr)
            control->weak_counter++;
    }

    template<class U>
    weak_ptr(const shared_ptr<U> &r) noexcept
            : control(r.control), Tpoint(r.Tpoint) {
        if (control != nullptr)
            control->weak_counter++;
    }


    shared_ptr<T> lock() const noexcept {
        if (control != nullptr && control->counter != 0)
            return shared_ptr<T>(*this);
        return shared_ptr<T>();
    }

    ~weak_ptr() {
        release();
    }


    weak_ptr &operator=(const weak_ptr &r) noexcept {
        weak_ptr temp(r);
        swap(temp);
        return *this;
    }

    template<class U>
    weak_ptr &operator=(const weak_ptr<U> &r) noexcept {
        weak_ptr temp(r);
        swap(temp);
        return *this;
    }

    template<class U>
    weak_ptr &operator=(const shared_ptr<U> &r) noexcept {
        weak_ptr temp(r);
        swap(temp);
        return *this;
    }

    weak_ptr &operator=(weak_ptr &&r) noexcept {
        weak_ptr temp(std::move(r));
        swap(temp);
        return *this;
    }

    template<class U>
    weak_ptr &operator=(weak_ptr<U> &&r) noexcept {
        weak_ptr temp(std::move(r));
        swap(temp);
        return *this;
    }
    T *operator->() const noexcept {
        return Tpoint;
    }


private:

    void swap(weak_ptr &r) noexcept {
        std::swap(control, r.control);
        std::swap(Tpoint, r.Tpoint);
    }

    void release() {
        Tpoint = nullptr;
        if (control != nullptr) {
            control->weak_counter--;
            if (control->counter == 0 && control->weak_counter == 0) {
                delete control;
                control = nullptr;
            }
        }
    }

    friend class shared_ptr<T>;
    template <typename U>
    friend class shared_ptr;
    template <typename U>
    friend class weak_ptr;

    control_block *control;
    T *Tpoint;
};
