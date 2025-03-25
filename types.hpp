// Copyright (C) 2025 TLExpress.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>
#include <unordered_map>

namespace ssharp::types
{

using buff_t = std::vector<uint8_t>;
using path_t = std::filesystem::path;
using paths_t = std::set<path_t>;
using pos_t = size_t;
using span_source_t = std::variant<buff_t, path_t>;
using span_attr_t = std::pair<pos_t, size_t>;
using hash_t = uint64_t;
using salt_t = uint16_t;
using hash_attr_t = std::pair<hash_t, salt_t>;
using entry_key_t = std::variant<path_t, hash_attr_t>;
using crc32_t = uint32_t;
using adler32_t = uint32_t;
using dictionary_t = std::pair<salt_t, std::unordered_map<hash_t, path_t>>;

enum class is_absolute_path_t
{
    relative,
    absolute
};

enum class compress_type_t
{
    no = 0,
    zlib = 1,
    raw = 2,
    gzip = 3,
    zstd = 4
};

enum class is_directory_t
{
    file,
    directory
};

enum class is_encrypted_t
{
    encrypted,
    decrypted
};

enum class sii_status_t
{
    text,
    binary,
    encrypted,
    _3nk
};

enum class file_type_t
{
    generic,
    sii,
    directory,
    mat,
    pmd,
    tobj,
    soundref,
};

using parsed_path_t = std::tuple<path_t, is_absolute_path_t, is_directory_t, std::optional<hash_attr_t>>;
using parsed_paths_t = std::set<parsed_path_t>;

struct compress_attr_t
{
    compress_type_t compress_type;
    size_t uncompressed_size;
};

} // namespace ssharp::types
