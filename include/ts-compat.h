/** @file

  Traffic Server compatibility logic.

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

namespace ts {

/** Container for standard two part version number.
 */
struct VersionNumber {
  /// Construct invalid (0.0) version.
  VersionNumber() = default;
  /// Construct explicit version.
  explicit VersionNumber(unsigned short major, unsigned short minor = 0);

  // Can't use unadorned "major" because that's a macro.
  unsigned short _major = 0; ///< Major version.
  unsigned short _minor = 0; ///< Minor version.
};

inline VersionNumber::VersionNumber(unsigned short major, unsigned short minor) : _major(major), _minor(minor) {}

inline bool operator < (VersionNumber const& lhs, VersionNumber const& rhs) {
  return lhs._major < rhs._major || (lhs._major == rhs._major && lhs._minor < rhs._minor);
}

inline bool operator == (VersionNumber const& lhs, VersionNumber const& rhs) {
  return lhs._major == rhs._major && lhs._minor == rhs._minor;
}

inline bool operator != (VersionNumber const& lhs, VersionNumber const& rhs) {
  return !(lhs == rhs);
}

inline bool operator > (VersionNumber const& lhs, VersionNumber const& rhs) {
  return rhs < lhs;
}

inline bool operator <= (VersionNumber const& lhs, VersionNumber const& rhs) {
  return !(lhs > rhs);
}

inline bool operator >= (VersionNumber const& lhs, VersionNumber const& rhs) {
  return !(rhs > lhs);
}

}; // namespace ts
