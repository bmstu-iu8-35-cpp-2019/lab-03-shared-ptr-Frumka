// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_MY_SHARED_PTR_HPP_
#define INCLUDE_MY_SHARED_PTR_HPP_

#include <atomic>
#include <utility>
#include <stdexcept>

class link_control {
private:
    std::atomic_uint _counter;

public:
    link_control() {
        _counter = 0;
    }

    explicit link_control(size_t number) {
        _counter = number;
    }

    void increase() {
        ++_counter;
    }

    void decrease() {
        if (_counter > 0) {
            --_counter;
        } else {
            throw std::logic_error("Counter is already zero");
        }
    }

    bool operator==(const unsigned int &a) const {
        return _counter == a;
    }

    size_t get() const {
        return static_cast<size_t>(_counter);
    }
};

template<typename T>
class SharedPtr {
private:
    link_control *_control = nullptr;
    T *_data = nullptr;

public:
    SharedPtr() = default;

    explicit SharedPtr(T *data) {
        if (data) {
            _control = new link_control(1);
            _data = data;
        }
    }

    SharedPtr(const SharedPtr &r) {
        _control = r._control;
        _control->increase();
        _data = r._data;
    }

    SharedPtr(SharedPtr &&r) {
        _control = r._control;
        r._control = nullptr;

        _data = r._data;
        r.data = nullptr;

        _control->increase();
    }

    ~SharedPtr() {
        if (*this) {
            _control->decrease();

            if (*_control == 0) {
                delete _control;
                delete _data;
            }
        }
    }

    SharedPtr<T> &operator=(const SharedPtr &r) {
        if (*this) {
            _control->decrease();

            if (*_control == 0) {
                delete _data;
                delete _control;
            }
        }

        _control = r._control;
        _data = r._data;

        if (*this)
            _control->increase();

        return (*this);
    }

    auto operator=(SharedPtr &&r) -> SharedPtr & {
        if (*this) {
            _control->decrease();
            if (*_control == 0) {
                delete _data;
                delete _control;
            }
        }
        _control = r._control;
        _data = r._data;

        if (*this)
            _control->increase();

        return (*this);
    }

    operator bool() const {
        return _data != nullptr;
    }

    auto operator*() const -> T & {
        return *_data;
    }

    auto operator->() const -> T * {
        return _data;
    }

    auto get() -> T * {
        return _data;
    }

    void reset() {
        if (!*this) return;
        _control->decrease();
        if (_control->get() == 0) {
            delete _data;
            delete _control;
        }
        _data = nullptr;
        _control = nullptr;
    }

    void reset(T *ptr) {
        if (*this) {
            _control->decrease();
            if (_control->get() == 0) {
                delete _control;
                delete _data;
            }
        }
        _control = new link_control(1);
        _data = ptr;
    }

    void swap(SharedPtr &r) {
        if (_data == r._data) return;
        auto temp = _data;
        _data = r._data;
        r._data = temp;
        return;
    }

    size_t use_count() const {
        if (!*this) return 0;
        return static_cast<size_t>(_control->get());
    }
};

#endif // INCLUDE_MY_SHARED_PTR_HPP_