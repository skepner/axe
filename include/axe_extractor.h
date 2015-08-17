//-----------------------------------------------------------------------------
//  Copyright (C) 2011-2012, GB Research, LLC (www.gbresearch.com)
//
//  Boost Software License - Version 1.0 - August 17th, 2003
//
//  Permission is hereby granted, free of charge, to any person or organization
//  obtaining a copy of the software and accompanying documentation covered by
//  this license (the "Software") to use, reproduce, display, distribute,
//  execute, and transmit the Software, and to prepare derivative works of the
//  Software, and to permit third-parties to whom the Software is furnished to
//  do so, all subject to the following:
//
//  The copyright notices in the Software and this entire statement, including
//  the above license grant, this restriction and the following disclaimer,
//  must be included in all copies of the Software, in whole or in part, and
//  all derivative works of the Software, unless such copies or derivative
//  works are solely in the form of machine-executable object code generated by
//  a source language processor.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
//  FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef AXE_EXTRACTOR_H
#define AXE_EXTRACTOR_H

#include <string>
#include <sstream>
#include <ostream>
#include <utility>
#include "axe_result.h"
#include "axe_trait.h"

namespace axe {

    //-------------------------------------------------------------------------
    /// rule extracts iterator range from matching R and passes it to E
    //-------------------------------------------------------------------------
    template<class R, class E>
    class r_extractor_t AXE_RULE
    {
        R r_;
        E e_;

    public:

        r_extractor_t(R&& r, E&& e) : r_(std::forward<R>(r)), e_(std::forward<E>(e)) {}

        template<class Iterator>
        result<Iterator> operator() (Iterator i1, Iterator i2)const
        {
            result<Iterator> i = r_(i1, i2);

            if(i.matched)
                e_(i1, i.position);

            return i;
        }
    };

    //-------------------------------------------------------------------------
    /// extractor to evaluate iterator range and convert using stringstream
    //-------------------------------------------------------------------------
    template<class T>
    class e_value_t AXE_EXTRACTOR
    {
        T& t_;
    public:
        explicit e_value_t(T& t) : t_(t) {}

        template<class Iterator>
        void operator() (Iterator i1, Iterator i2)const
        {
            std::operator>>(std::istringstream(std::string(i1, i2)), t_);
        }
    };

    //-------------------------------------------------------------------------
    /// extractor to evaluate iterator range specialized for std::basic_string
    //-------------------------------------------------------------------------
    template<class C, class T, class A>
    class e_value_t<std::basic_string<C, T, A>> AXE_EXTRACTOR
    {
        std::basic_string<C, T, A>& str;
    public:
        explicit e_value_t(std::basic_string<C, T, A>& aStr) : str(aStr) {}

        template<class Iterator>
        void operator() (Iterator i1, Iterator i2) const
        {
            str.assign(i1, i2);
        }
    };

    //-------------------------------------------------------------------------
    /// extractor for single character
    //-------------------------------------------------------------------------
    template<>
    class e_value_t<char> AXE_EXTRACTOR
    {
        char& c;
    public:
        explicit e_value_t(char& aC) : c(aC) {}

        template<class Iterator>
        void operator() (Iterator i1, Iterator i2) const
        {
            static_assert(std::is_convertible<decltype(*i1), char>::value,
                "iterator value_type must be convertible to char");
            if(i1 != i2)
                c = char(*i1);
        }
    };

    //-------------------------------------------------------------------------
    /// reference wrapper for extractor (can be used with lambda functions)
    //-------------------------------------------------------------------------
    template<class T>
    class e_ref_t AXE_EXTRACTOR
    {
        T t_;
    public:
        e_ref_t(T&& t) : t_(std::forward<T>(t)) {}

        template<class I>
        void operator()(I i1, I i2) const
        {
            t_(i1, i2);
        }
    };

    //-------------------------------------------------------------------------
    /// extractor for iterator range to determine its length
    //-------------------------------------------------------------------------
    template<class S>
    class e_length_t AXE_EXTRACTOR
    {
        S& length;
    public:
        explicit e_length_t(S& aLength) : length(aLength) {}

        template<class Iterator>
        void operator() (Iterator i1, Iterator i2) const
        {
            static_assert(std::is_convertible<size_t, S>::value,
                "iterator distance must be convertible to length type");
            length = S(std::distance(i1, i2));
        }
    };

    //-------------------------------------------------------------------------
    /// extractor for iterator range that pushes values to container
    //-------------------------------------------------------------------------
    template<class C>
    class e_push_back_t AXE_EXTRACTOR
    {
        C& c;
        typedef typename C::value_type value_type;
    public:
        explicit e_push_back_t(C& aC) : c(aC) {}

        template<class Iterator>
        void operator() (Iterator i1, Iterator i2) const
        {
            value_type value;
            e_value_t<value_type> ex(value);
            ex(i1, i2);
            c.push_back(std::move(value));
        }
    };
}

#endif
