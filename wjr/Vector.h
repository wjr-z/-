#pragma once
#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <vector>

_STD_BEGIN

template <class _Ty, class _Alloc = allocator<_Ty>>
class Vector { // varying size array of values
private:
    template <class>
    friend class _Vb_val;
    friend _Tidy_guard<Vector>;

    using _Alty = _Rebind_alloc_t<_Alloc, _Ty>;
    using _Alty_traits = allocator_traits<_Alty>;

public:

    using value_type = _Ty;
    using allocator_type = _Alloc;
    using pointer = typename _Alty_traits::pointer;
    using const_pointer = typename _Alty_traits::const_pointer;
    using reference = _Ty&;
    using const_reference = const _Ty&;
    using size_type = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;

private:
    using _Scary_val = _Vector_val<conditional_t<_Is_simple_alloc_v<_Alty>, _Simple_types<_Ty>,
        _Vec_iter_types<_Ty, size_type, difference_type, pointer, const_pointer, _Ty&, const _Ty&>>>;

public:
    using iterator = _Vector_iterator<_Scary_val>;
    using const_iterator = _Vector_const_iterator<_Scary_val>;
    using reverse_iterator = _STD reverse_iterator<iterator>;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

    _CONSTEXPR20_CONTAINER Vector() noexcept
        : _Mypair(_Zero_then_variadic_args_t{}) {
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
    }

    _CONSTEXPR20_CONTAINER explicit Vector(const _Alloc& _Al) noexcept : _Mypair(_One_then_variadic_args_t{}, _Al) {
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
    }

private:
    template <class _Ty2>
    _CONSTEXPR20_CONTAINER void _Construct_n_copies_of_ty(_CRT_GUARDOVERFLOW const size_type _Count, const _Ty2& _Val) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        auto& _My_data = _Mypair._Myval2;
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _My_data);
        if (_Count != 0) {
            _Buy_nonzero(_Count);
            _Tidy_guard<Vector> _Guard{ this };
            _My_data._Mylast = _Ufill(_My_data._Myfirst, _Count, _Val);
            _Guard._Target = nullptr;
        }

        _Proxy._Release();
    }

public:
    _CONSTEXPR20_CONTAINER explicit Vector(_CRT_GUARDOVERFLOW const size_type _Count, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        _Construct_n_copies_of_ty(_Count, _Value_init_tag{});
    }

    _CONSTEXPR20_CONTAINER Vector(
        _CRT_GUARDOVERFLOW const size_type _Count, const _Ty& _Val, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        _Construct_n_copies_of_ty(_Count, _Val);
    }

private:
    template <class _Iter>
    _CONSTEXPR20_CONTAINER void _Range_construct_or_tidy(_Iter _First, _Iter _Last, input_iterator_tag) {
        _Tidy_guard<Vector> _Guard{ this };
        for (; _First != _Last; ++_First) {
            emplace_back(*_First); // performance note: emplace_back()'s strong guarantee is unnecessary here
        }

        _Guard._Target = nullptr;
    }

    template <class _Iter>
    _CONSTEXPR20_CONTAINER void _Range_construct_or_tidy(_Iter _First, _Iter _Last, forward_iterator_tag) {
        const auto _Count = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));
        if (_Count != 0) {
            _Buy_nonzero(_Count);
            _Tidy_guard<Vector> _Guard{ this };
            auto& _My_data = _Mypair._Myval2;
            _My_data._Mylast = _Ucopy(_First, _Last, _My_data._Myfirst);
            _Guard._Target = nullptr;
        }
    }

public:
    template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20_CONTAINER Vector(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Adl_verify_range(_First, _Last);
        _Range_construct_or_tidy(_Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER Vector(initializer_list<_Ty> _Ilist, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Range_construct_or_tidy(_Ilist.begin(), _Ilist.end(), random_access_iterator_tag{});
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER Vector(const Vector& _Right)
        : _Mypair(_One_then_variadic_args_t{}, _Alty_traits::select_on_container_copy_construction(_Right._Getal())) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        auto& _My_data = _Mypair._Myval2;
        const auto& _Right_data = _Right._Mypair._Myval2;
        const pointer _Rightfirst = _Right_data._Myfirst;
        const pointer _Rightlast = _Right_data._Mylast;
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _My_data);
        if (_Rightfirst != _Rightlast) {
            _Buy_raw(static_cast<size_type>(_Rightlast - _Rightfirst));
            _Tidy_guard<Vector> _Guard{ this };
            _My_data._Mylast = _Ucopy(_Rightfirst, _Rightlast, _My_data._Myfirst);
            _Guard._Target = nullptr;
        }

        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER Vector(const Vector& _Right, const _Alloc& _Al) : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        auto& _My_data = _Mypair._Myval2;
        const auto& _Right_data = _Right._Mypair._Myval2;
        const pointer _Rightfirst = _Right_data._Myfirst;
        const pointer _Rightlast = _Right_data._Mylast;
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _My_data);
        if (_Rightfirst != _Rightlast) {
            _Buy_raw(static_cast<size_type>(_Rightlast - _Rightfirst));
            _Tidy_guard<Vector> _Guard{ this };
            _My_data._Mylast = _Ucopy(_Rightfirst, _Rightlast, _My_data._Myfirst);
            _Guard._Target = nullptr;
        }

        _Proxy._Release();
    }

private:
    _CONSTEXPR20_CONTAINER void _Move_construct(Vector& _Right, true_type) noexcept {
        // move from _Right, stealing its contents
        _Mypair._Myval2._Take_contents(_Right._Mypair._Myval2);
    }

    _CONSTEXPR20_CONTAINER void _Move_construct(Vector& _Right, false_type) {
        // move from _Right, possibly moving its contents
        if constexpr (!_Alty_traits::is_always_equal::value) {
            if (_Getal() != _Right._Getal()) {
                const auto& _Right_data = _Right._Mypair._Myval2;
                const pointer _Rightfirst = _Right_data._Myfirst;
                const pointer _Rightlast = _Right_data._Mylast;
                if (_Rightfirst != _Rightlast) {
                    _Buy_raw(static_cast<size_type>(_Rightlast - _Rightfirst));
                    _Tidy_guard<Vector> _Guard{ this };
                    auto& _My_data = _Mypair._Myval2;
                    _My_data._Mylast = _Umove(_Rightfirst, _Rightlast, _My_data._Myfirst);
                    _Guard._Target = nullptr;
                }
                return;
            }
        }

        _Move_construct(_Right, true_type{});
    }

public:
    _CONSTEXPR20_CONTAINER Vector(Vector&& _Right) noexcept
        : _Mypair(_One_then_variadic_args_t{}, _STD move(_Right._Getal()),
            _STD exchange(_Right._Mypair._Myval2._Myfirst, nullptr),
            _STD exchange(_Right._Mypair._Myval2._Mylast, nullptr),
            _STD exchange(_Right._Mypair._Myval2._Myend, nullptr)) {
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
        _Mypair._Myval2._Swap_proxy_and_iterators(_Right._Mypair._Myval2);
    }

    _CONSTEXPR20_CONTAINER Vector(Vector&& _Right, const _Alloc& _Al) noexcept(
        _Alty_traits::is_always_equal::value) // strengthened
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Move_construct(_Right, typename _Alty_traits::is_always_equal::type{});
        _Proxy._Release();
    }

private:
    _CONSTEXPR20_CONTAINER void _Move_assign(Vector& _Right, _Equal_allocators) noexcept {
        _Tidy();
        _Pocma(_Getal(), _Right._Getal());
        _Mypair._Myval2._Take_contents(_Right._Mypair._Myval2);
    }

    _CONSTEXPR20_CONTAINER void _Move_assign(Vector& _Right, _Propagate_allocators) noexcept /* terminates */ {
        _Tidy();

        _Pocma(_Getal(), _Right._Getal());
        _Mypair._Myval2._Take_contents(_Right._Mypair._Myval2);
    }

    _CONSTEXPR20_CONTAINER void _Move_assign(Vector& _Right, _No_propagate_allocators) {
        if (_Getal() == _Right._Getal()) {
            _Move_assign(_Right, _Equal_allocators{});
        }
        else {
            auto& _Right_data = _Right._Mypair._Myval2;
            const pointer _First = _Right_data._Myfirst;
            const pointer _Last = _Right_data._Mylast;
            const auto _Newsize = static_cast<size_type>(_Last - _First);

            auto& _My_data = _Mypair._Myval2;
            pointer& _Myfirst = _My_data._Myfirst;
            pointer& _Mylast = _My_data._Mylast;

            _My_data._Orphan_all();

            const auto _Oldcapacity = static_cast<size_type>(_My_data._Myend - _Myfirst);
            if constexpr (conjunction_v<bool_constant<_Ptr_copy_cat<_Ty*, _Ty*>::_Trivially_copyable>,
                _Uses_default_construct<_Alty, _Ty*, _Ty>, _Uses_default_destroy<_Alty, _Ty*>>) {
                if (_Newsize > _Oldcapacity) {
                    _Clear_and_reserve_geometric(_Newsize);
                }

                _Mylast = _Refancy<pointer>(_Copy_memmove(_Unfancy(_First), _Unfancy(_Last), _Unfancy(_Myfirst)));
            }
            else {
                auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);

                if (_Newsize > _Oldsize) {
                    if (_Newsize > _Oldcapacity) { // reallocate
                        _Clear_and_reserve_geometric(_Newsize);
                        _Oldsize = 0;
                    }

                    const pointer _Mid = _First + _Oldsize;
                    _Move_unchecked(_First, _Mid, _Myfirst);
                    _Mylast = _Umove(_Mid, _Last, _Mylast);
                }
                else {
                    const pointer _Newlast = _Myfirst + _Newsize;
                    _Move_unchecked(_First, _Last, _Myfirst);
                    _Destroy(_Newlast, _Mylast);
                    _Mylast = _Newlast;
                }
            }
        }
    }

public:
    _CONSTEXPR20_CONTAINER Vector& operator=(Vector&& _Right) noexcept(
        noexcept(_Move_assign(_Right, _Choose_pocma<_Alty>{}))) {
        if (this != _STD addressof(_Right)) {
            _Move_assign(_Right, _Choose_pocma<_Alty>{});
        }

        return *this;
    }

    _CONSTEXPR20_CONTAINER ~Vector() noexcept {
        _Tidy();
    }

private:
    template <class... _Valty>
    _CONSTEXPR20_CONTAINER decltype(auto) _Emplace_back_with_unused_capacity(_Valty&&... _Val) {
        // insert by perfectly forwarding into element at end, provide strong guarantee
        auto& _My_data = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;
        _Alty_traits::construct(_Getal(), _Unfancy(_Mylast), _STD forward<_Valty>(_Val)...);
        _Ty& _Result = *_Mylast;
        ++_Mylast;
    #if _HAS_CXX17
        return _Result;
    #else // ^^^ _HAS_CXX17 ^^^ // vvv !_HAS_CXX17 vvv
        (void)_Result;
    #endif // _HAS_CXX17
    }

public:
    template <class... _Valty>
    _CONSTEXPR20_CONTAINER decltype(auto) emplace_back(_Valty&&... _Val) {
        // insert by perfectly forwarding into element at end, provide strong guarantee
        auto& _My_data = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;
        if (_Mylast != _My_data._Myend) {
            return _Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...);
        }

        _Ty& _Result = *_Emplace_reallocate(_Mylast, _STD forward<_Valty>(_Val)...);
    #if _HAS_CXX17
        return _Result;
    #else // ^^^ _HAS_CXX17 ^^^ // vvv !_HAS_CXX17 vvv
        (void)_Result;
    #endif // _HAS_CXX17
    }

    _CONSTEXPR20_CONTAINER void push_back(const _Ty& _Val) { // insert element at end, provide strong guarantee
        emplace_back(_Val);
    }

    _CONSTEXPR20_CONTAINER void push_back(_Ty&& _Val) {
        // insert by moving into element at end, provide strong guarantee
        emplace_back(_STD move(_Val));
    }

    template <class... _Valty>
    _CONSTEXPR20_CONTAINER pointer _Emplace_reallocate(const pointer _Whereptr, _Valty&&... _Val) {
        // reallocate and insert by perfectly forwarding _Val at _Whereptr
        _Alty& _Al = _Getal();
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;

        const auto _Whereoff = static_cast<size_type>(_Whereptr - _Myfirst);
        const auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);

        const size_type _Newsize = _Oldsize + 1;
        const size_type _Newcapacity = _Calculate_growth(_Newsize);

        const pointer _Newvec = _Al.allocate(_Newcapacity);
        const pointer _Constructed_last = _Newvec + _Whereoff + 1;
        pointer _Constructed_first = _Constructed_last;

        _Alty_traits::construct(_Al, _Unfancy(_Newvec + _Whereoff), _STD forward<_Valty>(_Val)...);
        _Constructed_first = _Newvec + _Whereoff;

        if (_Whereptr == _Mylast) { // at back, provide strong guarantee
            _Umove_if_noexcept(_Myfirst, _Mylast, _Newvec);
        }
        else { // provide basic guarantee
            _Umove(_Myfirst, _Whereptr, _Newvec);
            _Constructed_first = _Newvec;
            _Umove(_Whereptr, _Mylast, _Newvec + _Whereoff + 1);
        }

        _Change_array(_Newvec, _Newsize, _Newcapacity);
        return _Newvec + _Whereoff;
    }

    template <class... _Valty>
    _CONSTEXPR20_CONTAINER iterator emplace(const_iterator _Where, _Valty&&... _Val) {
        // insert by perfectly forwarding _Val at _Where
        const pointer _Whereptr = _Where._Ptr;
        auto& _My_data = _Mypair._Myval2;
        const pointer _Oldlast = _My_data._Mylast;

        if (_Oldlast != _My_data._Myend) {
            if (_Whereptr == _Oldlast) { // at back, provide strong guarantee
                _Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...);
            }
            else {
                auto& _Al = _Getal();
                _Alloc_temporary<_Alty> _Obj(_Al, _STD forward<_Valty>(_Val)...); // handle aliasing
                // after constructing _Obj, provide basic guarantee
                _Alty_traits::construct(_Al, _Unfancy(_Oldlast), _STD move(_Oldlast[-1]));
                ++_My_data._Mylast;
                _Move_backward_unchecked(_Whereptr, _Oldlast - 1, _Oldlast);
                *_Whereptr = _STD move(_Obj._Storage._Value);
            }

            return _Make_iterator(_Whereptr);
        }

        return _Make_iterator(_Emplace_reallocate(_Whereptr, _STD forward<_Valty>(_Val)...));
    }

    _CONSTEXPR20_CONTAINER iterator insert(const_iterator _Where, const _Ty& _Val) { // insert _Val at _Where
        return emplace(_Where, _Val);
    }

    _CONSTEXPR20_CONTAINER iterator insert(const_iterator _Where, _Ty&& _Val) { // insert by moving _Val at _Where
        return emplace(_Where, _STD move(_Val));
    }

    _CONSTEXPR20_CONTAINER iterator insert(
        const_iterator _Where, _CRT_GUARDOVERFLOW const size_type _Count, const _Ty& _Val) {
        // insert _Count * _Val at _Where
        const pointer _Whereptr = _Where._Ptr;

        auto& _My_data = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;

        const pointer _Oldfirst = _My_data._Myfirst;
        const pointer _Oldlast = _Mylast;

        const auto _Whereoff = static_cast<size_type>(_Whereptr - _Oldfirst);
        const auto _Unused_capacity = static_cast<size_type>(_My_data._Myend - _Oldlast);
        const bool _One_at_back = _Count == 1 && _Whereptr == _Oldlast;
        if (_Count == 0) { // nothing to do, avoid invalidating iterators
        }
        else if (_Count > _Unused_capacity) { // reallocate
            const auto _Oldsize = static_cast<size_type>(_Oldlast - _Oldfirst);

            const size_type _Newsize = _Oldsize + _Count;
            const size_type _Newcapacity = _Calculate_growth(_Newsize);

            const pointer _Newvec = _Getal().allocate(_Newcapacity);
            const pointer _Constructed_last = _Newvec + _Whereoff + _Count;
            pointer _Constructed_first = _Constructed_last;

            _Ufill(_Newvec + _Whereoff, _Count, _Val);
            _Constructed_first = _Newvec + _Whereoff;

            if (_One_at_back) { // provide strong guarantee
                _Umove_if_noexcept(_Oldfirst, _Oldlast, _Newvec);
            }
            else { // provide basic guarantee
                _Umove(_Oldfirst, _Whereptr, _Newvec);
                _Constructed_first = _Newvec;
                _Umove(_Whereptr, _Oldlast, _Newvec + _Whereoff + _Count);
            }

            _Change_array(_Newvec, _Newsize, _Newcapacity);
        }
        else if (_One_at_back) { // provide strong guarantee
            _Emplace_back_with_unused_capacity(_Val);
        }
        else { // provide basic guarantee
            const _Alloc_temporary<_Alty> _Tmp_storage(_Getal(), _Val); // handle aliasing
            const auto& _Tmp = _Tmp_storage._Storage._Value;
            const auto _Affected_elements = static_cast<size_type>(_Oldlast - _Whereptr);

            if (_Count > _Affected_elements) { // new stuff spills off end
                _Mylast = _Ufill(_Oldlast, _Count - _Affected_elements, _Tmp);
                _Mylast = _Umove(_Whereptr, _Oldlast, _Mylast);
                _STD fill(_Whereptr, _Oldlast, _Tmp);
            }
            else { // new stuff can all be assigned
                _Mylast = _Umove(_Oldlast - _Count, _Oldlast, _Oldlast);
                _Move_backward_unchecked(_Whereptr, _Oldlast - _Count, _Oldlast);
                _STD fill(_Whereptr, _Whereptr + _Count, _Tmp);
            }
        }

        return _Make_iterator_offset(_Whereoff);
    }

private:
    template <class _Iter>
    _CONSTEXPR20_CONTAINER void _Insert_range(const_iterator _Where, _Iter _First, _Iter _Last, input_iterator_tag) {
        // insert input range [_First, _Last) at _Where
        if (_First == _Last) {
            return; // nothing to do, avoid invalidating iterators
        }

        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        const auto _Whereoff = static_cast<size_type>(_Where._Ptr - _Myfirst);
        const auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);

        // For one-at-back, provide strong guarantee.
        // Otherwise, provide basic guarantee (despite N4659 26.3.11.5 [Vector.modifiers]/1).
        // Performance note: except for one-at-back, emplace_back()'s strong guarantee is unnecessary here.

        for (; _First != _Last; ++_First) {
            emplace_back(*_First);
        }


        _STD rotate(_Myfirst + _Whereoff, _Myfirst + _Oldsize, _Mylast);
    }

    template <class _Iter>
    _CONSTEXPR20_CONTAINER void _Insert_range(const_iterator _Where, _Iter _First, _Iter _Last, forward_iterator_tag) {
        // insert forward range [_First, _Last) at _Where
        const pointer _Whereptr = _Where._Ptr;
        const auto _Count = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));

        auto& _My_data = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;

        const pointer _Oldfirst = _My_data._Myfirst;
        const pointer _Oldlast = _Mylast;
        const auto _Unused_capacity = static_cast<size_type>(_My_data._Myend - _Oldlast);

        if (_Count == 0) { // nothing to do, avoid invalidating iterators
        }
        else if (_Count > _Unused_capacity) { // reallocate
            const auto _Oldsize = static_cast<size_type>(_Oldlast - _Oldfirst);

            const size_type _Newsize = _Oldsize + _Count;
            const size_type _Newcapacity = _Calculate_growth(_Newsize);

            const pointer _Newvec = _Getal().allocate(_Newcapacity);
            const auto _Whereoff = static_cast<size_type>(_Whereptr - _Oldfirst);
            const pointer _Constructed_last = _Newvec + _Whereoff + _Count;
            pointer _Constructed_first = _Constructed_last;

            _TRY_BEGIN
                _Ucopy(_First, _Last, _Newvec + _Whereoff);
            _Constructed_first = _Newvec + _Whereoff;

            if (_Count == 1 && _Whereptr == _Oldlast) { // one at back, provide strong guarantee
                _Umove_if_noexcept(_Oldfirst, _Oldlast, _Newvec);
            }
            else { // provide basic guarantee
                _Umove(_Oldfirst, _Whereptr, _Newvec);
                _Constructed_first = _Newvec;
                _Umove(_Whereptr, _Oldlast, _Newvec + _Whereoff + _Count);
            }
            _CATCH_ALL
                _Destroy(_Constructed_first, _Constructed_last);
            _Getal().deallocate(_Newvec, _Newcapacity);
            _RERAISE;
            _CATCH_END

                _Change_array(_Newvec, _Newsize, _Newcapacity);
        }
        else { // Attempt to provide the strong guarantee for EmplaceConstructible failure.
              // If we encounter copy/move construction/assignment failure, provide the basic guarantee.
              // (For one-at-back, this provides the strong guarantee.)

            const auto _Affected_elements = static_cast<size_type>(_Oldlast - _Whereptr);

            if (_Count < _Affected_elements) { // some affected elements must be assigned
                _Mylast = _Umove(_Oldlast - _Count, _Oldlast, _Oldlast);
                _Move_backward_unchecked(_Whereptr, _Oldlast - _Count, _Oldlast);
                _Destroy(_Whereptr, _Whereptr + _Count);

                _TRY_BEGIN
                    _Ucopy(_First, _Last, _Whereptr);
                _CATCH_ALL
                    // glue the broken pieces back together

                    _TRY_BEGIN
                    _Umove(_Whereptr + _Count, _Whereptr + 2 * _Count, _Whereptr);
                _CATCH_ALL
                    // vaporize the detached piece
                _Destroy(_Whereptr + _Count, _Mylast);
                _Mylast = _Whereptr;
                _RERAISE;
                _CATCH_END

                    _Move_unchecked(_Whereptr + 2 * _Count, _Mylast, _Whereptr + _Count);
                _Destroy(_Oldlast, _Mylast);
                _Mylast = _Oldlast;
                _RERAISE;
                _CATCH_END
            }
            else { // affected elements don't overlap before/after
                const pointer _Relocated = _Whereptr + _Count;
                _Mylast = _Umove(_Whereptr, _Oldlast, _Relocated);
                _Destroy(_Whereptr, _Oldlast);

                _TRY_BEGIN
                    _Ucopy(_First, _Last, _Whereptr);
                _CATCH_ALL
                    // glue the broken pieces back together

                    _TRY_BEGIN
                    _Umove(_Relocated, _Mylast, _Whereptr);
                _CATCH_ALL
                    // vaporize the detached piece
                _Destroy(_Relocated, _Mylast);
                _Mylast = _Whereptr;
                _RERAISE;
                _CATCH_END

                    _Destroy(_Relocated, _Mylast);
                _Mylast = _Oldlast;
                _RERAISE;
                _CATCH_END
            }

        }
    }

public:
    template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20_CONTAINER iterator insert(const_iterator _Where, _Iter _First, _Iter _Last) {
        const pointer _Whereptr = _Where._Ptr;
        auto& _My_data = _Mypair._Myval2;
        const pointer _Oldfirst = _My_data._Myfirst;

        _Adl_verify_range(_First, _Last);
        const auto _Whereoff = static_cast<size_type>(_Whereptr - _Oldfirst);
        _Insert_range(_Where, _Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
        return _Make_iterator_offset(_Whereoff);
    }

    _CONSTEXPR20_CONTAINER iterator insert(const_iterator _Where, initializer_list<_Ty> _Ilist) {
        return insert(_Where, _Ilist.begin(), _Ilist.end());
    }

    _CONSTEXPR20_CONTAINER void assign(_CRT_GUARDOVERFLOW const size_type _Newsize, const _Ty& _Val) {
        // assign _Newsize * _Val
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;

        _My_data._Orphan_all();

        auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);
        if (_Newsize > _Oldsize) {
            const auto _Oldcapacity = static_cast<size_type>(_My_data._Myend - _Myfirst);
            if (_Newsize > _Oldcapacity) { // reallocate
                _Clear_and_reserve_geometric(_Newsize);
                _Oldsize = 0;
            }
            else {
                _STD fill(_Myfirst, _Mylast, _Val);
            }

            _Mylast = _Ufill(_Mylast, _Newsize - _Oldsize, _Val);
        }
        else {
            const pointer _Newlast = _Myfirst + _Newsize;
            _STD fill(_Myfirst, _Newlast, _Val);
            _Destroy(_Newlast, _Mylast);
            _Mylast = _Newlast;
        }
    }

private:
    template <class _Iter>
    _CONSTEXPR20_CONTAINER void _Assign_range(_Iter _First, _Iter _Last, input_iterator_tag) {
        // assign input range [_First, _Last)
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;

        _My_data._Orphan_all();

        pointer _Next = _Myfirst;

        for (; _First != _Last && _Next != _Mylast; ++_First, (void) ++_Next) {
            *_Next = *_First;
        }

        // Code size optimization: we've exhausted only the source, only the dest, or both.
        // If we've exhausted only the source: we Trim, then Append does nothing.
        // If we've exhausted only the dest: Trim does nothing, then we Append.
        // If we've exhausted both: Trim does nothing, then Append does nothing.

        // Trim.
        _Destroy(_Next, _Mylast);
        _Mylast = _Next;

        // Append.
        for (; _First != _Last; ++_First) {
            emplace_back(*_First); // performance note: emplace_back()'s strong guarantee is unnecessary here
        }
    }

    template <class _Iter>
    _CONSTEXPR20_CONTAINER void _Assign_range(_Iter _First, _Iter _Last, forward_iterator_tag) {
        // assign forward range [_First, _Last)
        const auto _Newsize = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        pointer& _Myend = _My_data._Myend;

        _My_data._Orphan_all();

        if constexpr (conjunction_v<bool_constant<_Ptr_copy_cat<_Iter, _Ty*>::_Trivially_copyable>,
            _Uses_default_construct<_Alty, _Ty*, decltype(*_First)>,
            _Uses_default_destroy<_Alty, _Ty*>>) {
        #ifdef __cpp_lib_constexpr_dynamic_alloc
            if (!_STD is_constant_evaluated())
            #endif // __cpp_lib_constexpr_dynamic_alloc
            {
                const auto _Oldcapacity = static_cast<size_type>(_Myend - _Myfirst);
                if (_Newsize > _Oldcapacity) {
                    _Clear_and_reserve_geometric(_Newsize);
                }

                _Mylast = _Refancy<pointer>(_Copy_memmove(_First, _Last, _Unfancy(_Myfirst)));
                return;
            }
        }
        auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);

        if (_Newsize > _Oldsize) {
            const auto _Oldcapacity = static_cast<size_type>(_Myend - _Myfirst);
            if (_Newsize > _Oldcapacity) { // reallocate
                _Clear_and_reserve_geometric(_Newsize);
                _Oldsize = 0;
            }

            // performance note: traversing [_First, _Mid) twice
            const _Iter _Mid = _STD next(_First, static_cast<difference_type>(_Oldsize));
            _Copy_unchecked(_First, _Mid, _Myfirst);
            _Mylast = _Ucopy(_Mid, _Last, _Mylast);
        }
        else {
            const pointer _Newlast = _Myfirst + _Newsize;
            _Copy_unchecked(_First, _Last, _Myfirst);
            _Destroy(_Newlast, _Mylast);
            _Mylast = _Newlast;
        }
    }

public:
    template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20_CONTAINER void assign(_Iter _First, _Iter _Last) {
        _Adl_verify_range(_First, _Last);
        _Assign_range(_Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
    }

    _CONSTEXPR20_CONTAINER void assign(initializer_list<_Ty> _Ilist) {
        _Assign_range(_Ilist.begin(), _Ilist.end(), random_access_iterator_tag{});
    }

private:
    _CONSTEXPR20_CONTAINER void _Copy_assign(const Vector& _Right, false_type) {
        _Pocca(_Getal(), _Right._Getal());
        auto& _Right_data = _Right._Mypair._Myval2;
        assign(_Right_data._Myfirst, _Right_data._Mylast);
    }

    _CONSTEXPR20_CONTAINER void _Copy_assign(const Vector& _Right, true_type) {
        if (_Getal() != _Right._Getal()) {
            _Tidy();
            _Mypair._Myval2._Reload_proxy(
                _GET_PROXY_ALLOCATOR(_Alty, _Getal()), _GET_PROXY_ALLOCATOR(_Alty, _Right._Getal()));
        }

        _Copy_assign(_Right, false_type{});
    }

public:
    _CONSTEXPR20_CONTAINER Vector& operator=(const Vector& _Right) {
        if (this != _STD addressof(_Right)) {
            _Copy_assign(_Right, _Choose_pocca<_Alty>{});
        }

        return *this;
    }

    _CONSTEXPR20_CONTAINER Vector& operator=(initializer_list<_Ty> _Ilist) {
        _Assign_range(_Ilist.begin(), _Ilist.end(), random_access_iterator_tag{});
        return *this;
    }

private:
    template <class _Ty2>
    _CONSTEXPR20_CONTAINER void _Resize_reallocate(const size_type _Newsize, const _Ty2& _Val) {

        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;

        const auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);
        const size_type _Newcapacity = _Calculate_growth(_Newsize);

        const pointer _Newvec = _Getal().allocate(_Newcapacity);
        const pointer _Appended_first = _Newvec + _Oldsize;
        pointer _Appended_last = _Appended_first;

        _Appended_last = _Ufill(_Appended_first, _Newsize - _Oldsize, _Val);
        _Umove_if_noexcept(_Myfirst, _Mylast, _Newvec);

        _Change_array(_Newvec, _Newsize, _Newcapacity);
    }

    template <class _Ty2>
    _CONSTEXPR20_CONTAINER void _Resize(const size_type _Newsize, const _Ty2& _Val) {
        // trim or append elements, provide strong guarantee
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        const auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);
        if (_Newsize < _Oldsize) { // trim
            const pointer _Newlast = _Myfirst + _Newsize;
            _Destroy(_Newlast, _Mylast);
            _Mylast = _Newlast;
            return;
        }

        if (_Newsize > _Oldsize) { // append
            const auto _Oldcapacity = static_cast<size_type>(_My_data._Myend - _Myfirst);
            if (_Newsize > _Oldcapacity) { // reallocate
                _Resize_reallocate(_Newsize, _Val);
                return;
            }
            const pointer _Oldlast = _Mylast;
            _Mylast = _Ufill(_Oldlast, _Newsize - _Oldsize, _Val);
        }

        // if _Newsize == _Oldsize, do nothing; avoid invalidating iterators
    }

public:
    _CONSTEXPR20_CONTAINER void resize(_CRT_GUARDOVERFLOW const size_type _Newsize) {
        // trim or append value-initialized elements, provide strong guarantee
        _Resize(_Newsize, _Value_init_tag{});
    }

    _CONSTEXPR20_CONTAINER void resize(_CRT_GUARDOVERFLOW const size_type _Newsize, const _Ty& _Val) {
        // trim or append copies of _Val, provide strong guarantee
        _Resize(_Newsize, _Val);
    }

private:
    _CONSTEXPR20_CONTAINER void _Reallocate_exactly(const size_type _Newcapacity) {
        // set capacity to _Newcapacity (without geometric growth), provide strong guarantee
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;

        const auto _Size = static_cast<size_type>(_Mylast - _Myfirst);

        const pointer _Newvec = _Getal().allocate(_Newcapacity);

        _Umove_if_noexcept(_Myfirst, _Mylast, _Newvec);

        _Change_array(_Newvec, _Size, _Newcapacity);
    }

    _CONSTEXPR20_CONTAINER void _Clear_and_reserve_geometric(const size_type _Newsize) {
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        pointer& _Myend = _My_data._Myend;

        const size_type _Newcapacity = _Calculate_growth(_Newsize);

        if (_Myfirst) { // destroy and deallocate old array
            _Destroy(_Myfirst, _Mylast);
            _Getal().deallocate(_Myfirst, static_cast<size_type>(_Myend - _Myfirst));

            _Myfirst = nullptr;
            _Mylast = nullptr;
            _Myend = nullptr;
        }

        _Buy_raw(_Newcapacity);
    }

public:
    _CONSTEXPR20_CONTAINER void reserve(_CRT_GUARDOVERFLOW const size_type _Newcapacity) {
        // increase capacity to _Newcapacity (without geometric growth), provide strong guarantee
        if (_Newcapacity > capacity()) { // something to do (reserve() never shrinks)
            _Reallocate_exactly(_Newcapacity);
        }
    }

    _CONSTEXPR20_CONTAINER void shrink_to_fit() { // reduce capacity to size, provide strong guarantee
        auto& _My_data = _Mypair._Myval2;
        const pointer _Oldlast = _My_data._Mylast;
        if (_Oldlast != _My_data._Myend) { // something to do
            const pointer _Oldfirst = _My_data._Myfirst;
            if (_Oldfirst == _Oldlast) {
                _Tidy();
            }
            else {
                _Reallocate_exactly(static_cast<size_type>(_Oldlast - _Oldfirst));
            }
        }
    }

    _CONSTEXPR20_CONTAINER void pop_back() noexcept /* strengthened */ {
        auto& _My_data = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;

        _Alty_traits::destroy(_Getal(), _Unfancy(_Mylast - 1));
        --_Mylast;
    }

    _CONSTEXPR20_CONTAINER iterator erase(const_iterator _Where) noexcept(
        is_nothrow_move_assignable_v<value_type>) /* strengthened */ {
        const pointer _Whereptr = _Where._Ptr;
        auto& _My_data = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;

        _Move_unchecked(_Whereptr + 1, _Mylast, _Whereptr);
        _Alty_traits::destroy(_Getal(), _Unfancy(_Mylast - 1));
        --_Mylast;
        return iterator(_Whereptr, _STD addressof(_My_data));
    }

    _CONSTEXPR20_CONTAINER iterator erase(const_iterator _First, const_iterator _Last) noexcept(
        is_nothrow_move_assignable_v<value_type>) /* strengthened */ {
        const pointer _Firstptr = _First._Ptr;
        const pointer _Lastptr = _Last._Ptr;
        auto& _My_data = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;


        if (_Firstptr != _Lastptr) { // something to do, invalidate iterators

            const pointer _Newlast = _Move_unchecked(_Lastptr, _Mylast, _Firstptr);
            _Destroy(_Newlast, _Mylast);
            _Mylast = _Newlast;
        }

        return iterator(_Firstptr, _STD addressof(_My_data));
    }

    _CONSTEXPR20_CONTAINER void clear() noexcept { // erase all
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;

        _My_data._Orphan_all();
        _Destroy(_Myfirst, _Mylast);
        _Mylast = _Myfirst;
    }

    _CONSTEXPR20_CONTAINER void swap(Vector& _Right) noexcept /* strengthened */ {
        if (this != _STD addressof(_Right)) {
            _Pocs(_Getal(), _Right._Getal());
            _Mypair._Myval2._Swap_val(_Right._Mypair._Myval2);
        }
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Ty* data() noexcept {
        return _Unfancy_maybe_null(_Mypair._Myval2._Myfirst);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const _Ty* data() const noexcept {
        return _Unfancy_maybe_null(_Mypair._Myval2._Myfirst);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER iterator begin() noexcept {
        auto& _My_data = _Mypair._Myval2;
        return iterator(_My_data._Myfirst, _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_iterator begin() const noexcept {
        auto& _My_data = _Mypair._Myval2;
        return const_iterator(_My_data._Myfirst, _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER iterator end() noexcept {
        auto& _My_data = _Mypair._Myval2;
        return iterator(_My_data._Mylast, _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_iterator end() const noexcept {
        auto& _My_data = _Mypair._Myval2;
        return const_iterator(_My_data._Mylast, _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_iterator cbegin() const noexcept {
        return begin();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_iterator cend() const noexcept {
        return end();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reverse_iterator crend() const noexcept {
        return rend();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER pointer _Unchecked_begin() noexcept {
        return _Mypair._Myval2._Myfirst;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_pointer _Unchecked_begin() const noexcept {
        return _Mypair._Myval2._Myfirst;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER pointer _Unchecked_end() noexcept {
        return _Mypair._Myval2._Mylast;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_pointer _Unchecked_end() const noexcept {
        return _Mypair._Myval2._Mylast;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER bool empty() const noexcept {
        auto& _My_data = _Mypair._Myval2;
        return _My_data._Myfirst == _My_data._Mylast;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type size() const noexcept {
        auto& _My_data = _Mypair._Myval2;
        return static_cast<size_type>(_My_data._Mylast - _My_data._Myfirst);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type max_size() const noexcept {
        return (_STD min)(
            static_cast<size_type>((numeric_limits<difference_type>::max)()), _Alty_traits::max_size(_Getal()));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type capacity() const noexcept {
        auto& _My_data = _Mypair._Myval2;
        return static_cast<size_type>(_My_data._Myend - _My_data._Myfirst);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Ty& operator[](const size_type& _Pos) noexcept /* strengthened */ {
        auto& _My_data = _Mypair._Myval2;

        return _My_data._Myfirst[_Pos];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const _Ty& operator[](const size_type _Pos) const noexcept /* strengthened */ {
        auto& _My_data = _Mypair._Myval2;

        return _My_data._Myfirst[_Pos];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Ty& at(const size_type _Pos) {
        auto& _My_data = _Mypair._Myval2;
        return _My_data._Myfirst[_Pos];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const _Ty& at(const size_type _Pos) const {
        auto& _My_data = _Mypair._Myval2;
        return _My_data._Myfirst[_Pos];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Ty& front() noexcept /* strengthened */ {
        auto& _My_data = _Mypair._Myval2;

        return *_My_data._Myfirst;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const _Ty& front() const noexcept /* strengthened */ {
        auto& _My_data = _Mypair._Myval2;

        return *_My_data._Myfirst;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Ty& back() noexcept /* strengthened */ {
        auto& _My_data = _Mypair._Myval2;

        return _My_data._Mylast[-1];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const _Ty& back() const noexcept /* strengthened */ {
        auto& _My_data = _Mypair._Myval2;

        return _My_data._Mylast[-1];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER allocator_type get_allocator() const noexcept {
        return static_cast<allocator_type>(_Getal());
    }

private:
    _CONSTEXPR20_CONTAINER pointer _Ufill(pointer _Dest, const size_type _Count, const _Ty& _Val) {
        // fill raw _Dest with _Count copies of _Val, using allocator
        return _Uninitialized_fill_n(_Dest, _Count, _Val, _Getal());
    }

    _CONSTEXPR20_CONTAINER pointer _Ufill(pointer _Dest, const size_type _Count, _Value_init_tag) {
        // fill raw _Dest with _Count value-initialized objects, using allocator
        return _Uninitialized_value_construct_n(_Dest, _Count, _Getal());
    }

    template <class _Iter>
    _CONSTEXPR20_CONTAINER pointer _Ucopy(_Iter _First, _Iter _Last, pointer _Dest) {
        // copy [_First, _Last) to raw _Dest, using allocator
        return _Uninitialized_copy(_First, _Last, _Dest, _Getal());
    }

    _CONSTEXPR20_CONTAINER pointer _Umove(pointer _First, pointer _Last, pointer _Dest) {
        // move [_First, _Last) to raw _Dest, using allocator
        return _Uninitialized_move(_First, _Last, _Dest, _Getal());
    }

    _CONSTEXPR20_CONTAINER void _Umove_if_noexcept1(pointer _First, pointer _Last, pointer _Dest, true_type) {
        // move [_First, _Last) to raw _Dest, using allocator
        _Uninitialized_move(_First, _Last, _Dest, _Getal());
    }

    _CONSTEXPR20_CONTAINER void _Umove_if_noexcept1(pointer _First, pointer _Last, pointer _Dest, false_type) {
        // copy [_First, _Last) to raw _Dest, using allocator
        _Uninitialized_copy(_First, _Last, _Dest, _Getal());
    }

    _CONSTEXPR20_CONTAINER void _Umove_if_noexcept(pointer _First, pointer _Last, pointer _Dest) {
        // move_if_noexcept [_First, _Last) to raw _Dest, using allocator
        _Umove_if_noexcept1(_First, _Last, _Dest,
            bool_constant<disjunction_v<is_nothrow_move_constructible<_Ty>, negation<is_copy_constructible<_Ty>>>>{});
    }

    _CONSTEXPR20_CONTAINER void _Destroy(pointer _First, pointer _Last) {
        // destroy [_First, _Last) using allocator
        _Destroy_range(_First, _Last, _Getal());
    }

    _CONSTEXPR20_CONTAINER size_type _Calculate_growth(const size_type _Newsize) const {
        // given _Oldcapacity and _Newsize, calculate geometric growth
        const size_type _Oldcapacity = capacity();

        const size_type _Geometric =
            _Oldcapacity <= 1024 ? _Oldcapacity <= 256 ? _Oldcapacity << 2 : _Oldcapacity << 1 : (_Oldcapacity + (_Oldcapacity >> 1));

        if (_Geometric < _Newsize)
            return _Newsize; // geometric growth would be insufficient

        return _Geometric; // geometric growth is sufficient
    }

    _CONSTEXPR20_CONTAINER void _Buy_raw(const size_type _Newcapacity) {
        // allocate array with _Newcapacity elements
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        pointer& _Myend = _My_data._Myend;

        const auto _Newvec = _Getal().allocate(_Newcapacity);
        _Myfirst = _Newvec;
        _Mylast = _Newvec;
        _Myend = _Newvec + _Newcapacity;
    }

    _CONSTEXPR20_CONTAINER void _Buy_nonzero(const size_type _Newcapacity) {
        // allocate array with _Newcapacity elements
    #ifdef _ENABLE_STL_INTERNAL_CHECK
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        pointer& _Myend = _My_data._Myend;
        _STL_INTERNAL_CHECK(!_Myfirst && !_Mylast && !_Myend); // check that *this is tidy
        _STL_INTERNAL_CHECK(0 < _Newcapacity);
    #endif // _ENABLE_STL_INTERNAL_CHECK

        _Buy_raw(_Newcapacity);
    }

    _CONSTEXPR20_CONTAINER void _Change_array(
        const pointer _Newvec, const size_type _Newsize, const size_type _Newcapacity) {
        // orphan all iterators, discard old array, acquire new array
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        pointer& _Myend = _My_data._Myend;

        _My_data._Orphan_all();

        if (_Myfirst) { // destroy and deallocate old array
            _Destroy(_Myfirst, _Mylast);
            _Getal().deallocate(_Myfirst, static_cast<size_type>(_Myend - _Myfirst));
        }

        _Myfirst = _Newvec;
        _Mylast = _Newvec + _Newsize;
        _Myend = _Newvec + _Newcapacity;
    }

    _CONSTEXPR20_CONTAINER void _Tidy() noexcept { // free all storage
        auto& _My_data = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        pointer& _Myend = _My_data._Myend;

        _My_data._Orphan_all();

        if (_Myfirst) { // destroy and deallocate old array
            _Destroy(_Myfirst, _Mylast);
            _Getal().deallocate(_Myfirst, static_cast<size_type>(_Myend - _Myfirst));

            _Myfirst = nullptr;
            _Mylast = nullptr;
            _Myend = nullptr;
        }
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Alty& _Getal() noexcept {
        return _Mypair._Get_first();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const _Alty& _Getal() const noexcept {
        return _Mypair._Get_first();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER iterator _Make_iterator(const pointer _Ptr) noexcept {
        return iterator(_Ptr, _STD addressof(_Mypair._Myval2));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER iterator _Make_iterator_offset(const size_type _Offset) noexcept {
        // return the iterator begin() + _Offset without a debugging check
        auto& _My_data = _Mypair._Myval2;
        return iterator(_My_data._Myfirst + _Offset, _STD addressof(_My_data));
    }

    _Compressed_pair<_Alty, _Scary_val> _Mypair;
};

_STD_END

#endif MY_VECTOR_H