#pragma once

#include "shared_ptr.h"

template<class T, class... Args>
shared_ptr<T> make_shared(Args &&... args) {
    return shared_ptr<T>(
            new smart_c_block<T>(std::forward<Args>(args)...)
                    );
}


template<class T, class U>
bool operator==(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept {
    return l.get() == r.get();
}

template<class T, class U>
bool operator!=(const shared_ptr<T> &l, const shared_ptr<U> &r) noexcept {
    return l.get() != r.get();
}

template<class T>
bool operator==(const shared_ptr<T> &l, std::nullptr_t) noexcept {
    return !l;
}

template<class T>
bool operator==(std::nullptr_t, const shared_ptr<T> &r) noexcept {
    return !r;
}

template<class T>
bool operator!=(const shared_ptr<T> &l, std::nullptr_t t) noexcept {
    return !(l == t);
}

template<class T>
bool operator!=(std::nullptr_t t, const shared_ptr<T> &r) noexcept {
    return !(t == r);
}
