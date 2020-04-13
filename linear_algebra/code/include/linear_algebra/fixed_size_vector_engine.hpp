//==================================================================================================
//  File:       fixed_size_vector_engine.hpp
//
//  Summary:    This header defines a fixed-size vector engine.  In this context, fixed-size
//              means that the extent of such objects are known at compile-time.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_FIXED_SIZE_VECTOR_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_FIXED_SIZE_VECTOR_ENGINE_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Fixed-size, fixed-capacity vector engine.
//==================================================================================================
//
template<class T, size_t N>
class fs_vector_engine
{
    static_assert(N >= 1);

  public:
    using engine_category = initable_vector_engine_tag;
    using element_type    = T;
    using value_type      = remove_cv_t<T>;
    using pointer         = element_type*;
    using const_pointer   = element_type const*;
    using reference       = element_type&;
    using const_reference = element_type const&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;

#ifdef LA_USE_MDSPAN
    using span_type       = mdspan<element_type, N>;
    using const_span_type = mdspan<element_type const, N>;
#endif

    //- Construct/copy/destroy
    //
    ~fs_vector_engine() noexcept = default;

    constexpr fs_vector_engine();
    constexpr fs_vector_engine(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine(fs_vector_engine const&) = default;
    template<class T2, size_t N2>
    constexpr fs_vector_engine(fs_vector_engine<T2, N2> const& src);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    constexpr fs_vector_engine(ET2 const& src);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    constexpr fs_vector_engine(initializer_list<T2> rhs);

    constexpr fs_vector_engine&     operator =(fs_vector_engine&&) noexcept = default;
    constexpr fs_vector_engine&     operator =(fs_vector_engine const&) = default;
    template<class T2, size_t N2>
    constexpr fs_vector_engine&     operator =(fs_vector_engine<T2, N2> const& rhs);
    template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T> = true>
    constexpr fs_vector_engine&     operator =(ET2 const& rhs);
    template<class T2, detail::enable_if_convertible_element<T2,T> = true>
    constexpr fs_vector_engine&     operator =(initializer_list<T2> rhs);

    //- Capacity
    //
    static constexpr size_type  capacity() noexcept;
    static constexpr size_type  size() noexcept;

    //- Element access
    //
    constexpr reference         operator ()(size_type i);
    constexpr const_reference   operator ()(size_type i) const;

    //- Data access
    //
#ifdef LA_USE_MDSPAN
    constexpr span_type         span() noexcept;
    constexpr const_span_type   span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void  swap(fs_vector_engine& rhs) noexcept;
    constexpr void  swap_elements(size_type i, size_type j) noexcept;

  private:
    template<class T2, size_t N2> friend class fs_vector_engine;

    array<T, N>     ma_elems;

    template<class T2, size_t N2>
    constexpr void  assign(fs_vector_engine<T2, N2> const& rhs);
    template<class ET2>
    constexpr void  assign(ET2 const& rhs);
    template<class T2>
    constexpr void  assign(initializer_list<T2> rhs);
};

//------------------------
//- Construct/copy/destroy
//
template<class T, size_t N> constexpr
fs_vector_engine<T,N>::fs_vector_engine()
:   ma_elems()
{}

template<class T, size_t N>
template<class T2, size_t N2> constexpr
fs_vector_engine<T,N>::fs_vector_engine(fs_vector_engine<T2, N2> const& rhs)
:   ma_elems()
{
    assign(rhs);
}

template<class T, size_t N>
template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T>> constexpr
fs_vector_engine<T,N>::fs_vector_engine(ET2 const& rhs)
:   ma_elems()
{
    assign(rhs);
}

template<class T, size_t N>
template<class T2, detail::enable_if_convertible_element<T2,T>> constexpr
fs_vector_engine<T,N>::fs_vector_engine(initializer_list<T2> rhs)
:   ma_elems()
{
    assign(rhs);
}

template<class T, size_t N>
template<class T2, size_t N2> constexpr
fs_vector_engine<T,N>&
fs_vector_engine<T,N>::operator =(fs_vector_engine<T2,N2> const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, size_t N>
template<class ET2, detail::enable_if_engine_has_convertible_element<ET2,T>> constexpr
fs_vector_engine<T,N>&
fs_vector_engine<T,N>::operator =(ET2 const& rhs)
{
    assign(rhs);
    return *this;
}

template<class T, size_t N>
template<class T2, detail::enable_if_convertible_element<T2,T>> constexpr
fs_vector_engine<T,N>&
fs_vector_engine<T,N>::operator =(initializer_list<T2> rhs)
{
    assign(rhs);
    return *this;
}

//----------
//- Capacity
//
template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::capacity() noexcept
{
    return N;
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::size_type
fs_vector_engine<T,N>::size() noexcept
{
    return N;
}

//----------------
//- Element access
//
template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::reference
fs_vector_engine<T,N>::operator ()(size_type i)
{
    return ma_elems[i];
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::const_reference
fs_vector_engine<T,N>::operator ()(size_type i) const
{
    return ma_elems[i];
}

//-------------
//- Data access
//
#ifdef LA_USE_MDSPAN

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::span_type
fs_vector_engine<T,N>::span() noexcept
{
    return span_type(ma_elems.data());
}

template<class T, size_t N> constexpr
typename fs_vector_engine<T,N>::const_span_type
fs_vector_engine<T,N>::span() const noexcept
{
    return const_span_type(ma_elems.data());
}

#endif
//-----------
//- Modifiers
//
template<class T, size_t N> constexpr
void
fs_vector_engine<T,N>::swap(fs_vector_engine& rhs) noexcept
{
    if (&rhs != this)
    {
        for (size_t i = 0;  i < N;  ++i)
        {
            detail::la_swap(ma_elems[i], rhs.ma_elems[i]);
        }
    }
}

template<class T, size_t N> constexpr
void
fs_vector_engine<T,N>::swap_elements(size_type i, size_type j) noexcept
{
    detail::la_swap(ma_elems[i], ma_elems[j]);
}

//------------------------
//- Private implementation
//
template<class T, size_t N>
template<class T2, size_t N2> constexpr
void
fs_vector_engine<T,N>::assign(fs_vector_engine<T2,N2> const& rhs)
{
    static_assert(N2 == N);

    for (size_type di = 0;  di < N;  ++di)
    {
        ma_elems[di] = static_cast<T>(rhs.ma_elems[di]);
    }
}

template<class T, size_t N>
template<class ET2> constexpr
void
fs_vector_engine<T,N>::assign(ET2 const& rhs)
{
    static_assert(is_vector_engine_v<ET2>);
    detail::check_source_engine_size(rhs, N);
    detail::assign_from_vector_engine(*this, rhs);
}

template<class T, size_t N>
template<class T2> constexpr
void
fs_vector_engine<T,N>::assign(initializer_list<T2> rhs)
{
    detail::check_source_init_list(rhs, N);
    detail::assign_from_vector_list(*this, rhs);
}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_FIXED_SIZE_VECTOR_ENGINE_HPP_DEFINED