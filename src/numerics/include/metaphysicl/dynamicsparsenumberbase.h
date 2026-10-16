//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MetaPhysicL - A metaprogramming library for physics calculations
//
// Copyright (C) 2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
//
// $Id: core.h 37197 2013-02-21 05:49:09Z roystgnr $
//
//--------------------------------------------------------------------------


#ifndef METAPHYSICL_DYNAMICSPARSENUMBERBASE_H
#define METAPHYSICL_DYNAMICSPARSENUMBERBASE_H

#include "metaphysicl/dynamicsparsenumberbase_decl.h"

#include "metaphysicl/metaphysicl_algorithm.h"
#include "metaphysicl/metaphysicl_common.h"
#include "metaphysicl/metaphysicl_math.h"

namespace MetaPhysicL {

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
std::size_t
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::size() const
{ metaphysicl_assert_equal_to(_data.size(), _indices.size());
  return _data.size(); }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
void
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::resize(std::size_t s)
{ metaphysicl_assert_equal_to(_data.size(), _indices.size());
  _data.resize(s);
  _indices.resize(s); }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::DynamicSparseNumberBase() {}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <typename Data2, typename Indices2, class... SubTypeArgs2>
METAPHYSICL_INLINE
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::
DynamicSparseNumberBase(const DynamicSparseNumberBase<Data2, Indices2, SubType, SubTypeArgs2...> & src)
{ 
  this->resize(src.size());
  const auto & src_indices = src.nude_indices();
  const auto & src_data = src.nude_data();
  for (std::size_t i = 0; i < src.size(); ++i)
  {
    _data[i] = src_data[i];
    _indices[i] = src_indices[i];
  }
}

#ifdef METAPHYSICL_USE_STD_MOVE
template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <typename Data2, typename Indices2, class... SubTypeArgs2>
METAPHYSICL_INLINE DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::DynamicSparseNumberBase(
    DynamicSparseNumberBase<Data2, Indices2, SubType, SubTypeArgs2...> && src)
{
  _data = std::move(src.nude_data());
  _indices = std::move(src.nude_indices());
}
#endif // METAPHYSICL_USE_STD_MOVE

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
typename Data::value_type*
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::raw_data()
{ return size()?&_data[0]:NULL; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
const typename Data::value_type*
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::raw_data() const
{ return size()?&_data[0]:NULL; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
typename Data::reference
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::raw_at(unsigned int i)
{ return _data[i]; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
typename Data::const_reference
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::raw_at(unsigned int i) const
{ return _data[i]; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
typename Indices::value_type&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::raw_index(unsigned int i)
{ return _indices[i]; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
const typename Indices::value_type &
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::raw_index(unsigned int i) const
{ return _indices[i]; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
const Data&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::nude_data() const
{ return _data; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
Data&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::nude_data()
{ return _data; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
const Indices&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::nude_indices() const
{ return _indices; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
Indices&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::nude_indices()
{ return _indices; }

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
std::size_t
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::runtime_index_query(index_value_type i) const
{
  auto it = algorithm::lower_bound(_indices.begin(), _indices.end(), i);
  if (it == _indices.end() || *it != i)
    return MetaPhysicL::numeric_limits<std::size_t>::max();
  std::size_t offset = it - _indices.begin();
  metaphysicl_assert_equal_to(_indices[offset], i);
  return offset;
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
std::size_t
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::runtime_index_of(index_value_type i) const
{
  auto it = algorithm::lower_bound(_indices.begin(), _indices.end(), i);
  metaphysicl_assert(it != _indices.end());
  std::size_t offset = it - _indices.begin();
  metaphysicl_assert_equal_to(_indices[offset], i);
  return offset;
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
typename Data::value_type &
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::operator[](index_value_type i)
{
  static T zero = 0;

  // Bad user code could make this fail.  We'd prefer to catch OOB
  // writes at *write* time but at least we can catch at read time.
  metaphysicl_assert(zero == T(0));

  std::size_t rq = runtime_index_query(i);
  if (rq == MetaPhysicL::numeric_limits<std::size_t>::max())
    return zero;
  return _data[rq];
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
const typename Data::value_type&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::operator[](index_value_type i) const
{
  static const T zero = 0;
  std::size_t rq = runtime_index_query(i);
  if (rq == MetaPhysicL::numeric_limits<std::size_t>::max())
    return zero;
  return _data[rq];
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <unsigned int i>
METAPHYSICL_INLINE
typename DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::template entry_type<i>::type&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::get() {
  return _data[runtime_index_of(i)];
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <unsigned int i>
METAPHYSICL_INLINE
const typename DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::template entry_type<i>::type&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::get() const {
  return _data[runtime_index_of(i)];
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
typename DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::value_type&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::insert(unsigned int i)
{
  auto upper_it = algorithm::lower_bound(_indices.begin(), _indices.end(), i);
  std::size_t offset = upper_it - _indices.begin();

  // If we don't have entry i, insert it.  Yes this is O(N).
  if ((upper_it == _indices.end()) ||
      *upper_it != I(i))
    {
      std::size_t old_size = this->size();
      this->resize(old_size+1);
      algorithm::copy_backward(_indices.begin()+offset, _indices.begin()+old_size, _indices.end());
      algorithm::copy_backward(_data.begin()+offset, _data.begin()+old_size, _data.end());
      _indices[offset] = i;
      _data[offset] = 0;
    }

  // We have entry i now; return it
  return _data[offset];
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <unsigned int i>
METAPHYSICL_INLINE
typename DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::template entry_type<i>::type&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::insert() {
  return this->insert(i);
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <unsigned int i, typename T2>
METAPHYSICL_INLINE
void
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::set(const T2& val) {
  _data[runtime_index_of(i)] = val;
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
bool
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::boolean_test() const {
  std::size_t index_size = size();
  for (unsigned int i=0; i != index_size; ++i)
    if (_data[i])
      return true;
  return false;
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
SubType<SubTypeArgs...>
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::operator- () const {
  std::size_t index_size = size();
  SubType<SubTypeArgs...> returnval;
  returnval.resize(index_size);
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
  for (unsigned int i=0; i != index_size; ++i)
    {
      returnval.raw_index(i) = _indices[i];
      returnval.raw_at(i) = -_data[i];
    }
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
  return returnval;
}

  // Since this is a dynamically allocated sparsity pattern, we can
  // increase it as needed to support e.g. operator+=
template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <typename Indices2>
METAPHYSICL_INLINE
void
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::sparsity_union (const Indices2 & new_indices)
{
#ifndef NDEBUG
  // Assert sorted and no duplicates
  if (_indices.size() > 1)
    for (std::size_t i = 0; i < _indices.size() - 1; ++i)
      metaphysicl_assert(_indices[i] < _indices[i + 1]);
  if (new_indices.size() > 1)
    for (std::size_t i = 0; i < new_indices.size() - 1; ++i)
      metaphysicl_assert(new_indices[i] < new_indices[i + 1]);
#endif

  //
  // First phase: count how many new indices will be inserted
  //

  std::size_t index_it = 0;
  std::size_t index2_it = 0;

  typedef typename Indices2::value_type I2;
  typedef typename CompareTypes<I,I2>::supertype max_index_type;
  // number of indices in new_indices which aren't in _indices
  max_index_type unseen_indices = 0;

  const I maxI = numeric_limits<I>::max();

  while (index2_it != new_indices.size()) {
    I idx1 = (index_it == _indices.size()) ? maxI : _indices[index_it];
    I2 idx2 = new_indices[index2_it];

    // Advance our index while we are behind new_indices index
    while (idx1 < idx2) {
      ++index_it;
      idx1 = (index_it == _indices.size()) ? maxI : _indices[index_it];
    }

    // advance the two indexes in lock-step while they are equal and we aren't at the end
    // of _indices
    while ((idx1 == idx2) &&
           (idx1 != maxI)) {
      ++index_it;
      idx1 = (index_it == _indices.size()) ? maxI : _indices[index_it];
      ++index2_it;
      idx2 = (index2_it == new_indices.size()) ? maxI : new_indices[index2_it];
    }

    // Advance the new_indices index while we it's behind our index, all the while adding to unseen_indices
    while (idx2 < idx1) {
      ++unseen_indices;
      ++index2_it;
      if (index2_it == new_indices.size())
        // If we've hit the end of new_indices we break from the outer loop
        break;
      idx2 = new_indices[index2_it];
    }
  }

  // The common case is cheap
  if (!unseen_indices)
    return;

  //
  // Phase 2: reverse in-place merge
  //

  const std::size_t old_size = this->size();
  const std::size_t new_size = old_size + unseen_indices;

  // Calls resize on both _data and _indices
  this->resize(new_size);

  // Write iterator starting at the tail of our resized containers
  std::ptrdiff_t write_it = new_size - 1;

  // Read iterator starting at our old tail
  std::ptrdiff_t read_it = old_size - 1;
  // Read iterator for new_indices starting at its tail
  std::ptrdiff_t new_indices_read_it = new_indices.size() - 1;

  for (; write_it != -1; --write_it) {
    // First operand: we've reached the end of our original indices
    // Second operand: we haven't reached the end of our original indices *but* the new_indices
    //   read iterator is pointing to an index that is greater than our read iterator
    //   is pointing to
    // Either way, we should write the new_indices index. Since we are writing from the
    // new_indices container, for which we have not yet imported the corresponding data,
    // we write 0 to the corresponding _data location
    if ((read_it == -1) ||
        ((new_indices_read_it != -1) &&
         (new_indices[new_indices_read_it] > _indices[read_it]))) {
      _data[write_it] = 0;
      _indices[write_it] = new_indices[new_indices_read_it];
      // Finished reading from the new indices, thus decrement its iterator
      --new_indices_read_it;
    } else {
      //
      // If we didn't satisfy the if branch, that means we will be writing from our _indices/_data
      // instead of from new_indices
      //

      // Handle the case where our _indices iterator and the new_indices iterator point to the same
      // index value. E.g. we must make sure that we decrement both _indices and new_indices
      // iterators
      if ((new_indices_read_it != -1) &&
          (new_indices[new_indices_read_it] == _indices[read_it]))
        --new_indices_read_it;
      metaphysicl_assert(read_it > -1);
      metaphysicl_assert(write_it > -1);
      _data[write_it] = _data[read_it];
      _indices[write_it] = _indices[read_it];
      // Finished reading from our _indices/_data, thus decrement their iterator
      --read_it;
    }
  }

  metaphysicl_assert(read_it  == -1);
  metaphysicl_assert(write_it  == -1);
  metaphysicl_assert(new_indices_read_it == -1);
}


  // Since this is a dynamically allocated sparsity pattern, we can
  // decrease it when possible for efficiency
template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <typename Indices2>
METAPHYSICL_INLINE
void
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::
sparsity_intersection (const Indices2 & new_indices)
{
#ifndef NDEBUG
  // Assert sorted and no duplicates
  if (_indices.size() > 1)
    for (std::size_t i = 0; i < _indices.size() - 1; ++i)
      metaphysicl_assert(_indices[i] < _indices[i + 1]);
  if (new_indices.size() > 1)
    for (std::size_t i = 0; i < new_indices.size() - 1; ++i)
      metaphysicl_assert(new_indices[i] < new_indices[i + 1]);
#endif

#ifndef NDEBUG
  typedef typename Indices2::value_type I2;
  typedef typename CompareTypes<I,I2>::supertype max_index_type;
  auto index_it = _indices.begin();
  auto index2_it = new_indices.begin();

  max_index_type shared_indices = 0;

  const I maxI = MetaPhysicL::numeric_limits<I>::max();

  while (index2_it != new_indices.end()) {
    I idx1 = (index_it == _indices.end()) ? maxI : *index_it;
    I2 idx2 = *index2_it;

    while (idx1 < idx2) {
      ++index_it;
      idx1 = (index_it == _indices.end()) ? maxI : *index_it;
    }

    while ((idx1 == idx2) &&
           (idx1 != maxI)) {
      ++index_it;
      idx1 = (index_it == _indices.end()) ? maxI : *index_it;
      ++index2_it;
      idx2 = (index2_it == new_indices.end()) ? maxI : *index2_it;
      ++shared_indices;
    }

    while (idx2 < idx1) {
      ++index2_it;
      if (index2_it == new_indices.end())
        break;
      idx2 = *index2_it;
    }
  }
#endif

  // We'll loop up through the arrays, copying indices (and
  // corresponding data) that should be there downward into place.

  // Merged values:
  auto md_it = _data.begin();
  auto mi_it = _indices.begin();

  // Our old values:
  auto d_it = _data.begin();
  auto i_it = _indices.begin();

  // Values to merge with:
  auto i2_it = new_indices.begin();

  for (; i_it != _indices.end() && i2_it != new_indices.end();
       ++md_it, ++mi_it, ++d_it, ++i_it, ++i2_it) {
    while (*i2_it < *i_it) {
      ++i2_it;
      if (i2_it == new_indices.end())
        break;
    }
    if (i2_it == new_indices.end())
      break;
    while (*i2_it > *i_it) {
        ++i_it;
      if (i_it == _indices.end())
        break;
    }
    if (i_it == _indices.end())
      break;

    *md_it = *d_it;
    *mi_it = *i_it;
  }

  metaphysicl_assert_equal_to(md_it - _data.begin(),
                              shared_indices);
  metaphysicl_assert_equal_to(mi_it - _indices.begin(),
                              shared_indices);

  const std::size_t n_indices = md_it - _data.begin();

  _indices.resize(n_indices);
  _data.resize(n_indices);
}



  // Since this is a dynamically allocated sparsity pattern, we can
  // decrease it when possible for efficiency
template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
METAPHYSICL_INLINE
void
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::sparsity_trim (const value_type tolerance)
{
  metaphysicl_assert
    (algorithm::adjacent_find(_indices.begin(), _indices.end()) ==
     _indices.end());
#ifdef METAPHYSICL_HAVE_CXX11
  metaphysicl_assert(algorithm::is_sorted(_indices.begin(), _indices.end()));
#endif
  metaphysicl_assert(tolerance >= 0);

#ifndef NDEBUG
  I used_indices = 0;

  {
    auto index_it = _indices.begin();
    auto data_it = _data.begin();
    for (; index_it != _indices.end(); ++index_it, ++data_it)
      if (MetaPhysicL::math::abs(*data_it) > tolerance)
        ++used_indices;
  }
#endif

  // We'll loop up through the arrays, copying indices (and
  // corresponding data) that should be there downward into place.

  // Downward-merged values:
  auto md_it = _data.begin();
  auto mi_it = _indices.begin();

  // Our old values:
  auto d_it = _data.begin();

  for (auto i_it = _indices.begin();
       i_it != _indices.end(); ++i_it, ++d_it)
    if (MetaPhysicL::math::abs(*d_it) > tolerance)
      {
        *mi_it = *i_it;
        *md_it = *d_it;
        ++mi_it;
        ++md_it;
      }

  const std::size_t n_indices = md_it - _data.begin();

  metaphysicl_assert_equal_to(n_indices, used_indices);
  metaphysicl_assert_equal_to(mi_it - _indices.begin(),
                              used_indices);

  _indices.resize(n_indices);
  _data.resize(n_indices);
}

  // Not defineable since !0 != 0
  // SubType<SubTypeArgs...> operator! () const;

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <class... SubTypeArgs2>
METAPHYSICL_INLINE
SubType<SubTypeArgs...>&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::operator+= (const SubType<SubTypeArgs2...>& a)
{
  // Resize if necessary
  this->sparsity_union(a.nude_indices());

  auto data_it  = _data.begin();
  auto index_it = _indices.begin();
  auto data2_it  =
    a.nude_data().begin();
  auto index2_it =
    a.nude_indices().begin();
  for (; data2_it != a.nude_data().end(); ++data2_it, ++index2_it)
    {
      auto idx1 = *index_it;
      auto idx2 = *index2_it;

      while (idx1 < idx2) {
        ++index_it;
        ++data_it;
        metaphysicl_assert(index_it != _indices.end());
        idx1 = *index_it;
      }
      metaphysicl_assert_equal_to(idx1, idx2);

      *data_it += *data2_it;
    }

  return static_cast<SubType<SubTypeArgs...>&>(*this);
}


template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <class... SubTypeArgs2>
METAPHYSICL_INLINE
SubType<SubTypeArgs...>&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::operator-= (const SubType<SubTypeArgs2...>& a)
{
  // Resize if necessary
  this->sparsity_union(a.nude_indices());

  auto data_it  = _data.begin();
  auto index_it = _indices.begin();
  auto data2_it  =
    a.nude_data().begin();
  auto index2_it =
    a.nude_indices().begin();
  for (; data2_it != a.nude_data().end(); ++data2_it, ++index2_it)
    {
      auto idx1 = *index_it;
      auto idx2 = *index2_it;

      while (idx1 < idx2) {
        ++index_it;
        ++data_it;
        metaphysicl_assert(index_it != _indices.end());
        idx1 = *index_it;
      }
      metaphysicl_assert_equal_to(idx1, idx2);

      *data_it -= *data2_it;
    }

  return static_cast<SubType<SubTypeArgs...>&>(*this);
}


template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <class... SubTypeArgs2>
METAPHYSICL_INLINE
SubType<SubTypeArgs...>&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::operator*= (const SubType<SubTypeArgs2...>& a)
{
  // Resize if possible
  this->sparsity_intersection(a.nude_indices());

  auto data_it  = _data.begin();
  auto index_it = _indices.begin();
  auto data2_it  =
    a.nude_data().begin();
  auto index2_it =
    a.nude_indices().begin();
  for (; data2_it != a.nude_data().end(); ++data2_it, ++index2_it)
    {
      auto idx1 = *index_it;
      auto idx2 = *index2_it;

      while (idx1 < idx2) {
        ++index_it;
        ++data_it;
        metaphysicl_assert(index_it != _indices.end());
        idx1 = *index_it;
      }

      if (idx1 == idx2)
        *data_it *= *data2_it;
    }

  return static_cast<SubType<SubTypeArgs...>&>(*this);
}


template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <class... SubTypeArgs2>
METAPHYSICL_INLINE
SubType<SubTypeArgs...>&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::operator/= (const SubType<SubTypeArgs2...>& a)
{
  auto data_it  = _data.begin();
  auto index_it = _indices.begin();
  auto data2_it  =
    a.nude_data().begin();
  auto index2_it =
    a.nude_indices().begin();
  for (; data2_it != a.nude_data().end(); ++data2_it, ++index2_it)
    {
      auto idx1 = *index_it;
      auto idx2 = *index2_it;

      while (idx1 < idx2) {
        ++index_it;
        ++data_it;
        metaphysicl_assert(index_it != _indices.end());
        idx1 = *index_it;
      }

      if (idx1 == idx2)
        *data_it /= *data2_it;
    }

  return static_cast<SubType<SubTypeArgs...>&>(*this);
}


template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <typename T2>
METAPHYSICL_INLINE
SubType<SubTypeArgs...>&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::operator*= (const T2& a)
{
  std::size_t index_size = size();
  for (unsigned int i=0; i != index_size; ++i)
    _data[i] *= a;
  return static_cast<SubType<SubTypeArgs...>&>(*this);
}

template <typename Data, typename Indices, template <class...> class SubType, class... SubTypeArgs>
template <typename T2>
METAPHYSICL_INLINE
SubType<SubTypeArgs...>&
DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>::operator/= (const T2& a)
{
  std::size_t index_size = size();
  for (unsigned int i=0; i != index_size; ++i)
    _data[i] /= a;
  return static_cast<SubType<SubTypeArgs...>&>(*this);
}

//
// Non-member functions
//

template <template <class...> class SubType,
          typename BoolData,
          typename BoolIndices,
          class... BoolSubTypeArgs,
          typename Data,
          typename Indices,
          class... SubTypeArgs,
          typename Data2,
          typename Indices2,
          class... SubTypeArgs2>
METAPHYSICL_INLINE typename SubType<SubTypeArgs...>::template rebind<
    typename CompareTypes<typename Data::value_type, typename Data2::value_type>::supertype,
    typename CompareTypes<typename Indices::value_type,
                          typename Indices2::value_type>::supertype>::other
if_else(
    const DynamicSparseNumberBase<BoolData, BoolIndices, SubType, BoolSubTypeArgs...> & condition,
    const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & if_true,
    const DynamicSparseNumberBase<Data2, Indices2, SubType, SubTypeArgs2...> & if_false)
{
  metaphysicl_assert
    (algorithm::adjacent_find(condition.nude_indices().begin(), condition.nude_indices().end()) ==
     condition.nude_indices().end());
  metaphysicl_assert
    (algorithm::adjacent_find(if_true.nude_indices().begin(), if_true.nude_indices().end()) ==
     if_true.nude_indices().end());
  metaphysicl_assert
    (algorithm::adjacent_find(if_false.nude_indices().begin(), if_false.nude_indices().end()) ==
     if_false.nude_indices().end());
#ifdef METAPHYSICL_HAVE_CXX11
  metaphysicl_assert(algorithm::is_sorted(condition.nude_indices().begin(), condition.nude_indices().end()));
  metaphysicl_assert(algorithm::is_sorted(if_true.nude_indices().begin(), if_true.nude_indices().end()));
  metaphysicl_assert(algorithm::is_sorted(if_false.nude_indices().begin(), if_false.nude_indices().end()));
#endif

  typedef
      typename CompareTypes<typename Data::value_type, typename Data2::value_type>::supertype TS;
  typedef
      typename CompareTypes<typename Indices::value_type, typename Indices2::value_type>::supertype
          IS;

  typename SubType<SubTypeArgs...>::template rebind<TS, IS>::other returnval;

  // First count returnval size
  IS required_size = 0;
  {
    auto indexcond_it      = condition.nude_indices().begin();
    auto datacond_it        = condition.nude_data().begin();
    auto indextrue_it       = if_true.nude_indices().begin();
    const auto endtrue_it   = if_true.nude_indices().end();
    auto datatrue_it        = if_true.nude_data().begin();
    auto indexfalse_it     = if_false.nude_indices().begin();
    const auto endfalse_it = if_false.nude_indices().end();
    auto datafalse_it      = if_false.nude_data().begin();

    for (; indexcond_it != condition.nude_indices().end(); ++indexcond_it, ++datacond_it)
     {
       while (indexfalse_it != endfalse_it &&
              *indexfalse_it < *indexcond_it)
         {
           if (*datafalse_it)
             ++required_size;

           ++indexfalse_it;
           ++datafalse_it;
         }

       if (*datacond_it)
         {
           while (indextrue_it != endtrue_it &&
                  *indextrue_it < *indexcond_it)
             {
               ++indextrue_it;
               ++datatrue_it;
             }
           if (indextrue_it != endtrue_it &&
               *indextrue_it == *indexcond_it &&
               *datatrue_it)
             {
               ++required_size;
               ++indextrue_it;
               ++datatrue_it;
             }
           if (*indexfalse_it == *indexcond_it)
             {
               ++indexfalse_it;
               ++datafalse_it;
             }
         }
       else
         {
           if (indexfalse_it != endfalse_it &&
               *indexfalse_it == *indexcond_it &&
               *datafalse_it)
             {
               ++required_size;
               ++indexfalse_it;
               ++datafalse_it;
             }
         }
     }
  }

  // Then fill returnval
  returnval.resize(required_size);
  {
    auto indexcond_it      = condition.nude_indices().begin();
    auto datacond_it        = condition.nude_data().begin();
    auto indextrue_it       = if_true.nude_indices().begin();
    const auto endtrue_it   = if_true.nude_indices().end();
    auto datatrue_it        = if_true.nude_data().begin();
    auto indexfalse_it     = if_false.nude_indices().begin();
    const auto endfalse_it = if_false.nude_indices().end();
    auto datafalse_it      = if_false.nude_data().begin();

    auto indexreturn_it          = returnval.nude_indices().begin();
    auto datareturn_it           = returnval.nude_data().begin();

    for (; indexcond_it != condition.nude_indices().end(); ++indexcond_it, ++datacond_it)
     {
       while (indexfalse_it != endfalse_it &&
              *indexfalse_it < *indexcond_it)
         {
           if (*datafalse_it)
             {
               *indexreturn_it = *indexfalse_it;
               *datareturn_it  = *datafalse_it;
               ++indexreturn_it;
               ++datareturn_it;
             }

           ++indexfalse_it;
           ++datafalse_it;
         }

       if (*datacond_it)
         {
           while (indextrue_it != endtrue_it &&
                  *indextrue_it < *indexcond_it)
             {
               ++indextrue_it;
               ++datatrue_it;
             }
           if (indextrue_it != endtrue_it &&
               *indextrue_it == *indexcond_it &&
               *datatrue_it)
             {
               *indexreturn_it = *indextrue_it;
               *datareturn_it  = *datatrue_it;
               ++indexreturn_it;
               ++datareturn_it;
               ++indextrue_it;
               ++datatrue_it;
             }
           if (*indexfalse_it == *indexcond_it)
             {
               ++indexfalse_it;
               ++datafalse_it;
             }
         }
       else
         {
           if (indexfalse_it != endfalse_it &&
               *indexfalse_it == *indexcond_it &&
               *datafalse_it)
             {
               *indexreturn_it = *indexfalse_it;
               *datareturn_it  = *datafalse_it;
               ++indexreturn_it;
               ++datareturn_it;
               ++indexfalse_it;
               ++datafalse_it;
             }
         }
     }
  }

  metaphysicl_assert
    (algorithm::adjacent_find(returnval.nude_indices().begin(), returnval.nude_indices().end()) ==
     returnval.nude_indices().end());
#ifdef METAPHYSICL_HAVE_CXX11
  metaphysicl_assert(algorithm::is_sorted(returnval.nude_indices().begin(), returnval.nude_indices().end()));
#endif

  return returnval;
}



#define DynamicSparseNumberBase_op_ab(opname, subtypename, functorname) \
  template <class... AArgs, class... BArgs> \
METAPHYSICL_INLINE \
typename Symmetric##functorname##Type<subtypename<AArgs...>, \
                                      subtypename<BArgs...>>::supertype \
operator opname (const subtypename<AArgs...> & a, const subtypename<BArgs...> & b) \
{ \
  typedef typename Symmetric##functorname##Type<subtypename<AArgs...>,  \
                                                subtypename<BArgs...>>::supertype type; \
  type returnval = a; \
  returnval opname##= b; \
  return returnval; \
}


#ifdef METAPHYSICL_USE_STD_MOVE

#define DynamicSparseNumberBase_op(subtypename, opname, functorname) \
DynamicSparseNumberBase_op_ab(opname, subtypename, functorname) \
 \
template <class... AArgs, class... BArgs> \
METAPHYSICL_INLINE typename Symmetric##functorname##Type<subtypename<AArgs...>, \
                                             subtypename<BArgs...>>::supertype \
operator opname(subtypename<AArgs...> && a, const subtypename<BArgs...> & b) \
{ \
  typedef typename Symmetric##functorname##Type<subtypename<AArgs...>, \
                                                subtypename<BArgs...>>::supertype type; \
  type returnval = std::move(a); \
  returnval opname##= b; \
  return returnval; \
}

#else

#define DynamicSparseNumberBase_op(subtypename, opname, functorname) \
DynamicSparseNumberBase_op_ab(opname, subtypename, functorname)

#endif

// Let's also allow scalar times vector.
// Scalar plus vector, etc. remain undefined in the sparse context.

template <template <class...> class SubType,
          typename Data,
          typename Indices,
          class... SubTypeArgs,
          typename T>
METAPHYSICL_INLINE typename MultipliesType<SubType<SubTypeArgs...>, T, true>::supertype
operator*(const T & a, const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & b)
{
  const unsigned int index_size = b.size();

  typename MultipliesType<SubType<SubTypeArgs...>,T,true>::supertype
    returnval;
  returnval.resize(index_size);

  for (unsigned int i=0; i != index_size; ++i) {
    returnval.raw_at(i) = a * b.raw_at(i);
    returnval.raw_index(i) = b.raw_index(i);
  }

  return returnval;
}

template <template <class...> class SubType,
          typename Data,
          typename Indices,
          class... SubTypeArgs,
          typename T>
METAPHYSICL_INLINE typename MultipliesType<SubType<SubTypeArgs...>, T>::supertype
operator*(const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & a, const T & b)
{
  const unsigned int index_size = a.size();

  typename MultipliesType<SubType<SubTypeArgs...>,T>::supertype
    returnval;
  returnval.resize(index_size);

  for (unsigned int i=0; i != index_size; ++i) {
    returnval.raw_at(i) = a.raw_at(i) * b;
    returnval.raw_index(i) = a.raw_index(i);
  }
  return returnval;
}

template <template <class...> class SubType,
          typename Data,
          typename Indices,
          class... SubTypeArgs,
          typename T>
METAPHYSICL_INLINE typename DividesType<SubType<SubTypeArgs...>, T>::supertype
operator/(const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & a, const T & b)
{
  const unsigned int index_size = a.size();

  typename DividesType<SubType<SubTypeArgs...>,T>::supertype returnval;
  returnval.resize(index_size);

  for (unsigned int i=0; i != index_size; ++i) {
    returnval.raw_at(i) = a.raw_at(i) / b;
    returnval.raw_index(i) = a.raw_index(i);
  }

  return returnval;
}

#if __cplusplus >= 201103L
template <template <class...> class SubType,
          typename Data,
          typename Indices,
          class... SubTypeArgs,
          typename T>
METAPHYSICL_INLINE typename MultipliesType<SubType<SubTypeArgs...>, T>::supertype
operator*(DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> && a, const T & b)
{
  typename MultipliesType<SubType<SubTypeArgs...>,T>::supertype
    returnval = std::move(static_cast<SubType<SubTypeArgs...>&&>(a));

  returnval *= b;

  return returnval;
}

template <template <class...> class SubType,
          typename Data,
          typename Indices,
          class... SubTypeArgs,
          typename T>
METAPHYSICL_INLINE typename DividesType<SubType<SubTypeArgs...>, T>::supertype
operator/(DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> && a, const T & b)
{
  typename DividesType<SubType<SubTypeArgs...>,T>::supertype
    returnval = std::move(static_cast<SubType<SubTypeArgs...>&&>(a));

  returnval /= b;

  return returnval;
}
#endif


#define DynamicSparseNumberBase_operator_binary(opname, functorname) \
template <template <class...> class SubType, \
          typename Data, \
          typename Indices, \
          class... SubTypeArgs, \
          typename Data2, \
          typename Indices2, \
          class... SubTypeArgs2> \
METAPHYSICL_INLINE typename SubType<SubTypeArgs...>::template rebind< \
  bool, \
  typename CompareTypes<typename Indices::value_type, \
                        typename Indices2::value_type>::supertype>::other \
operator opname(const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & a, \
                const DynamicSparseNumberBase<Data2, Indices2, SubType, SubTypeArgs2...> & b) \
{ \
  typedef typename Data::value_type T; \
  typedef typename Data2::value_type T2; \
  typedef typename Indices::value_type I; \
  typedef typename Indices2::value_type I2; \
  typedef typename SymmetricCompareTypes<T, T2>::supertype TS; \
  typedef typename CompareTypes<I, I2>::supertype IS; \
  typedef typename SubType<SubTypeArgs...>::template rebind< \
    bool, \
    typename CompareTypes<typename Indices::value_type, \
                          typename Indices2::value_type>::supertype>::other RetType; \
  RetType returnval; \
  returnval.nude_indices() = a.nude_indices(); \
  returnval.nude_data().resize(a.nude_indices().size()); \
  returnval.sparsity_union(b.nude_indices()); \
 \
  auto  index_a_it = a.nude_indices().begin(); \
  auto index_b_it = b.nude_indices().begin(); \
  auto     index_out_it = returnval.nude_indices().begin(); \
 \
  auto  data_a_it = a.nude_data().begin(); \
  auto data_b_it = b.nude_data().begin(); \
  auto     data_out_it = returnval.nude_data().begin(); \
 \
  const IS  maxIS  = MetaPhysicL::numeric_limits<IS>::max(); \
 \
  for (; index_out_it != returnval.nude_indices().end(); ++index_out_it, ++data_out_it) { \
    const IS index_a = (index_a_it == a.nude_indices().end()) ? maxIS : *index_a_it; \
    const IS index_b = (index_b_it == b.nude_indices().end()) ? maxIS : *index_b_it; \
    const IS index_out = *index_out_it; \
    const TS data_a  = (index_a_it == a.nude_indices().end()) ? 0: *data_a_it; \
    const TS data_b  = (index_b_it == b.nude_indices().end()) ? 0: *data_b_it; \
    TS & data_out = *data_out_it; \
 \
    if (index_a == index_out) { \
      if (index_b == index_out) { \
        data_out = data_a opname data_b; \
        index_b_it++; \
        data_b_it++; \
      } else { \
        data_out = data_a opname 0; \
      } \
      index_a_it++; \
      data_a_it++; \
    } else { \
      metaphysicl_assert_equal_to(index_b, index_out); \
      data_out = 0 opname data_b; \
      index_b_it++; \
      data_b_it++; \
    } \
  } \
 \
  return returnval; \
} \
template <template <class...> class SubType, \
          typename Data, \
          typename Indices, \
          class... SubTypeArgs, \
          typename T> \
METAPHYSICL_INLINE typename SubType<SubTypeArgs...>::template rebind<bool>::other operator opname( \
  const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & a, const T & b) \
{ \
  typename SubType<SubTypeArgs...>::template rebind<bool>::other returnval; \
 \
  std::size_t index_size = a.size(); \
  returnval.resize(index_size); \
  returnval.nude_indices() = a.nude_indices(); \
 \
  for (unsigned int i=0; i != index_size; ++i) \
    returnval.raw_at(i) = (a.raw_at(i) opname b); \
 \
  return returnval; \
} \
template <template <class...> class SubType, \
          typename Data, \
          typename Indices, \
          class... SubTypeArgs, \
          typename T> \
METAPHYSICL_INLINE typename SubType<SubTypeArgs...>::template rebind<bool>::other operator opname( \
    const T & a, const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & b) \
{ \
  typename SubType<SubTypeArgs...>::template rebind<bool>::other returnval; \
 \
  std::size_t index_size = a.size(); \
  returnval.nude_indices() = a.nude_indices(); \
  returnval.nude_data().resize(index_size); \
 \
  for (unsigned int i=0; i != index_size; ++i) \
    returnval.raw_at(i) = (a opname b.raw_at(i)); \
 \
  return returnval; \
}

// NOTE: unary functions for which 0-op-0 is true are undefined compile-time
// errors, because there's no efficient way to have them make sense in
// the sparse context.

DynamicSparseNumberBase_operator_binary(<, less)
// DynamicSparseNumberBase_operator_binary(<=)
DynamicSparseNumberBase_operator_binary(>, greater)
// DynamicSparseNumberBase_operator_binary(>=)
// DynamicSparseNumberBase_operator_binary(==)
DynamicSparseNumberBase_operator_binary(!=, not_equal_to)

// FIXME - make && an intersection rather than a union for efficiency
DynamicSparseNumberBase_operator_binary(&&, logical_and)
DynamicSparseNumberBase_operator_binary(||, logical_or)


template <template <typename...> class SubType, typename Data, typename Indices, typename... SubTypeArgs>
inline
std::ostream&
operator<< (std::ostream& output, const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...>& a)
{
  // Enclose the entire output in braces
  output << '{';

  std::size_t index_size = a.size();

  // Output the first value from a non-empty set
  // All values are given as ordered (index, value) pairs
  if (index_size)
    output << '(' << a.raw_index(0) << ',' <<
              a.raw_at(0) << ')';

  // Output the comma-separated subsequent values from a non-singleton
  // set
  for (unsigned int i = 1; i < index_size; ++i)
    {
      output << ", (" << a.raw_index(i) << ',' << a.raw_data()[i] << ')';
    }
  output << '}';
  return output;
}


// For backwards compatibility we still allow violating the C++
// standard by putting our partial template specializations into
// namespace std.
#ifdef METAPHYSICL_ENABLE_STD_VIOLATION
} // namespace MetaPhysicL

namespace std {

namespace math = MetaPhysicL::math;
using MetaPhysicL::CompareTypes;
using MetaPhysicL::DynamicSparseNumberBase;
using MetaPhysicL::SymmetricCompareTypes;
#endif


#define DynamicSparseNumberBase_std_unary(funcname) \
template <template <class...> class SubType, \
          typename Data, typename Indices, class... SubTypeArgs> \
METAPHYSICL_INLINE \
SubType<SubTypeArgs...> \
funcname (const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & a) \
{ \
  std::size_t index_size = a.size(); \
  SubType<SubTypeArgs...> returnval; \
  returnval.nude_indices() = a.nude_indices(); \
  returnval.nude_data().resize(index_size); \
  for (unsigned int i=0; i != index_size; ++i) \
    returnval.raw_at(i) = math::funcname(a.raw_at(i)); \
 \
  return returnval; \
}

#define DynamicSparseNumberBase_std_binary_union(funcname) \
template <template <class...> class SubType, \
          typename Data, \
          typename Indices, \
          class... SubTypeArgs, \
          typename Data2, \
          typename Indices2, \
          class... SubTypeArgs2> \
METAPHYSICL_INLINE typename SubType<SubTypeArgs...>::template rebind< \
    typename SymmetricCompareTypes<typename Data::value_type, \
                                   typename Data2::value_type>::supertype, \
    typename CompareTypes<typename Indices::value_type, \
                          typename Indices2::value_type>::supertype>::other \
funcname(const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & a, \
         const DynamicSparseNumberBase<Data2, Indices2, SubType, SubTypeArgs2...> & b) \
{ \
  typedef typename Data::value_type T; \
  typedef typename Data2::value_type T2; \
  typedef typename Indices::value_type I; \
  typedef typename Indices2::value_type I2; \
  typedef typename SymmetricCompareTypes<T, T2>::supertype TS; \
  typedef typename CompareTypes<I, I2>::supertype IS; \
  typedef typename SubType<SubTypeArgs...>::template rebind< \
      typename SymmetricCompareTypes<typename Data::value_type, \
                                     typename Data2::value_type>::supertype, \
      typename CompareTypes<typename Indices::value_type, \
                            typename Indices2::value_type>::supertype>::other RetType; \
  RetType returnval; \
 \
  std::size_t index_size = a.nude_indices.size(); \
  returnval.nude_indices = a.nude_indices; \
  returnval.nude_data.resize(index_size); \
  returnval.sparsity_union(b.nude_indices); \
 \
  auto  index_a_it = a.nude_indices.begin(); \
  auto index_b_it = b.nude_indices.begin(); \
  auto     index_out_it = returnval.nude_indices.begin(); \
 \
  auto  data_a_it = a.nude_data.begin(); \
  auto data_b_it = b.nude_data.begin(); \
  auto     data_out_it = returnval.nude_data.begin(); \
 \
  const IS  maxIS  = MetaPhysicL::numeric_limits<IS>::max(); \
 \
  for (; index_out_it != returnval.nude_indices.end(); ++index_out_it, ++data_out_it) { \
    const IS index_a = (index_a_it == a.nude_indices.end()) ? maxIS : *index_a_it; \
    const IS index_b = (index_b_it == b.nude_indices.end()) ? maxIS : *index_b_it; \
    const IS index_out = *index_out_it; \
    const TS data_a  = (index_a_it == a.nude_indices.end()) ? 0: *data_a_it; \
    const TS data_b  = (index_b_it == b.nude_indices.end()) ? 0: *data_b_it; \
    TS & data_out = *data_out_it; \
 \
    if (index_a == index_out) { \
      if (index_b == index_out) { \
        data_out = math::funcname(data_a, data_b); \
        index_b_it++; \
        data_b_it++; \
      } else { \
        data_out = math::funcname(data_a, 0); \
      } \
      index_a_it++; \
      data_a_it++; \
    } else { \
      metaphysicl_assert_equal_to(index_b, index_out); \
      data_out = math::funcname(0, data_b); \
      index_b_it++; \
      data_b_it++; \
    } \
  } \
 \
  return returnval; \
} \
 \
template <template <class...> class SubType, \
          typename Data, \
          typename Indices, \
          class... SubTypeArgs, \
          typename T2> \
METAPHYSICL_INLINE typename SubType<SubTypeArgs...>::template rebind< \
    typename SymmetricCompareTypes<typename Data::value_type, T2>::supertype, \
    typename Indices::value_type>::other \
funcname(const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & a, \
         const T2 & b) \
{ \
  typedef typename Data::value_type T; \
  typedef typename SymmetricCompareTypes<T, T2>::supertype TS; \
  typedef \
      typename SubType<SubTypeArgs...>::template rebind<TS, typename Indices::value_type>::other \
          RetType; \
  RetType returnval; \
 \
  std::size_t index_size = a.size(); \
  returnval.resize(index_size); \
  returnval.nude_indices = a.nude_indices; \
 \
  for (unsigned int i=0; i != index_size; ++i) \
    returnval.raw_at(i) = math::funcname(a.raw_at(i), b); \
 \
  return returnval; \
} \
 \
template <template <class...> class SubType, \
          typename Data, \
          typename Indices, \
          class... SubTypeArgs, \
          typename T> \
METAPHYSICL_INLINE typename SubType<SubTypeArgs...>::template rebind< \
    typename SymmetricCompareTypes<T, typename Data::value_type>::supertype, \
    typename Indices::value_type>::other \
funcname(const T & a, const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & b) \
{ \
  typedef typename Data::value_type T2; \
  typedef typename SymmetricCompareTypes<T, T2>::supertype TS; \
  typedef \
      typename SubType<SubTypeArgs...>::template rebind<TS, typename Indices::value_type>::other \
          RetType; \
  RetType returnval; \
 \
  std::size_t index_size = a.size(); \
  returnval.resize(index_size); \
  returnval.nude_indices = b.nude_indices; \
 \
  for (unsigned int i=0; i != index_size; ++i) \
    returnval.raw_at(i) = math::funcname(a, b.raw_at(i)); \
 \
  return returnval; \
}

// Pow needs its own specialization, both to avoid being confused by
// pow<T1,T2> and because pow(x,0) isn't 0.
template <template <class...> class SubType,
          typename Data,
          typename Indices,
          class... SubTypeArgs,
          typename T2>
METAPHYSICL_INLINE typename SubType<SubTypeArgs...>::template rebind<
    typename SymmetricCompareTypes<typename Data::value_type, T2>::supertype,
    typename Indices::value_type>::other
pow(const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & a, const T2 & b)
{
  typedef typename Data::value_type T;
  typedef typename SymmetricCompareTypes<T, T2>::supertype TS;
  typedef typename SubType<SubTypeArgs...>::template rebind<TS, typename Indices::value_type>::other
      RetType;
  RetType returnval;

  std::size_t index_size = a.size();
  returnval.nude_indices() = a.nude_indices();
  returnval.nude_data().resize(index_size);

  for (unsigned int i=0; i != index_size; ++i)
    returnval.raw_at(i) = math::pow(a.raw_at(i), b);

  return returnval;
}


// NOTE: unary functions for which f(0) != 0 are undefined compile-time
// errors, because there's no efficient way to have them make sense in
// the sparse context.

// DynamicSparseNumberBase_std_binary(pow) // separate definition
// DynamicSparseNumberBase_std_unary(exp)
// DynamicSparseNumberBase_std_unary(log)
// DynamicSparseNumberBase_std_unary(log10)
DynamicSparseNumberBase_std_unary(sin)
// DynamicSparseNumberBase_std_unary(cos)
DynamicSparseNumberBase_std_unary(tan)
DynamicSparseNumberBase_std_unary(asin)
// DynamicSparseNumberBase_std_unary(acos)
DynamicSparseNumberBase_std_unary(atan)
DynamicSparseNumberBase_std_binary_union(atan2)
DynamicSparseNumberBase_std_unary(sinh)
// DynamicSparseNumberBase_std_unary(cosh)
DynamicSparseNumberBase_std_unary(tanh)
DynamicSparseNumberBase_std_unary(sqrt)
DynamicSparseNumberBase_std_unary(abs)
DynamicSparseNumberBase_std_unary(fabs)
DynamicSparseNumberBase_std_binary_union(max)
DynamicSparseNumberBase_std_binary_union(min)
DynamicSparseNumberBase_std_unary(ceil)
DynamicSparseNumberBase_std_unary(floor)
DynamicSparseNumberBase_std_binary_union(fmod) // TODO: optimize this

#if __cplusplus >= 201103L
DynamicSparseNumberBase_std_unary(expm1)
DynamicSparseNumberBase_std_unary(cbrt)
DynamicSparseNumberBase_std_unary(asinh)
DynamicSparseNumberBase_std_unary(atanh)
DynamicSparseNumberBase_std_unary(erf)
DynamicSparseNumberBase_std_unary(trunc)
DynamicSparseNumberBase_std_unary(round)
DynamicSparseNumberBase_std_unary(nearbyint)
DynamicSparseNumberBase_std_unary(rint)

DynamicSparseNumberBase_std_binary_union(remainder) // TODO: optimize this
DynamicSparseNumberBase_std_binary_union(fmax)
DynamicSparseNumberBase_std_binary_union(fmin)
DynamicSparseNumberBase_std_binary_union(fdim)
DynamicSparseNumberBase_std_binary_union(hypot)
#endif // __cplusplus >= 201103L

#define DynamicSparseNumberBase_std_unary_rebind(funcname) \
template <template <class...> class SubType, \
          typename Data, \
          typename Indices, \
          class... SubTypeArgs> \
METAPHYSICL_INLINE auto funcname(const DynamicSparseNumberBase<Data, Indices, SubType, SubTypeArgs...> & in) \
    ->typename SubType<SubTypeArgs...>::template rebind<decltype(math::funcname( \
                                                            typename Data::value_type())), \
                                                        typename Indices::value_type>::other \
{ \
  typedef typename SubType<SubTypeArgs...>::template rebind< \
      decltype(math::funcname(typename Data::value_type())), \
      typename Indices::value_type>::other RetType; \
  RetType returnval; \
  auto size = in.size(); \
  returnval.nude_indices() = in.nude_indices(); \
  returnval.nude_data().resize(size); \
 \
  for (decltype(size) i = 0; i < size; ++i) \
    returnval.raw_at(i) = math::funcname(in.raw_at(i));  \
  return returnval; \
}

DynamicSparseNumberBase_std_unary_rebind(real)
DynamicSparseNumberBase_std_unary_rebind(imag)
DynamicSparseNumberBase_std_unary_rebind(norm)

#if __cplusplus >= 201103L
DynamicSparseNumberBase_std_unary_rebind(isfinite)
DynamicSparseNumberBase_std_unary_rebind(isinf)
DynamicSparseNumberBase_std_unary_rebind(isnan)
DynamicSparseNumberBase_std_unary_rebind(isnormal)
DynamicSparseNumberBase_std_unary_rebind(signbit)
#endif // __cplusplus >= 201103L

} // namespace std (deprecated) or MetaPhysicL


#endif // METAPHYSICL_DYNAMICSPARSENUMBERARRAY_H
