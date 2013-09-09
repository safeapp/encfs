/*****************************************************************************
 * Author:   Rian Hunter <rian@alum.mit.edu>
 *
 *****************************************************************************
 * Copyright (c) 2013, Rian Hunter
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* A lightweight version of C++1y optional */

#ifndef _optional_incl_
#define _optional_incl_

#include <stdexcept>

#include "base/config.h"

namespace _my_optional
{

class nullopt_t
{
 public:
  constexpr nullopt_t()
  {}
};

constexpr nullopt_t nullopt;

class bad_optional_access : public std::logic_error
{
public:
  bad_optional_access(const char *a)
  : std::logic_error(a)
  {}
};

template<class T, typename std::enable_if<!std::is_reference<T>::value, int>::type = 0>
class optional
{
private:
  union
  {
    char _null_state;
    T _val;
  };
  bool _engaged;

  constexpr int _check_engaged()
  {

    return 0;
  }

public:
  constexpr optional()
  : _null_state('\0')
  , _engaged( false )
  {}

  constexpr optional(nullopt_t)
  : _null_state('\0')
  , _engaged( false )
  {}

  constexpr optional(T && val)
  : _val( std::move( val ) )
  , _engaged( true )
  {}

  optional &operator=(const optional & val)
  {
    if(val._engaged)
    {
      _val = val._val;
      _engaged = true;
    } else
    {
      if(_engaged)
        _val.~T();
      _engaged = false;
    }
    return *this;
  }

  optional(const optional & val)
  : optional()
  {
    *this = val;
  }

  optional &operator=(optional && val)
  {
    if(val._engaged)
    {
      _val = std::move(val._val);
      _engaged = true;
    } else
    {
      if(_engaged)
        _val.~T();
      _engaged = false;
    }
    return *this;
  }

  optional(optional && val)
  : optional()
  {
    *this = std::move( val );
  }

  ~optional()
  {
    if (_engaged)
      _val.~T();
  }

  constexpr const T &operator *() const
  {
    static_assert(_engaged, "bad optional access");
    return _val;
  }

  T &operator *()
  {
    if (!_engaged) { throw bad_optional_access("bad optional access"); }
    return _val;
  }

  constexpr const T *operator ->() const
  {
    static_assert(_engaged, "bad optional access");
    return &_val;
  }

  T *operator ->()
  {
    if (!_engaged) { throw bad_optional_access("bad optional access"); }
    return &_val;
  }

  constexpr explicit operator bool() const
  {
    return _engaged;
  }
};

template<class T>
bool operator==(optional<T> f, nullopt_t)
{
  return !f;
}

template<class T>
bool operator==(nullopt_t, optional<T> f)
{
  return !f;
}

template<class T>
bool operator==(optional<T> a, optional<T> b)
{
  if (a && b) {
    return *a == *b;
  }
  else if (!a && !b) {
    return true;
  }
  else {
    return false;
  }
}

}

#ifdef HAVE_OPTIONAL
  using std::optional;
  using std::nullopt_t;
  using std::nullopt;
  using std::bad_optional_access;
#else
  using namespace _my_optional;
#endif

#endif