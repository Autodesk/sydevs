#pragma once
#ifndef SYDEVS_ARRAYND_H_
#define SYDEVS_ARRAYND_H_

#include <sydevs/core/arraynd_base.h>
#include <ostream>

namespace sydevs {

    
/**
 * @brief A class template for a multidimensional array with elements of type
 *        `T` arranged in a lattice of `ndims` dimensions.
 *
 * @details
 * An `arraynd` object represents a multidimensional array of elements of type
 * `T` arranged in a rectangular lattice of `ndims` dimensions. Although the
 * element type and number of dimensions are determined at compile-time, the
 * lengths the dimensions (`dims`), the total number of elements (`size`), and
 * the value of each element can be changed dynamically.
 *
 * Multidimensional arrays can be constructed using a set of 9 type aliases
 * named `arraynd`, but with `n` replaced by 1, 2, 3, ..., 9. The instruction
 * below constructs a 3D multidimensional array with dimensions 2, 4, and 5, 
 * with all 40 floating-point elements initialized to zero.
 *
 * ~~~
 * #include <sydevs/core/arraynd.h>
 *
 * auto arr = array3d<float64>({2, 4, 5}, 0.0);
 * ~~~
 *
 * Multidimensional array elements are stored in row-major order. When an array
 * is constructed with data supplied in the form of a `std::vector`, the vector
 * elements are copied into the array in row-major order. The following example
 * illustrates the construction and printing of a 2-by-3 array populated with 
 * data from a length-6 vector.
 *
 * ~~~
 * auto arr = array2d<int64>({2, 3}, {0, 1, 2, 3, 4, 5});
 *
 * std::cout << arr;  // Prints "{{0, 1, 2}, {3, 4, 5}}".
 * ~~~
 * 
 * Arrays can also be constructed by mapping indices to values.
 *
 * ~~~
 * auto arr = array3d<int64>({2, 3, 4}, [](const std::array<int64, 3>& indices) {
 *     return 100*indices[0] + 10*indices[1] + indices[2];
 * });
 *
 * std::cout << arr;  // Prints "{{{0, 1, 2, 3}, 
 *                    //           {10, 11, 12, 13}, 
 *                    //           {20, 21, 22, 23}},
 *                    //          {{100, 101, 102, 103}, 
 *                    //           {110, 111, 112, 113},
 *                    //           {120, 121, 122, 123}}}"
 *                    //
 *                    //        (without line breaks).
 * ~~~
 *
 * The most efficient way to access individual multidimensional array elements
 * is using `operator()`.
 *
 * ~~~
 * auto arr = array3d<int64>({2, 3, 4}, [](const std::array<int64, 3>& indices) {
 *     return 100*indices[0] + 10*indices[1] + indices[2];
 * });
 *
 * std::cout << arr(1, 0, 3);  // Prints "103".
 *
 * arr(1, 0, 3) = 7;  // Changes an element to "7".
 * ~~~
 *
 * 1D arrays and std::array instances can also be used to access elements.
 *
 * Elements can also be accessed by chaining `ndims` invocations of 
 * `operator[]`. However, this is less efficient than `operator()` because it
 * involves constructing `ndims - 1` new multidimensional arrays that share
 * memory with the original.
 *
 * ~~~
 * auto arr = array3d<int64>({2, 3, 4}, [](const std::array<int64, 3>& indices) {
 *     return 100*indices[0] + 10*indices[1] + indices[2];
 * });
 *
 * std::cout << arr[1][0][3];  // Prints "103" (inefficient).
 *
 * arr[1][0][3] = 7;  // Changes an element to "7" (inefficient).
 * ~~~
 *
 * One should use `operator[]` in conjunction with the 
 * [range](@ref sydevs::range) class to construct slices that share data with
 * the original array.
 *
 * ~~~
 * auto arr = array3d<int64>({2, 3, 4}, [](const std::array<int64, 3>& indices) {
 *     return 100*indices[0] + 10*indices[1] + indices[2];
 * });
 *
 * std::cout << arr[range()][1][range().start_at(1).stride_by(2)]
 *         // Takes every element from the first dimension.
 *         // Replaces the second dimension with contents associated with index 1.
 *         // Takes every 2rd element from the third dimension starting at index 1.
 *         // Prints "{{11, 13}, {111, 113}}".
 *
 * arr[range()][1][range().start_at(1).stride_by(2)].fill(7);
 *         // Replaces a 2-by-2 slice of the original array with the value 7.
 *
 * arr[range()][1][range().start_at(1).stride_by(2)].assign(other);
 *         // Replaces a 2-by-2 slice of the original array with the values in the
 *         // multidimensional array `other`.
 * ~~~
 *
 * Copying a multidimensional array shares the internal data by default. The
 * result of `operator[]` also shares data, and so does the `view` member
 * function. The `copy` member function performs a deep copy of a 
 * multidimensional array so that no data is shared.
 *
 * ~~~
 * auto arr = array2d<int64>({2, 3}, {0, 1, 2, 3, 4, 5});
 *
 * auto arr2 = arr;                    // Shares data.
 * auto arr3 = arr[range()][range()];  // Shares data.
 * auto arr4 = arr.view();             // Shares data.
 * auto arr5 = arr.copy();             // Copies data.
 * ~~~
 *
 * There are also several transformation functions that either share or copy
 * the data. For example, `view_transposed` creates a new array that reverses
 * the order of the dimensions while sharing data with the original. By 
 * contrast, `copy_transposed` creates a new transposed array with its own
 * copy of the data.
 *
 * If data is shared with a constant multidimensional array, then neither array
 * can be modified. In the following example, `arr` is `const` and `arr2`
 * shares data with it.
 *
 * ~~~
 * try {
 *     const auto arr = array2d<std::string>({2, 2}, {"A", "B", "C", "D"});
 *     auto arr2 = arr.view();
 *     arr2(0, 0) = "E";
 * }
 * catch (const std::logic_error& e) {
 *     std::cout << "ERROR: " << e.what() << std::endl;
 * }
 * ~~~
 *
 * Since replacing element `(0, 0)` of `arr2` would also replace an element of
 * `arr`, and since `arr` is `const`, the error below is produced.
 *
 * `ERROR: Attempt to obtain a non-const reference to readonly multidimensional
 *        array data`
 *
 * Various mathematical operations are available for multidimensional arrays
 * and scalars, and for pairs of multidimensional arrays with the same
 * dimensions.
 * 
 * ~~~
 * auto a = array2d<int64>({2, 3}, {0, 1, 2, 3, 4, 5});
 * auto b = array2d<int64>({2, 3}, {2, 1, 0, 2, 4, 6});
 *
 * std::cout << a + b;               // Prints "{{2, 2, 2}, {5, 8, 11}}".
 * std::cout << a + 5;               // Prints "{{5, 6, 7}, {8, 9, 10}}".
 * std::cout << 5 - a;               // Prints "{{5, 4, 3}, {2, 1, 0}}".
 * std::cout << a/2;                 // Prints "{{0, 0, 1}, {1, 2, 2}}".
 * std::cout << (a < b);             // Prints "{{1, 0, 0}, {0, 0, 1}}".
 * std::cout << (a == b);            // Prints "{{0, 1, 0}, {0, 1, 0}}".
 * std::cout << (a >= 1 && a <= 4);  // Prints "{{0, 1, 1}, {1, 1, 0}}".
 * std::cout << all(a >= 0);         // Prints "1".
 * std::cout << any(a == 7);         // Prints "0".
 * ~~~
 *
 * There is also an element type conversion function `to` and an element
 * replacement function `replace`, demonstrated below.
 *
 * ~~~
 * auto arr = array2d<float64>({2, 2}, {0.5, 1.5, -0.5, -1.5});
 *
 * std::cout << to<int64>(arr);               // Prints "{{0, 1}, {0, -1}}"
 * std::cout << replace(arr, arr < 0, 0);     // Prints "{{0.5, 1.5}, {0, 0}}"
 * std::cout << replace(arr, arr < 0, -arr);  // Prints "{{0.5, 1.5}, {0.5, 1.5}}"
 * ~~~
 */
template<typename T, int64 ndims>
class arraynd : public arraynd_base<T, ndims>
{
friend class arraynd<T, ndims+1>;
public:
    /**
     * @brief Constructs an `arraynd` object with dimensions of length zero.
     *
     * @tparam T The type of all elements of the multidimensional array.
     * @tparam ndims The number of dimensions of the multidimensional array.
     */
    arraynd();

    /**
     * @brief Constructs an `arraynd` object with the specified dimensions 
     *        `dims` and all elements initialized to `value`.
     *
     * @param dims The dimensions.
     * @param value A value to be assigned to all elements of the
     *              multidimensional array.
     *
     * @tparam T The type of all elements of the multidimensional array.
     * @tparam ndims The number of dimensions of the multidimensional array.
     */
    arraynd(const std::array<int64, ndims>& dims, const T& value);

    /**
     * @brief Constructs an `arraynd` object with the specified dimensions 
     *        `dims` and elements initialized with the supplied `data`.
     * 
     * @details
     * The supplied `data` vector must have a length equal to the product of
     * the dimensions so that the multidimensional array can be filled. 
     * Otherwise, a `std::domain_error` exception is thrown. `The elements of
     * `data` are copied into the multidimensional array in row-major order.
     * 
     * @param dims The dimensions.
     * @param data A vector of values to be copied into the multidimensional
     *             array.
     *
     * @tparam T The type of all elements of the multidimensional array.
     * @tparam ndims The number of dimensions of the multidimensional array.
     */
    arraynd(const std::array<int64, ndims>& dims, const std::vector<T>& data);

    /**
     * @brief Constructs an `arraynd` object with the specified dimensions 
     *        `dims` and elements initialized using the function object `func`.
     * 
     * @details
     * The function object `func` maps a set of indices to the value that will
     * be assigned to the corresponding location in the multidimensional array.
     * 
     * @param dims The dimensions.
     * @param func The function object invoked to initialize each element.
     *
     * @tparam T The type of all elements of the multidimensional array.
     * @tparam ndims The number of dimensions of the multidimensional array.
     */
    arraynd(const std::array<int64, ndims>& dims, std::function<T(const std::array<int64, ndims>& indices)> func);
    
    arraynd(const arraynd<T, ndims>&)                      = default;  ///< Copy constructor (if `rhs.is_view()`, data is shared; otherwise data is copied)
    arraynd<T, ndims>& operator=(const arraynd<T, ndims>&) = default;  ///< Copy assignment (if `!is_view() && rhs.is_view()`, data is shared; otherwise data is copied)
    arraynd(arraynd<T, ndims>&&)                           = default;  ///< Move constructor (data is shared)
    arraynd<T, ndims>& operator=(arraynd<T, ndims>&&)      = default;  ///< Move assignment (data is shared)
    ~arraynd()                                             = default;  ///< Destructor

    const arraynd<T, ndims-1> operator[](int64 index) const;  ///< Create a const slice with one dimension replaced by referencing elements at the specified `index` (data is shared).
    arraynd<T, ndims-1> operator[](int64 index);              ///< Create a slice with one dimension replaced by referencing elements at the specified `index` (data is shared).

    const arraynd<T, ndims> operator[](range r) const;  ///< Create a const slice with one dimension filtered by referencing elements in the specified range `r` (data is shared).
    arraynd<T, ndims> operator[](range r);              ///< Create a slice with one dimension filtered by referencing elements in the specified range `r` (data is shared).

    const arraynd<T, ndims> view() const;  ///< Create a const view of the multidimensional array (data is shared).
    arraynd<T, ndims> view();              ///< Create a view of the multidimensional array (data is shared).
    arraynd<T, ndims> copy() const;        ///< Create a deep copy of the multidimensional array (data is copied).

    const arraynd<T, ndims> view_transposed() const;  ///< Create a const transposed view of the multidimensional array (data is shared).
    arraynd<T, ndims> view_transposed();              ///< Create a transposed view of the multidimensional array (data is shared).
    arraynd<T, ndims> copy_transposed() const;        ///< Create a transposed deep copy of the multidimensional array (data is copied).

    const arraynd<T, ndims> view_swapped_axes(int64 idim0, int64 idim1) const;  ///< Create a const view with dimensions `idim0` and `idim1` swapped (data is shared).
    arraynd<T, ndims> view_swapped_axes(int64 idim0, int64 idim1);              ///< Create a view with dimensions `idim0` and `idim1` swapped (data is shared).
    arraynd<T, ndims> copy_swapped_axes(int64 idim0, int64 idim1) const;        ///< Create a deep copy with dimensions `idim0` and `idim1` swapped (data is copied).

    const arraynd<T, ndims + 1> view_subdivided_axis(int64 idim, const std::array<int64, 2>& dims) const;  ///< Create a const view with dimension `idim` split into two dimensions with lengths given by `dims` (data is shared).
    arraynd<T, ndims + 1> view_subdivided_axis(int64 idim, const std::array<int64, 2>& dims);              ///< Create a view with dimension `idim` split into two dimensions with lengths given by `dims` (data is shared).
    arraynd<T, ndims + 1> copy_subdivided_axis(int64 idim, const std::array<int64, 2>& dims) const;        ///< Create a deep copy with dimension `idim` split into two dimensions with lengths given by `dims` (data is copied).

    const arraynd<T, ndims - 1> view_absorbed_axis(int64 idim) const;  ///< Create a const view with dimension `idim` absorbed into dimension `idim - 1` (data is shared).
    arraynd<T, ndims - 1> view_absorbed_axis(int64 idim);              ///< Create a view with dimension `idim` absorbed into dimension `idim - 1` (data is shared).
    arraynd<T, ndims - 1> copy_absorbed_axis(int64 idim) const;        ///< Create a deep copy with dimension `idim` absorbed into dimension `idim - 1` (data is copied).

private:
    arraynd(const arraynd<T, ndims+1>& rhs, int64 index, bool readonly);
    arraynd(const arraynd<T, ndims>& rhs, range r, bool readonly);
    arraynd(const arraynd<T, ndims>& rhs, bool is_view, bool readonly);
};


/**
 * @brief A one-dimensional (1D) specialization of the `arraynd`
 *        multidimensional array template.
 */
template<typename T>
class arraynd<T, 1> : public arraynd_base<T, 1>
{
friend class arraynd<T, 2>;
public:
    /**
     * @brief Constructs a 1D `arraynd` object of length zero.
     */
    arraynd();

    /**
     * @brief Constructs a 1D `arraynd` object with the length specified in
     *        `dims[0]` and all elements initialized to `value`.
     */
    arraynd(const std::array<int64, 1>& dims, const T& value);

    /**
     * @brief Constructs a 1D `arraynd` object with the length specified in
     *        `dims[0]` and elements initialized with the supplied `data`.
     * 
     * @details
     * The supplied `data` vector must have a length equal to `dims[0]` so that
     * the 1D multidimensional array can be filled. Otherwise, a 
     * `std::domain_error` exception is thrown. `The elements of `data` are
     * copied into the multidimensional array in row-major order.
     * 
     * @param dims The dimensions.
     * @param data A vector of values to be copied into the multidimensional
     *             array.
     */
    arraynd(const std::array<int64, 1>& dims, const std::vector<T>& data);

    /**
     * @brief Constructs a 1D `arraynd` object with the length specified in
     *        `dims[0]` and elements initialized using the function object 
     *        `func`.
     * 
     * @details
     * The function object `func` maps a set of indices to the value that will
     * be assigned to the corresponding location in the 1D multidimensional 
     * array.
     * 
     * @param dims The dimensions.
     * @param func The function object invoked to initialize each element.
     */
    arraynd(const std::array<int64, 1>& dims, std::function<T(const std::array<int64, 1>& indices)> func);

    arraynd<T, 1>(const arraynd<T, 1>&)            = default;  ///< Copy constructor (if rhs.is_view(), data is shared; otherwise data is copied)
    arraynd<T, 1>& operator=(const arraynd<T, 1>&) = default;  ///< Copy assignment (if rhs.is_view(), data is shared; otherwise data is copied)
    arraynd<T, 1>(arraynd<T, 1>&&)                 = default;  ///< Move constructor (data is shared)
    arraynd<T, 1>& operator=(arraynd<T, 1>&&)      = default;  ///< Move assignment (data is shared)
    ~arraynd<T, 1>()                               = default;  ///< Destructor

    const T& operator[](int64 index) const;  ///< Create a const reference to the element at the specified `index`.
    T& operator[](int64 index);              ///< Create a reference to the element at the specified `index`.

    const arraynd<T, 1> operator[](range r) const;  ///< Create a const slice with the dimension filtered by referencing elements in the specified range `r` (data is shared).
    arraynd<T, 1> operator[](range r);              ///< Create a slice with the dimension filtered by referencing elements in the specified range `r` (data is shared).

    const arraynd<T, 1> view() const;  ///< Create a const view of the 1D multidimensional array (data is shared).
    arraynd<T, 1> view();              ///< Create a view of the 1D multidimensional array (data is shared).
    arraynd<T, 1> copy() const;        ///< Create a deep copy of the 1D multidimensional array (data is copied).

    const arraynd<T, 2> view_subdivided_axis(int64 idim, const std::array<int64, 2>& dims) const;  ///< Create a const view with the single dimension split into two dimensions with lengths given by `dims` (data is shared).
    arraynd<T, 2> view_subdivided_axis(int64 idim, const std::array<int64, 2>& dims);              ///< Create a view with the single dimension split into two dimensions with lengths given by `dims` (data is shared).
    arraynd<T, 2> copy_subdivided_axis(int64 idim, const std::array<int64, 2>& dims) const;        ///< Create a deep copy with the single dimension split into two dimensions with lengths given by `dims` (data is copied).

private:
    arraynd(const arraynd<T, 2>& rhs, int64 index, bool readonly);
    arraynd(const arraynd<T, 1>& rhs, range r, bool readonly);
    arraynd(const arraynd<T, 1>& rhs, bool is_view, bool readonly);
};


// n-dimensional array aliases

template<typename T> using array1d = arraynd<T, 1>;
template<typename T> using array2d = arraynd<T, 2>;
template<typename T> using array3d = arraynd<T, 3>;
template<typename T> using array4d = arraynd<T, 4>;
template<typename T> using array5d = arraynd<T, 5>;
template<typename T> using array6d = arraynd<T, 6>;
template<typename T> using array7d = arraynd<T, 7>;
template<typename T> using array8d = arraynd<T, 8>;
template<typename T> using array9d = arraynd<T, 9>;


// n-dimensional array (ndims > 1) implementation

template<typename T, int64 ndims>
arraynd<T, ndims>::arraynd()
    : arraynd_base<T, ndims>()
{
}


template<typename T, int64 ndims>
arraynd<T, ndims>::arraynd(const std::array<int64, ndims>& dims, const T& value)
    : arraynd_base<T, ndims>(dims, value)
{
}


template<typename T, int64 ndims>
arraynd<T, ndims>::arraynd(const std::array<int64, ndims>& dims, const std::vector<T>& data)
    : arraynd_base<T, ndims>(dims, data)
{
}


template<typename T, int64 ndims>
arraynd<T, ndims>::arraynd(const std::array<int64, ndims>& dims, std::function<T(const std::array<int64, ndims>& indices)> func)
    : arraynd_base<T, ndims>(dims, func)
{
}

    
template<typename T, int64 ndims>
const arraynd<T, ndims-1> arraynd<T, ndims>::operator[](int64 index) const
{
    return arraynd<T, ndims-1>(*this, index, true);
}


template<typename T, int64 ndims>
arraynd<T, ndims-1> arraynd<T, ndims>::operator[](int64 index)
{
    return arraynd<T, ndims-1>(*this, index, this->is_readonly());
}


template<typename T, int64 ndims>
const arraynd<T, ndims> arraynd<T, ndims>::operator[](range r) const
{
    return arraynd<T, ndims>(*this, r, true);
}


template<typename T, int64 ndims>
arraynd<T, ndims> arraynd<T, ndims>::operator[](range r)
{
    return arraynd<T, ndims>(*this, r, this->is_readonly());
}


template<typename T, int64 ndims>
const arraynd<T, ndims> arraynd<T, ndims>::view() const
{
    return arraynd<T, ndims>(*this, true, true);
}


template<typename T, int64 ndims>
arraynd<T, ndims> arraynd<T, ndims>::view()
{
    return arraynd<T, ndims>(*this, true, this->is_readonly());
}


template<typename T, int64 ndims>
arraynd<T, ndims> arraynd<T, ndims>::copy() const
{
    return arraynd<T, ndims>(*this, false, false);
}


template<typename T, int64 ndims>
const arraynd<T, ndims> arraynd<T, ndims>::view_transposed() const
{
    auto arr = view();
    this->transpose(arr);
    return arr;
}


template<typename T, int64 ndims>
arraynd<T, ndims> arraynd<T, ndims>::view_transposed()
{
    auto arr = view();
    this->transpose(arr);
    return arr;
}


template<typename T, int64 ndims>
arraynd<T, ndims> arraynd<T, ndims>::copy_transposed() const
{
    auto arr = view();
    this->transpose(arr);
    return arr.copy();
}


template<typename T, int64 ndims>
const arraynd<T, ndims> arraynd<T, ndims>::view_swapped_axes(int64 idim0, int64 idim1) const
{
    auto arr = view();
    this->swap_axes(arr, idim0, idim1);
    return arr;
}


template<typename T, int64 ndims>
arraynd<T, ndims> arraynd<T, ndims>::view_swapped_axes(int64 idim0, int64 idim1)
{
    auto arr = view();
    this->swap_axes(arr, idim0, idim1);
    return arr;
}


template<typename T, int64 ndims>
arraynd<T, ndims> arraynd<T, ndims>::copy_swapped_axes(int64 idim0, int64 idim1) const
{
    auto arr = view();
    this->swap_axes(arr, idim0, idim1);
    return arr.copy();
}

    
template<typename T, int64 ndims>
const arraynd<T, ndims + 1> arraynd<T, ndims>::view_subdivided_axis(int64 idim, const std::array<int64, 2>& dims) const
{
    auto arr = arraynd<T, ndims + 1>();
    this->subdivide_axis(*this, arr, idim, dims);
    return arr;
}


template<typename T, int64 ndims>
arraynd<T, ndims + 1> arraynd<T, ndims>::view_subdivided_axis(int64 idim, const std::array<int64, 2>& dims)
{
    auto arr = arraynd<T, ndims + 1>();
    this->subdivide_axis(*this, arr, idim, dims);
    return arr;
}


template<typename T, int64 ndims>
arraynd<T, ndims + 1> arraynd<T, ndims>::copy_subdivided_axis(int64 idim, const std::array<int64, 2>& dims) const
{
    auto arr0 = copy();
    return arr0.view_subdivided_axis(idim, dims);
}


template<typename T, int64 ndims>
const arraynd<T, ndims - 1> arraynd<T, ndims>::view_absorbed_axis(int64 idim) const
{
    auto arr = arraynd<T, ndims - 1>(std::array<int64, ndims - 1>(), std::vector<T>());
    this->absorb_axis(*this, arr, idim);
    return arr;
}


template<typename T, int64 ndims>
arraynd<T, ndims - 1> arraynd<T, ndims>::view_absorbed_axis(int64 idim)
{
    auto arr = arraynd<T, ndims - 1>(std::array<int64, ndims - 1>(), std::vector<T>());
    this->absorb_axis(*this, arr, idim);
    return arr;
}


template<typename T, int64 ndims>
arraynd<T, ndims - 1> arraynd<T, ndims>::copy_absorbed_axis(int64 idim) const
{
    auto arr0 = copy();
    return arr0.view_absorbed_axis(idim);
}


template<typename T, int64 ndims>
arraynd<T, ndims>::arraynd(const arraynd<T, ndims+1>& rhs, int64 index, bool readonly)
    : arraynd_base<T, ndims>(rhs, index, readonly)
{
}


template<typename T, int64 ndims>
arraynd<T, ndims>::arraynd(const arraynd<T, ndims>& rhs, range r, bool readonly)
    : arraynd_base<T, ndims>(rhs, r, readonly)
{
}

    
template<typename T, int64 ndims>
arraynd<T, ndims>::arraynd(const arraynd<T, ndims>& rhs, bool is_view, bool readonly)
    : arraynd_base<T, ndims>(rhs, is_view, readonly)
{
}

    
// 1-dimensional array implementation

template<typename T>
arraynd<T, 1>::arraynd()
    : arraynd_base<T, 1>()
{
}


template<typename T>
arraynd<T, 1>::arraynd(const std::array<int64, 1>& dims, const T& value)
    : arraynd_base<T, 1>(dims, value)
{
}


template<typename T>
arraynd<T, 1>::arraynd(const std::array<int64, 1>& dims, const std::vector<T>& data)
    : arraynd_base<T, 1>(dims, data)
{
}


template<typename T>
arraynd<T, 1>::arraynd(const std::array<int64, 1>& dims, std::function<T(const std::array<int64, 1>& indices)> func)
    : arraynd_base<T, 1>(dims, func)
{
}


template<typename T>
const T& arraynd<T, 1>::operator[](int64 index) const
{
    return arraynd_base<T, 1>::data()[arraynd_base<T, 1>::offset() + index*arraynd_base<T, 1>::strides()[0]];
}


template<typename T>
T& arraynd<T, 1>::operator[](int64 index)
{
    if (this->is_readonly()) throw std::logic_error("Attempt to obtain a non-const reference to readonly multidimensional array data");
    return arraynd_base<T, 1>::data()[arraynd_base<T, 1>::offset() + index*arraynd_base<T, 1>::strides()[0]];
}


template<typename T>
const arraynd<T, 1> arraynd<T, 1>::operator[](range r) const
{
    return arraynd<T, 1>(*this, r, true);
}


template<typename T>
arraynd<T, 1> arraynd<T, 1>::operator[](range r)
{
    return arraynd<T, 1>(*this, r, this->is_readonly());
}


template<typename T>
const arraynd<T, 1> arraynd<T, 1>::view() const
{
    return arraynd<T, 1>(*this, true, true);
}


template<typename T>
arraynd<T, 1> arraynd<T, 1>::view()
{
    return arraynd<T, 1>(*this, true, this->is_readonly());
}


template<typename T>
arraynd<T, 1> arraynd<T, 1>::copy() const
{
    return arraynd<T, 1>(*this, false, false);
}


template<typename T>
const arraynd<T, 2> arraynd<T, 1>::view_subdivided_axis(int64 idim, const std::array<int64, 2>& dims) const
{
    auto arr = arraynd<T, 2>();
    this->subdivide_axis(*this, arr, idim, dims);
    return arr;
}


template<typename T>
arraynd<T, 2> arraynd<T, 1>::view_subdivided_axis(int64 idim, const std::array<int64, 2>& dims)
{
    auto arr = arraynd<T, 2>();
    this->subdivide_axis(*this, arr, idim, dims);
    return arr;
}


template<typename T>
arraynd<T, 2> arraynd<T, 1>::copy_subdivided_axis(int64 idim, const std::array<int64, 2>& dims) const
{
    auto arr0 = copy();
    return arr0.view_subdivided_axis(idim, dims);
}


template<typename T>
arraynd<T, 1>::arraynd(const arraynd<T, 2>& rhs, int64 index, bool readonly)
    : arraynd_base<T, 1>(rhs, index, readonly)
{
}


template<typename T>
arraynd<T, 1>::arraynd(const arraynd<T, 1>& rhs, range r, bool readonly)
    : arraynd_base<T, 1>(rhs, r, readonly)
{
}

    
template<typename T>
arraynd<T, 1>::arraynd(const arraynd<T, 1>& rhs, bool is_view, bool readonly)
    : arraynd_base<T, 1>(rhs, is_view, readonly)
{
}


// multi-dimensional array operations

template<typename T, int64 ndims>
bool aligned(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    auto okay = true;
    for (int64 idim = 0; okay && idim < ndims; ++idim) {
        okay = (lhs.dims()[idim] == rhs.dims()[idim]);
    }
    return okay;
}


template<typename T, int64 ndims>
arraynd<T, ndims> operator+(const arraynd<T, ndims>& rhs)
{
    return rhs.copy();
}


template<typename T, int64 ndims>
arraynd<T, ndims> operator-(const arraynd<T, ndims>& rhs)
{
    return arraynd<T, ndims>(rhs.dims(), [&rhs](const std::array<int64, ndims>& indices) {
        return -rhs(indices);
    });
}


template<typename T, int64 ndims>
arraynd<T, ndims> operator+(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply + operator to multidimensional arrays with inconsistent dimensions");    
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) + rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<T, ndims> operator+(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) + rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<T, ndims> operator+(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<T, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs + rhs(indices);
    });
}


template<typename T, int64 ndims>
arraynd<T, ndims> operator-(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply - operator to multidimensional arrays with inconsistent dimensions");    
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) - rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<T, ndims> operator-(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) - rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<T, ndims> operator-(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<T, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs - rhs(indices);
    });
}


template<typename T, int64 ndims>
arraynd<T, ndims> operator*(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply * operator to multidimensional arrays with inconsistent dimensions");    
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices)*rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<T, ndims> operator*(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices)*rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<T, ndims> operator*(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<T, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs*rhs(indices);
    });
}


template<typename T, int64 ndims>
arraynd<T, ndims> operator/(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply / operator to multidimensional arrays with inconsistent dimensions");    
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices)/rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<T, ndims> operator/(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices)/rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<T, ndims> operator/(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<T, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs/rhs(indices);
    });
}


template<int64 ndims>
arraynd<bool, ndims> operator!(const arraynd<bool, ndims>& arr)
{
    return arraynd<bool, ndims>(arr.dims(), [&arr](const std::array<int64, ndims>& indices) {
        return !arr(indices);
    });
}


/**
 * @brief Returns `true` if all elements of `arr` are `true`.
 */
template<int64 ndims>
bool all(const arraynd<bool, ndims>& arr)
{
    bool conjunction = true;
    arr.traverse([&conjunction](const std::array<int64, ndims>& indices, const bool& value) {
        conjunction = (conjunction && value);
        return conjunction;
    });
    return conjunction;
}


/**
 * @brief Returns `true` if any element of `arr` is `true`.
 */
template<int64 ndims>
bool any(const arraynd<bool, ndims>& arr)
{
    bool disjunction = false;
    arr.traverse([&disjunction](const std::array<int64, ndims>& indices, const bool& value) {
        disjunction = (disjunction || value);
        return !disjunction;
    });
    return disjunction;
}


template<typename T, int64 ndims>
arraynd<bool, ndims> operator<(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply < operator to multidimensional arrays with inconsistent dimensions");
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) < rhs(indices);
    });
}


template<typename T, int64 ndims>
arraynd<bool, ndims> operator>(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply > operator to multidimensional arrays with inconsistent dimensions");
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) > rhs(indices);
    });
}


template<typename T, int64 ndims>
arraynd<bool, ndims> operator<=(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply <= operator to multidimensional arrays with inconsistent dimensions");
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) <= rhs(indices);
    });
}


template<typename T, int64 ndims>
arraynd<bool, ndims> operator>=(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply >= operator to multidimensional arrays with inconsistent dimensions");
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) >= rhs(indices);
    });
}


template<typename T, int64 ndims>
arraynd<bool, ndims> operator==(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply == operator to multidimensional arrays with inconsistent dimensions");
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) == rhs(indices);
    });
}


template<typename T, int64 ndims>
arraynd<bool, ndims> operator!=(const arraynd<T, ndims>& lhs, const arraynd<T, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply != operator to multidimensional arrays with inconsistent dimensions");
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) != rhs(indices);
    });
}


template<int64 ndims>
arraynd<bool, ndims> operator&&(const arraynd<bool, ndims>& lhs, const arraynd<bool, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply && operator to multidimensional arrays with inconsistent dimensions");
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) && rhs(indices);
    });
}


template<int64 ndims>
arraynd<bool, ndims> operator||(const arraynd<bool, ndims>& lhs, const arraynd<bool, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to apply || operator to multidimensional arrays with inconsistent dimensions");
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) || rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator<(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) < rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator>(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) > rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator<=(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) <= rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator>=(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) >= rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator==(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) == rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator!=(const arraynd<T, ndims>& lhs, const U& rhs)
{
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) != rhs;
    });
}


template<int64 ndims>
arraynd<bool, ndims> operator&&(const arraynd<bool, ndims>& lhs, const bool& rhs)
{
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) && rhs;
    });
}


template<int64 ndims>
arraynd<bool, ndims> operator||(const arraynd<bool, ndims>& lhs, const bool& rhs)
{
    return arraynd<bool, ndims>(lhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs(indices) || rhs;
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator<(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<bool, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs < rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator>(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<bool, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs > rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator<=(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<bool, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs <= rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator>=(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<bool, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs >= rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator==(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<bool, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs == rhs(indices);
    });
}


template<typename T, int64 ndims, typename U>
arraynd<bool, ndims> operator!=(const U& lhs, const arraynd<T, ndims>& rhs)
{
    return arraynd<bool, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs != rhs(indices);
    });
}


template<int64 ndims>
arraynd<bool, ndims> operator&&(const bool& lhs, const arraynd<bool, ndims>& rhs)
{
    return arraynd<bool, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs && rhs(indices);
    });
}


template<int64 ndims>
arraynd<bool, ndims> operator||(const bool& lhs, const arraynd<bool, ndims>& rhs)
{
    return arraynd<bool, ndims>(rhs.dims(), [&lhs, &rhs](const std::array<int64, ndims>& indices) {
        return lhs || rhs(indices);
    });
}


/**
 * @brief Returns a multidimensional array with the elements of `arr` converted
 *        to type `T`.
 */
template<typename T, int64 ndims, typename U>
arraynd<T, ndims> to(const arraynd<U, ndims>& arr)
{
    return arraynd<T, ndims>(arr.dims(), [&arr](const std::array<int64, ndims>& indices) {
        return static_cast<T>(arr(indices));
    });
}


/**
 * @brief Returns a multidimensional array similar to `arr` but with elements
 *        for which `selection(indices)` is `true` replaced with `rhs`.
 */
template<typename T, int64 ndims, typename U>
arraynd<T, ndims> replace(const arraynd<T, ndims>& lhs, const arraynd<bool, ndims>& selection, const U& rhs)
{
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &selection, &rhs](const std::array<int64, ndims>& indices) {
        return selection(indices) ? rhs : lhs(indices);
    });
}


/**
 * @brief Returns a multidimensional array similar to `arr` but with elements
 *        for which `selection(indices)` is `true` replaced with the
 *        corresponding elements of `rhs`.
 */
template<typename T, int64 ndims, typename U>
arraynd<T, ndims> replace(const arraynd<T, ndims>& lhs, const arraynd<bool, ndims>& selection, const arraynd<U, ndims>& rhs)
{
    if (!aligned(lhs, rhs)) throw std::domain_error("Attempt to replace values using multidimensional arrays with inconsistent dimensions");
    return arraynd<T, ndims>(lhs.dims(), [&lhs, &selection, &rhs](const std::array<int64, ndims>& indices) {
        return selection(indices) ? rhs(indices) : lhs(indices);
    });
}


template<typename T, int64 ndims>
std::ostream& operator<<(std::ostream& os, const arraynd<T, ndims>& rhs)
{
    auto indices = std::array<int64, ndims>();
    for (int64 idim = 0; idim < ndims; ++idim) {
        indices[idim] = 0;
    }
    int64 offset = rhs.offset();
    int64 idim = 0;
    os << "{";
    while (idim >= 0) {
        if (indices[idim] < rhs.dims()[idim]) {
            // The index is not beyond the last element on the current axis.
            // Continue outputting the current sub-array.
            if (indices[idim] > 0) {
                // This is not the first element on the current axis.
                // Output a delimiter.
                os << ", ";
            }
            if (idim == ndims - 1) {
                // The current axis is the last one.
                // Output the current element and advance the index.
                os << rhs.data()[offset];
                ++indices[idim];
                offset += rhs.strides()[idim];
            }
            else {
                // The current axis is not the last one.
                // Start outputting a new sub-array.
                ++idim;
                os << "{";
            }
        }
        else {
            // The index is beyond the last element on the current axis.
            // Finish outputting the current sub-array.
            indices[idim] = 0;
            offset -= rhs.dims()[idim]*rhs.strides()[idim];
            --idim;
            if (idim >= 0) {
                ++indices[idim];
                offset += rhs.strides()[idim];
                os << "}";
            }
        }
    }
    os << "}";
    return os;
}


}  // namespace


namespace std {
/**
 * @brief Compares 1D multidimensional arrays lexiographically (instead of
 *        element-wise as in `operator()`), supporting their use in `std::set`
 *        and other STL containers.
 */
template<typename T>
struct less<sydevs::array1d<T>>
{
    bool operator()(const sydevs::array1d<T>& lhs, const sydevs::array1d<T>& rhs) const {
        return std::lexicographical_compare(lhs.data(), lhs.data() + lhs.size(), 
                                            rhs.data(), rhs.data() + rhs.size());
    }
};
}


#endif
