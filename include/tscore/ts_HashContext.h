/** @file Basic hash function support.

  @section license License

  Licensed to the Apache Software Foundation (ASF) under one or more contributor license agreements.
  See the NOTICE file distributed with this work for additional information regarding copyright
  ownership.  The ASF licenses this file to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance with the License.  You may obtain a
  copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software distributed under the License
  is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
  or implied. See the License for the specific language governing permissions and limitations under
  the License.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <cctype>
#include "swoc/MemSpan.h"
#include "swoc/TextView.h"

namespace ts
{
/** Base protocol class for hash functors.
 *
 * Each specific hash function embedded in a hash functor is a subclass of this class and
 * follows this API. Subclasses should override the return type to return the subclass type.
 *
 * The main purpose of this is to allow run time changes in hashing, which is required in various
 * circumstances.
 */
struct HashContext {
  using self_type = HashContext; ///< Self reference type.

  /// Pass a single byte to the hashing function.
  virtual self_type &update(uint8_t v) = 0;

  /// Pass @a data to the hashing function.
  virtual self_type &update(std::string_view const &data) = 0;

  /// Finalize the hash function output.
  virtual self_type & final() = 0;

  /// Reset the hash function state.
  virtual self_type &clear() = 0;

  /// Get the size of the resulting hash value.
  virtual size_t size() const = 0;

  /// Store the result in @a dst.
  /// @a dst must be at least @c size() bytes long.
  /// @return @c true if the result was copied to @a data, @c false otherwise.
  virtual bool store(swoc::MemSpan<void> dst) const = 0;

  /** Pass data from transformed @a view to the hash function.
   *
   * The output transform type @a V must be convertible to a @c uint8_t.
   */
  template <typename X, typename V> auto update(swoc::TransformView<X,V> view) -> std::enable_if<std::is_constructible<uint8_t, V>::value, self_type&>;

  virtual ~HashContext() = default; ///< Force virtual destructor.
};

template < typename T > struct SizedHashContext : public HashContext {
protected:
  using self_type = SizedHashContext;

public:
  using value_type = T;

  // Co-vary the return type.
  self_type &update(uint8_t v) override = 0;
  self_type &update(std::string_view const &data) override = 0;

  self_type & final() override = 0;

  self_type &clear() override = 0;

  virtual value_type get() const = 0;

  /// Get the size of the resulting hash value.
  size_t size() const override;

  /** Immediately produce a hash value from @a data.
   *
   * @param data Hash input data/
   * @return Hash value.
   *
   * This is a convenience method for when all the data to hash is already available.
   */
  value_type hash_immediate(std::string_view const &data);

  /** Hash immediate data from a transformed @a view.
   *
   * The output transform type @a V must be convertible to a @c uint8_t.
   */
  template <typename X, typename V> auto hash_immediate(swoc::TransformView<X, V> const &view) -> std::enable_if<std::is_constructible<uint8_t, V>::value, value_type>;
};

using Hash32Context = SizedHashContext<uint32_t>;

using Hash64Context = SizedHashContext<uint64_t>;

template < typename T, size_t ... IDX > T sigma(T const * data) {
  return 0 + 

template < unsigned N > union LargeHashResult {
  uint8_t byte[N];
  uint64_t word[N/8];

  uint64_t fold() const;
};

template < unsigned N > LargeHashResult<N>::fold() const {
  return 0 ^ std
}

using Hash128Result = LargeHashResult<16>;
using Hash128Context = SizedHashContext<Hash128Result>;

using Hash256Result = LargeHashResult<32>;
using Hash256Context = SizedHashContext<Hash256Result>;

// ----
// Implementation

template <typename X, typename V> auto HashContext::update(swoc::TransformView<X,V> view) -> std::enable_if<std::is_constructible<uint8_t, V>::value, self_type &> {
  while (!view.empty()) {
    this->update(uint8_t(*view));
  }
  return *this;
}

template < typename T > size_t SizedHashContext<T>::size() const { return sizeof(value_type); }

template < typename T > auto SizedHashContext<T>::hash_immediate(std::string_view const& data) -> value_type {
  return this->update(data).final().get();
}

template < typename T >
template < typename X, typename V> auto SizedHashContext<T>::hash_immediate(swoc::TransformView<X, V> const &view) -> std::enable_if<std::is_constructible<uint8_t, V>::value, value_type> {
  return this->update(view).final().get();
}

} // namespace ts
