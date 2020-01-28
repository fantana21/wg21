//==================================================================================================
//  File:       transpose_engine.hpp
//
//  Summary:    This header defines an engine that acts as a "view" of matrix transpose.
//==================================================================================================
//
#ifndef LINEAR_ALGEBRA_TRANSPOSE_ENGINE_HPP_DEFINED
#define LINEAR_ALGEBRA_TRANSPOSE_ENGINE_HPP_DEFINED

namespace STD_LA {
//==================================================================================================
//  Matrix transpose engine, meant to act as a "view" of a matrix transpose in expressions, in
//  order to help avoid unnecessary allocation and element copying.
//==================================================================================================
//
template<class ET, class MCT>
class transpose_engine
{
    static_assert(is_matrix_engine_v<ET>);
    static_assert(is_matrix_engine_tag<MCT>);

  public:
    //- Types
    //
    using engine_category = MCT;
    using element_type    = typename ET::element_type;
    using value_type      = typename ET::value_type;
    using pointer         = detail::noe_pointer_t<ET, MCT>;
    using const_pointer   = typename ET::const_pointer;
    using reference       = detail::noe_reference_t<ET, MCT>;
    using const_reference = typename ET::const_reference;
    using difference_type = typename ET::difference_type;
    using size_type       = typename ET::size_type;
    using size_tuple      = typename ET::size_tuple;

#ifdef LA_USE_MDSPAN
    using span_type       = detail::noe_mdspan_transpose_t<detail::noe_mdspan_t<ET, MCT>>;
    using const_span_type = detail::noe_mdspan_transpose_t<detail::noe_const_mdspan_t<ET, MCT>>;
#endif

    //- Construct/copy/destroy
    //
    ~transpose_engine() noexcept = default;

    constexpr transpose_engine();
    constexpr transpose_engine(transpose_engine&&) noexcept = default;
    constexpr transpose_engine(transpose_engine const&) = default;

    constexpr transpose_engine&     operator =(transpose_engine&&) noexcept = default;
    constexpr transpose_engine&     operator =(transpose_engine const&) = default;

    template<class ET2, detail::enable_if_convertible_engine<ET2, ET> = true>
    constexpr transpose_engine&     operator =(ET2 const& rhs);
    template<class U, detail::enable_if_writable<ET, ET> = true>
    constexpr transpose_engine&     operator =(initializer_list<initializer_list<U>> list);

    //- Capacity
    //
    constexpr size_type     columns() const noexcept;
    constexpr size_type     rows() const noexcept;
    constexpr size_tuple    size() const noexcept;

    constexpr size_type     column_capacity() const noexcept;
    constexpr size_type     row_capacity() const noexcept;
    constexpr size_tuple    capacity() const noexcept;

    //- Element access
    //
    constexpr reference     operator ()(size_type i, size_type j) const;

    //- Data access
    //
#ifdef LA_USE_MDSPAN
    constexpr span_type     span() const noexcept;
#endif

    //- Modifiers
    //
    constexpr void      swap(transpose_engine& rhs);

  private:
    template<class ET2, class OT2>  friend class matrix;
    using referent_type = detail::noe_referent_t<ET, MCT>;

    referent_type*      mp_other;

    constexpr transpose_engine(referent_type& eng);
};

//------------------------
//- Construct/copy/destroy
//
template<class ET, class MCT> constexpr
transpose_engine<ET, MCT>::transpose_engine()
:   mp_other(nullptr)
{}

template<class ET, class VCT>
template<class ET2, detail::enable_if_convertible_engine<ET2, ET>> constexpr
transpose_engine<ET,VCT>&
transpose_engine<ET,VCT>::operator =(ET2 const& rhs)
{
    detail::check_source_engine_size(rhs, mp_other->columns(), mp_other->rows());
    detail::assign_from_matrix_engine(*this, rhs);
    return *this;
}

template<class ET, class VCT>
template<class U, detail::enable_if_writable<ET, ET>> constexpr
transpose_engine<ET,VCT>&
transpose_engine<ET,VCT>::operator =(initializer_list<initializer_list<U>> rhs)
{
    detail::check_source_init_list(rhs, mp_other->columns(), mp_other->rows());
    detail::assign_from_matrix_list(*this, rhs);
    return *this;
}

//----------
//- Capacity
//
template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_type
transpose_engine<ET, MCT>::columns() const noexcept
{
    return mp_other->rows();
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_type
transpose_engine<ET, MCT>::rows() const noexcept
{
    return mp_other->columns();
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_tuple
transpose_engine<ET, MCT>::size() const noexcept
{
    return size_tuple(mp_other->columns(), mp_other->rows());
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_type
transpose_engine<ET, MCT>::column_capacity() const noexcept
{
    return mp_other->rows();
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_type
transpose_engine<ET, MCT>::row_capacity() const noexcept
{
    return mp_other->columns();
}

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::size_tuple
transpose_engine<ET, MCT>::capacity() const noexcept
{
    return size_tuple(mp_other->columns(), mp_other->rows());
}

//----------------
//- Element access
//
template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::reference
transpose_engine<ET, MCT>::operator ()(size_type i, size_type j) const
{
    return (*mp_other)(j, i);
}

//-------------
//- Data access
//
#ifdef LA_USE_MDSPAN

template<class ET, class MCT> constexpr
typename transpose_engine<ET, MCT>::span_type
transpose_engine<ET, MCT>::span() const noexcept
{
    return detail::noe_mdspan_transpose(mp_other->span());
}

#endif
//-----------
//- Modifiers
//
template<class ET, class MCT> constexpr
void
transpose_engine<ET, MCT>::swap(transpose_engine& rhs)
{
    std::swap(mp_other, rhs.mp_other);
}

template<class ET, class MCT> constexpr
transpose_engine<ET, MCT>::transpose_engine(referent_type& eng)
:   mp_other(&eng)
{}

}       //- STD_LA namespace
#endif  //- LINEAR_ALGEBRA_TRANSPOSE_ENGINE_HPP_DEFINED
