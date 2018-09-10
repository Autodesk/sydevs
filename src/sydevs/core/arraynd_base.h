#pragma once
#ifndef SYDEVS_ARRAYND_BASE_H_
#define SYDEVS_ARRAYND_BASE_H_

#include <sydevs/core/range.h>
#include <functional>
#include <algorithm>
#include <memory>
#include <vector>
#include <array>

namespace sydevs {


/**
 * @brief A base class template for a multidimensional array with elements
 *        of type `T` arranged in a lattice of `ndim` dimensions.
 *
 * @details
 * `arraynd_base` contains all member variables, all member functions that
 * behave the same for `ndims == 1` and `ndims > 1`, and all member functions
 * that do not return a multidimensional array.
 */
template<typename T, int64 ndims>
class arraynd_base
{
friend class arraynd_base<T, ndims-1>;
friend class arraynd_base<T, ndims+1>;
public:
    ~arraynd_base() = default;  ///< Destructor

    std::array<int64, ndims> dims() const;     ///< Returns the lengths of each dimension.
    std::array<int64, ndims> strides() const;  ///< Returns the number of element-size steps in memory between successive elements for each dimension.

    bool empty() const;    ///< Returns `true` if there is at least one element.
    int64 size() const;    ///< Returns the total number of elements.
    int64 offset() const;  ///< Returns the number of element-size steps in memory before the first element.

    const T* data() const;  ///< Returns a pointer to the element data (const).
    T* data();              ///< Returns a pointer to the element data.

    bool is_contiguous() const;  ///< Returns `true` if a row-major traversal of the multidimensional array accesses each element of `data` in sequence.
    bool is_view() const;        ///< Returns `true` if this multidimensional array is a view of another, meaning that data is shared.
    bool is_readonly() const;    ///< Returns `true` if the data is readonly, in which case attempts to modify it raise a std::logic_error.

    const T& operator()(const std::array<int64, ndims>& indices) const;  ///< Returns a reference to an element (const).
    T& operator()(const std::array<int64, ndims>& indices);              ///< Returns a reference to an element.

    const T& operator()(const arraynd_base<int64, 1>& indices) const;  ///< Returns a reference to an element (const).
    T& operator()(const arraynd_base<int64, 1>& indices);              ///< Returns a reference to an element.

    template<typename... Indices>
    const T& operator()(Indices... indices) const;  ///< Returns a reference to an element (const).

    template<typename... Indices>
    T& operator()(Indices... indices);  ///< Returns a reference to an element.

    void fill(const T& value);                                                                  ///< Replaces every element with `value`.
    void assign(const arraynd_base<T, ndims>& rhs);                                             ///< Replaces every element with the corresponding value in `rhs`.
    void assign_from_function(std::function<T(const std::array<int64, ndims>& indices)> func);  ///< Replaces every element with the result of `func` evaluated at the cooresponding `indices`.

    /**
     * @brief Traverses the multidimensional array in row-major order, calling
     *        `func` at every element.
     * 
     * @details
     * For each element traversed in row-major order, `func` is called with the
     * `indices` of the element and the element's `value`. If `func` returns
     * `true`, the traversal continues. If `func` returns `false`, the 
     * traversal is terminated.
     * 
     * @param func The function to be called at every element.
     * @return `true` if the entire multidimensional array is traversed and 
     *         `func` returns `true` at every element; `false` otherwise.
     */
    bool traverse(std::function<bool(const std::array<int64, ndims>& indices, const T& value)> func) const;

protected:
    arraynd_base();
    arraynd_base(const std::array<int64, ndims>& dims, const T& value);
    arraynd_base(const std::array<int64, ndims>& dims, const std::vector<T>& data);
    arraynd_base(const std::array<int64, ndims>& dims, std::function<T(const std::array<int64, ndims>& indices)> func);

    arraynd_base(const arraynd_base<T, ndims+1>& rhs, int64 index, bool is_readonly);
    arraynd_base(const arraynd_base<T, ndims>& rhs, range r, bool is_readonly);

    arraynd_base(const arraynd_base<T, ndims>& rhs, bool is_view, bool is_readonly);

    arraynd_base(const arraynd_base<T, ndims>& rhs);
    arraynd_base<T, ndims>& operator=(const arraynd_base<T, ndims>& rhs);

    arraynd_base(arraynd_base<T, ndims>&&)                      = default;
    arraynd_base<T, ndims>& operator=(arraynd_base<T, ndims>&&) = default;

    static void transpose(arraynd_base<T, ndims>& arr);
    static void swap_axes(arraynd_base<T, ndims>& arr, int64 idim0, int64 idim1);
    static void subdivide_axis(const arraynd_base<T, ndims>& arr0, arraynd_base<T, ndims + 1>& arr, int64 idim, const std::array<int64, 2>& dims);
    static void absorb_axis(const arraynd_base<T, ndims>& arr0, arraynd_base<T, ndims-1>& arr, int64 idim);

private:
    void compute_strides_and_size();

    void fill_recursively(const T& value, std::array<int64, ndims>& indices, int64 idim, int64 offset);
    void assign_recursively(const arraynd_base<T, ndims>& rhs, std::array<int64, ndims>& indices, int64 idim, int64 offset);
    void assign_from_function_recursively(std::function<T(const std::array<int64, ndims>& indices)> func, std::array<int64, ndims>& indices, int64 idim, int64 offset);
    bool traverse_recursively(std::function<bool(const std::array<int64, ndims>& indices, const T& value)> func, std::array<int64, ndims>& indices, int64 idim, int64 offset) const;
    
    std::array<int64, ndims> dims_;
    std::array<int64, ndims> strides_;
    int64 size_;
    int64 offset_;
    int64 rotation_;
    bool is_view_;
    bool is_readonly_;
    std::shared_ptr<T> data_ptr_;
};


template<typename T, int64 ndims>
arraynd_base<T, ndims>::arraynd_base()
    : dims_()
    , strides_()
    , size_(0)
    , offset_(0)
    , rotation_(0)
    , is_view_(false)
    , is_readonly_(false)
    , data_ptr_(nullptr)
{
    static_assert(ndims >= 1, "ndims must be at least 1");
    for (int idim = 0; idim < ndims; ++idim) {
        dims_[idim] = 0;
    }
    compute_strides_and_size();
    data_ptr_ = std::shared_ptr<T>(new T[size_], std::default_delete<T[]>());
}


template<typename T, int64 ndims>
arraynd_base<T, ndims>::arraynd_base(const std::array<int64, ndims>& dims, const T& value)
    : dims_(dims)
    , strides_()
    , size_(0)
    , offset_(0)
    , rotation_(0)
    , is_view_(false)
    , is_readonly_(false)
    , data_ptr_(nullptr)
{
    static_assert(ndims >= 1, "ndims must be at least 1");
    compute_strides_and_size();
    data_ptr_ = std::shared_ptr<T>(new T[size_], std::default_delete<T[]>());
    fill(value);
}


template<typename T, int64 ndims>
arraynd_base<T, ndims>::arraynd_base(const std::array<int64, ndims>& dims, const std::vector<T>& data)
    : dims_(dims)
    , strides_()
    , size_(0)
    , offset_(0)
    , rotation_(0)
    , is_view_(false)
    , is_readonly_(false)
    , data_ptr_(nullptr)
{
    static_assert(ndims >= 1, "ndims must be at least 1");
    compute_strides_and_size();
    if (data.size() != size_) throw std::domain_error("Length of multidimensional array data must match product of array dimensions");
    data_ptr_ = std::shared_ptr<T>(new T[size_], std::default_delete<T[]>());
    for (int64 i = 0; i < size_; ++i){
        data_ptr_.get()[i] = data[i];
    }
}


template<typename T, int64 ndims>
arraynd_base<T, ndims>::arraynd_base(const std::array<int64, ndims>& dims, std::function<T(const std::array<int64, ndims>& indices)> func)
    : dims_(dims)
    , strides_()
    , size_(0)
    , offset_(0)
    , rotation_(0)
    , is_view_(false)
    , is_readonly_(false)
    , data_ptr_(nullptr)
{
    static_assert(ndims >= 1, "ndims must be at least 1");
    compute_strides_and_size();
    data_ptr_ = std::shared_ptr<T>(new T[size_], std::default_delete<T[]>());
    assign_from_function(func);
}


template<typename T, int64 ndims>
std::array<int64, ndims> arraynd_base<T, ndims>::dims() const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    return dims_;
}


template<typename T, int64 ndims>
std::array<int64, ndims> arraynd_base<T, ndims>::strides() const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    return strides_;
}


template<typename T, int64 ndims>
bool arraynd_base<T, ndims>::empty() const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    return size_ == 0;
}


template<typename T, int64 ndims>
int64 arraynd_base<T, ndims>::size() const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    return size_;
}


template<typename T, int64 ndims>
int64 arraynd_base<T, ndims>::offset() const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    return offset_;
}


template<typename T, int64 ndims>
const T* arraynd_base<T, ndims>::data() const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    return data_ptr_.get();
}


template<typename T, int64 ndims>
T* arraynd_base<T, ndims>::data()
{
    if (is_readonly_) throw std::logic_error("Attempt to obtain a non-const reference to readonly multidimensional array data");
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    return data_ptr_.get();
}


template<typename T, int64 ndims>
bool arraynd_base<T, ndims>::is_contiguous() const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    auto contiguous_so_far = true;
    auto size = 1;
    for (int64 idim = ndims - 1; contiguous_so_far && idim >= 0; --idim) {
        if (strides_[idim] != size) {
            contiguous_so_far = false;
        }
        else {
            size *= dims_[idim];
        }
    }
    return contiguous_so_far;
}


template<typename T, int64 ndims>
bool arraynd_base<T, ndims>::is_view() const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    return is_view_;
}


template<typename T, int64 ndims>
bool arraynd_base<T, ndims>::is_readonly() const
{
    return is_readonly_;
}


template<typename T, int64 ndims>
const T& arraynd_base<T, ndims>::operator()(const std::array<int64, ndims>& indices) const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    int64 offset = offset_;
    for (int64 idim = 0; idim < ndims; ++idim) {
        if (indices[idim] < 0 || indices[idim] >= dims_[idim]) throw std::out_of_range("Invalid multidimensional array index");
        offset += indices[idim]*strides_[idim];
    }
    return data_ptr_.get()[offset];
}


template<typename T, int64 ndims>
T& arraynd_base<T, ndims>::operator()(const std::array<int64, ndims>& indices)
{
    if (is_readonly_) throw std::logic_error("Attempt to obtain a non-const reference to readonly multidimensional array data");
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    int64 offset = offset_;
    for (int64 idim = 0; idim < ndims; ++idim) {
        if (indices[idim] < 0 || indices[idim] >= dims_[idim]) throw std::out_of_range("Invalid multidimensional array index");
        offset += indices[idim]*strides_[idim];
    }
    return data_ptr_.get()[offset];
}


template<typename T, int64 ndims>
const T& arraynd_base<T, ndims>::operator()(const arraynd_base<int64, 1>& indices) const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    if (indices.size() != ndims) throw std::logic_error("Size of indexing array does not match the number of multidimensional array dimensions");
    int64 offset = offset_;
    for (int64 idim = 0; idim < ndims; ++idim) {
        if (indices({idim}) < 0 || indices({idim}) >= dims_[idim]) throw std::out_of_range("Invalid multidimensional array index");
        offset += indices({idim})*strides_[idim];
    }
    return data_ptr_.get()[offset];
}


template<typename T, int64 ndims>
T& arraynd_base<T, ndims>::operator()(const arraynd_base<int64, 1>& indices)
{
    if (is_readonly_) throw std::logic_error("Attempt to obtain a non-const reference to readonly multidimensional array data");
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    if (indices.size() != ndims) throw std::logic_error("Size of indexing array does not match the number of multidimensional array dimensions");
    int64 offset = offset_;
    for (int64 idim = 0; idim < ndims; ++idim) {
        if (indices({idim}) < 0 || indices({idim}) >= dims_[idim]) throw std::out_of_range("Invalid multidimensional array index");
        offset += indices({idim})*strides_[idim];
    }
    return data_ptr_.get()[offset];
}


template<typename T, int64 ndims>
template<typename... Indices>
const T& arraynd_base<T, ndims>::operator()(Indices... indices) const
{
    return (*this)({indices...});
}


template<typename T, int64 ndims>
template<typename... Indices>
T& arraynd_base<T, ndims>::operator()(Indices... indices)
{
    return (*this)({indices...});
}

    
template<typename T, int64 ndims>
void arraynd_base<T, ndims>::fill(const T& value)
{
    if (is_readonly_) throw std::logic_error("Attempt to modify readonly multidimensional array data");
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    auto indices = std::array<int64, ndims>();
    fill_recursively(value, indices, 0, offset_);
}

    
template<typename T, int64 ndims>
void arraynd_base<T, ndims>::assign(const arraynd_base<T, ndims>& rhs)
{
    if (is_readonly_) throw std::logic_error("Attempt to modify readonly multidimensional array data");
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    for (int64 idim = 0; idim < ndims; ++idim) {
        if (dims_[idim] != rhs.dims()[idim]) throw std::logic_error("Multidimensional array dimensions do not match");
    }
    auto indices = std::array<int64, ndims>();
    assign_recursively(rhs, indices, 0, offset_);
}

    
template<typename T, int64 ndims>
void arraynd_base<T, ndims>::assign_from_function(std::function<T(const std::array<int64, ndims>& indices)> func)
{
    if (is_readonly_) throw std::logic_error("Attempt to modify readonly multidimensional array data");
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    auto indices = std::array<int64, ndims>();
    assign_from_function_recursively(func, indices, 0, offset_);
}

    
template<typename T, int64 ndims>
bool arraynd_base<T, ndims>::traverse(std::function<bool(const std::array<int64, ndims>& indices, const T& value)> func) const
{
    if (rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    auto indices = std::array<int64, ndims>();
    return traverse_recursively(func, indices, 0, offset_);
}


template<typename T, int64 ndims>
arraynd_base<T, ndims>::arraynd_base(const arraynd_base<T, ndims+1>& rhs, int64 index, bool is_readonly)
    : dims_()
    , strides_()
    , size_(0)
    , offset_(0)
    , rotation_(rhs.rotation_)
    , is_view_(true)
    , is_readonly_(is_readonly)
    , data_ptr_(rhs.data_ptr_)
{
    if (index < 0 || index >= rhs.dims_[0]) throw std::out_of_range("Invalid multidimensional array index");
    size_ = 1;
    for (int64 idim = 0; idim < ndims; ++idim) {
        dims_[idim] = rhs.dims_[idim + 1];
        strides_[idim] = rhs.strides_[idim + 1];
        size_ *= dims_[idim];
    }
    offset_ = rhs.offset_ + index*rhs.strides_[0];
    if (rotation_ > 0) {
        --rotation_;
    }
}


template<typename T, int64 ndims>
arraynd_base<T, ndims>::arraynd_base(const arraynd_base<T, ndims>& rhs, range r, bool is_readonly)
    : dims_()
    , strides_()
    , size_(0)
    , offset_(0)
    , rotation_(rhs.rotation_)
    , is_view_(true)
    , is_readonly_(is_readonly)
    , data_ptr_(rhs.data_ptr_)
{
    if (r.start() < 0 || r.start() >= rhs.dims_[0]) throw std::out_of_range("Multidimensional array range must start inside array");
    if (r.stride() == 0) throw std::invalid_argument("Multidimensional array range must have a nonzero stride");
    size_ = 1;
    for (int64 idim = 0; idim < ndims - 1; ++idim) {
        dims_[idim] = rhs.dims_[idim + 1];
        strides_[idim] = rhs.strides_[idim + 1];
        size_ *= dims_[idim];
    }
    int64 stop = (r.stop() < 0)                ? 0 :
                 (r.stop() > rhs.dims_[0] - 1) ? rhs.dims_[0] - 1 :
                                                 r.stop();
    int64 gap = stop - r.start();
    dims_[ndims - 1] = 0;
    if (gap == 0 || (gap > 0 && r.stride() > 0) || (gap < 0 && r.stride() < 0)) {
        dims_[ndims - 1] = (gap + r.stride())/r.stride();
    }
    strides_[ndims - 1] = rhs.strides_[0]*r.stride();
    size_ *= dims_[ndims - 1];
    offset_ = rhs.offset_ + r.start()*rhs.strides_[0];
    if (rotation_ == 0) {
        rotation_ = ndims - 1;
    }
    else {
        --rotation_;
    }
}

    
template<typename T, int64 ndims>
arraynd_base<T, ndims>::arraynd_base(const arraynd_base<T, ndims>& rhs, bool is_view, bool is_readonly)
    : dims_(rhs.dims_)
    , strides_(rhs.strides_)
    , size_(rhs.size_)
    , offset_(rhs.offset_)
    , rotation_(rhs.rotation_)
    , is_view_(is_view)
    , is_readonly_(is_readonly)
    , data_ptr_(nullptr)
{
    if (is_view_) {
        data_ptr_ = rhs.data_ptr_;
    }
    else {
        compute_strides_and_size();
        offset_ = 0;
        rotation_ = 0;
        is_readonly_ = false;
        data_ptr_ = std::shared_ptr<T>(new T[size_], std::default_delete<T[]>());
        assign(rhs);
    }
}


template<typename T, int64 ndims>
arraynd_base<T, ndims>::arraynd_base(const arraynd_base<T, ndims>& rhs)
    : dims_(rhs.dims_)
    , strides_(rhs.strides_)
    , size_(rhs.size_)
    , offset_(rhs.offset_)
    , rotation_(rhs.rotation_)
    , is_view_(rhs.is_view_)
    , is_readonly_(rhs.is_readonly_)
    , data_ptr_(nullptr)
{
    if (is_view_) {
        data_ptr_ = rhs.data_ptr_;
    }
    else {
        compute_strides_and_size();
        offset_ = 0;
        rotation_ = 0;
        is_readonly_ = false;
        data_ptr_ = std::shared_ptr<T>(new T[size_], std::default_delete<T[]>());
        assign(rhs);
    }
}


template<typename T, int64 ndims>
arraynd_base<T, ndims>& arraynd_base<T, ndims>::operator=(const arraynd_base<T, ndims>& rhs)
{
    if (is_view_) {
        assign(rhs);
    }
    else {
        dims_ = rhs.dims_;
        is_view_ = rhs.is_view_;
        if (is_view_) {
            strides_ = rhs.strides_;
            size_ = rhs.size_;
            offset_ = rhs.offset_;
            rotation_ = rhs.rotation_;
            is_readonly_ = rhs.is_readonly_;
            data_ptr_ = rhs.data_ptr_;
        }
        else {
            compute_strides_and_size();
            offset_ = 0;
            rotation_ = 0;
            is_readonly_ = false;
            data_ptr_ = std::shared_ptr<T>(new T[size_], std::default_delete<T[]>());
            assign(rhs);
        }
    }
    return *this;
}


template<typename T, int64 ndims>
void arraynd_base<T, ndims>::transpose(arraynd_base<T, ndims>& arr)
{
    if (arr.rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    std::reverse(arr.dims_.begin(), arr.dims_.end());
    std::reverse(arr.strides_.begin(), arr.strides_.end());
}


template<typename T, int64 ndims>
void arraynd_base<T, ndims>::swap_axes(arraynd_base<T, ndims>& arr, int64 idim0, int64 idim1)
{
    if (arr.rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    if (idim0 < 0 || idim0 >= ndims || idim1 < 0 || idim1 >= ndims) throw std::out_of_range("Invalid multidimensional array index");
    std::swap(arr.dims_[idim0], arr.dims_[idim1]);
    std::swap(arr.strides_[idim0], arr.strides_[idim1]);
}


template<typename T, int64 ndims>
void arraynd_base<T, ndims>::subdivide_axis(const arraynd_base<T, ndims>& arr0, arraynd_base<T, ndims + 1>& arr, int64 idim, const std::array<int64, 2>& dims)
{
    if (arr0.rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    if (!arr0.is_contiguous()) throw std::domain_error("Attempt to subdivide an axis of a non-contiguous multi-dimensional array");
    if (idim < 0 || idim >= ndims) throw std::out_of_range("Invalid multidimensional array index");
    if (dims[0]*dims[1] != arr0.dims_[idim]) throw std::domain_error("Product of post-subdivision multidimensional array dimensions does not match pre-subdivision dimension");
    for (int64 i = 0; i < idim; ++i) {
        arr.dims_[i] = arr0.dims_[i];
        arr.strides_[i] = arr0.strides_[i];
    }
    arr.dims_[idim] = dims[0];
    arr.dims_[idim + 1] = dims[1];
    arr.strides_[idim] = arr0.strides_[idim]*dims[1];
    arr.strides_[idim + 1] = arr0.strides_[idim];
    for (int64 i = idim + 2; i < ndims + 1; ++i) {
        arr.dims_[i] = arr0.dims_[i - 1];
        arr.strides_[i] = arr0.strides_[i - 1];
    }
    arr.size_ = arr0.size_;
    arr.offset_ = arr0.offset_;
    arr.rotation_ = 0;
    arr.is_view_ = true;
    arr.is_readonly_ = arr0.is_readonly_;
    arr.data_ptr_ = arr0.data_ptr_;
}


template<typename T, int64 ndims>
void arraynd_base<T, ndims>::absorb_axis(const arraynd_base<T, ndims>& arr0, arraynd_base<T, ndims-1>& arr, int64 idim)
{
    if (arr0.rotation_ > 0) throw std::logic_error("Attempt to use a partially sliced multidimensional array (try completing the slicing operation by invoking operator[](range) as needed)");
    if (!arr0.is_contiguous()) throw std::domain_error("Attempt to absorb an axis of a non-contiguous multi-dimensional array");
    if (idim < 0 || idim >= ndims) throw std::out_of_range("Invalid multidimensional array index");
    if (idim == 0) throw std::domain_error("Attempt to absorb the first axis of a multidimensional array (the second axis must be absorbed into the first)");
    for (int64 i = 0; i < idim - 1; ++i) {
        arr.dims_[i] = arr0.dims_[i];
        arr.strides_[i] = arr0.strides_[i];
    }
    arr.dims_[idim - 1] = arr0.dims_[idim - 1]*arr0.dims_[idim];
    arr.strides_[idim - 1] = arr0.strides_[idim];
    for (int64 i = idim; i < ndims - 1; ++i) {
        arr.dims_[i] = arr0.dims_[i + 1];
        arr.strides_[i] = arr0.strides_[i + 1];
    }
    arr.size_ = arr0.size_;
    arr.offset_ = arr0.offset_;
    arr.rotation_ = 0;
    arr.is_view_ = true;
    arr.is_readonly_ = arr0.is_readonly_;
    arr.data_ptr_ = arr0.data_ptr_;
}


template<typename T, int64 ndims>
void arraynd_base<T, ndims>::compute_strides_and_size()
{
    size_ = 1;
    for (int64 idim = ndims - 1; idim >= 0; --idim) {
        strides_[idim] = size_;
        size_ *= dims_[idim];
    }
}


template<typename T, int64 ndims>
void arraynd_base<T, ndims>::fill_recursively(const T& value, std::array<int64, ndims>& indices, int64 idim, int64 offset)
{
    if (idim == ndims - 1) {
        for (int64 i = 0; i < dims_[idim]; ++i) {
            indices[idim] = i;
            data_ptr_.get()[offset + i*strides_[idim]] = value;
        }
    }
    else {
        for (int64 i = 0; i < dims_[idim]; ++i) {
            indices[idim] = i;
            fill_recursively(value, indices, idim + 1, offset + i*strides_[idim]);
        }
    }
}


template<typename T, int64 ndims>
void arraynd_base<T, ndims>::assign_recursively(const arraynd_base<T, ndims>& rhs, std::array<int64, ndims>& indices, int64 idim, int64 offset)
{
    if (idim == ndims - 1) {
        for (int64 i = 0; i < dims_[idim]; ++i) {
            indices[idim] = i;
            data_ptr_.get()[offset + i*strides_[idim]] = rhs(indices);
        }
    }
    else {
        for (int64 i = 0; i < dims_[idim]; ++i) {
            indices[idim] = i;
            assign_recursively(rhs, indices, idim + 1, offset + i*strides_[idim]);
        }
    }
}


template<typename T, int64 ndims>
void arraynd_base<T, ndims>::assign_from_function_recursively(std::function<T(const std::array<int64, ndims>& indices)> func, std::array<int64, ndims>& indices, int64 idim, int64 offset)
{
    if (idim == ndims - 1) {
        for (int64 i = 0; i < dims_[idim]; ++i) {
            indices[idim] = i;
            data_ptr_.get()[offset + i*strides_[idim]] = func(indices);
        }
    }
    else {
        for (int64 i = 0; i < dims_[idim]; ++i) {
            indices[idim] = i;
            assign_from_function_recursively(func, indices, idim + 1, offset + i*strides_[idim]);
        }
    }
}


template<typename T, int64 ndims>
bool arraynd_base<T, ndims>::traverse_recursively(std::function<bool(const std::array<int64, ndims>& indices, const T& value)> func, std::array<int64, ndims>& indices, int64 idim, int64 offset) const
{
    auto proceed = true;
    if (idim == ndims - 1) {
        for (int64 i = 0; proceed && i < dims_[idim]; ++i) {
            indices[idim] = i;
            proceed = func(indices, data_ptr_.get()[offset + i*strides_[idim]]);
        }
    }
    else {
        for (int64 i = 0; proceed && i < dims_[idim]; ++i) {
            indices[idim] = i;
            proceed = traverse_recursively(func, indices, idim + 1, offset + i*strides_[idim]);
        }
    }
    return proceed;
}


}  // namespace

#endif
