#pragma once
#ifndef SYDEVS_POINTER_H_
#define SYDEVS_POINTER_H_

#include <memory>

namespace sydevs {


class pointer
{
public:
    pointer() noexcept;
    explicit pointer(std::nullptr_t X) noexcept;

    template<typename T> 
    explicit pointer(T* ptr);

    pointer(const pointer&) noexcept            = default;  ///< Copy constructor
    pointer& operator=(const pointer&) noexcept = default;  ///< Copy assignment
    pointer(pointer&&) noexcept                 = default;  ///< Move constructor
    pointer& operator=(pointer&&) noexcept      = default;  ///< Move assignment
    ~pointer()                                  = default;  ///< Destructor

    void reset() noexcept;

    template<typename T> 
    void reset(T* ptr);

    template<typename T> 
    T& dereference() const;
    
    explicit operator bool() const noexcept;

private:
    std::shared_ptr<void> ptr_;
};


inline pointer::pointer() noexcept
    : ptr_()
{
}


inline pointer::pointer(std::nullptr_t X) noexcept
    : ptr_(X)
{
}


template<typename T> 
inline pointer::pointer(T* ptr)
    : ptr_(ptr)
{
}


inline void pointer::reset() noexcept
{
    ptr_.reset();
}


template<typename T> 
inline void pointer::reset(T* ptr)
{
    ptr_.reset(ptr);
}


template<typename T> 
inline T& pointer::dereference() const
{
    if (!ptr_) throw std::logic_error("Attempt to dereference null pointer object");
    return *static_cast<T*>(ptr_.get());
}

    
inline pointer::operator bool() const noexcept
{
    return bool(ptr_);
}


}  // namespace

#endif
