#pragma once
#ifndef SYDEVS_POINTER_H_
#define SYDEVS_POINTER_H_

#include <memory>

namespace sydevs {


/**
 * @brief A data type which represents a pointer to anything.
 *
 * @details
 * A `pointer` instance encapsulates `std::shared_ptr<void>`, which is
 * essentially a reference-counted pointer to any type of data. The example
 * below demonstrates how to use the class.
 * 
 * ~~~
 * auto val = pointer(new int64(7));
 * val.reset();
 * val.reset(new float64(-0.5));
 * if (val) {
 *     auto x = val.dereference<float64>();
 * }
 * ~~~
 */
class pointer
{
public:
    /**
     * @brief Constructs a null `pointer` instance.
     */
    pointer() noexcept;

    /**
     * @brief Constructs a null `pointer` instance from a `nullptr`.
     */
    explicit pointer(std::nullptr_t X) noexcept;

    /**
     * @brief Constructs a `pointer` instance referencing the same data as `ptr`.
     */
    template<typename T> 
    explicit pointer(T* ptr);

    pointer(const pointer&) noexcept            = default;  ///< Copy constructor
    pointer& operator=(const pointer&) noexcept = default;  ///< Copy assignment
    pointer(pointer&&) noexcept                 = default;  ///< Move constructor
    pointer& operator=(pointer&&) noexcept      = default;  ///< Move assignment
    ~pointer()                                  = default;  ///< Destructor

    void reset() noexcept;  ///< Modifies the pointer to reference `nullptr`.

    template<typename T> 
    void reset(T* ptr);  ///< Modifies the pointer to reference the same data as `ptr`.

    template<typename T> 
    T& dereference() const;  ///< Returns the referenced data, casting it to type `T`.
    
    explicit operator bool() const noexcept;  ///< Returns `true` if the pointer does not reference `nullptr`.

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
