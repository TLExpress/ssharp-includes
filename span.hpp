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

#include "exceptions.hpp"
#include "types.hpp"

#include <concepts>
#include <expected>

namespace ssharp::span
{

using namespace ssharp::types;
namespace ssexcept = ssharp::exceptions;

template <typename t>
concept span_source_t_c = std::same_as<t, buff_t> || std::same_as<t, path_t> ||
                          std::same_as<t, span_source_t>;

template <typename t>
concept buff_t_c = std::same_as<t, buff_t>;

class span_error : public ssexcept::exception
{
  public:
    span_error(const std::string& msg) : exception("span_error: " + msg), msg(msg) {}

    const char* what() const noexcept override
    {
        return msg.c_str();
    }

  private:
    std::string msg;
};

class span_t
{
  public:
    span_t() = delete;
    span_t(const span_t&) = default;
    span_t(span_t&&) = default;
    span_t& operator=(const span_t&) = default;
    span_t& operator=(span_t&&) = default;

    template <buff_t_c buff_t_t>
    span_t(buff_t_t&& buff, std::optional<span_attr_t> attr = std::nullopt)
    {
        if (attr && attr->first + attr->second > buff.size())
        {
            throw span_error("out of range");
        }
        source = std::forward<buff_t_t>(buff);
        this->attr = attr.value_or(std::make_pair(0, buff.size()));
    }

    span_t(const path_t& path, std::optional<span_attr_t> attr = std::nullopt)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            throw std::ios::failure("failed to open file: " + path.string());
        }
        file.seekg(0, std::ios::end);
        auto total_size = file.tellg();
        if (total_size == -1)
        {
            throw std::ios::failure("failed to get file size: " + path.string());
        }
        auto [pos, size] = attr.value_or(std::make_pair(0, total_size));
        auto u_pos = static_cast<size_t>(pos);
        auto u_total_size = static_cast<size_t>(total_size);
        if (u_pos + size > u_total_size)
        {
            throw span_error("out of range");
        }
        source = path;
        this->attr = std::make_pair(u_pos, size);
    }
    
    span_t(const span_t& span, span_attr_t attr)
    {
        auto [span_pos, span_size] = span.attr;
        auto [pos, size] = attr;
        if (static_cast<size_t>(pos) + size > span_size)
        {
            throw span_error("out of range");
        }
        if (std::holds_alternative<buff_t>(span.source))
        {
            auto& buff_ref = std::get<buff_t>(span.source);
            auto buff = buff_t(buff_ref.begin() + span_pos + pos,
                               buff_ref.begin() + span_pos + pos + size);
            this->source = buff;
            this->attr = {0, size};
        }
        else if (std::holds_alternative<path_t>(span.source))
        {
            this->source = span.source;
            this->attr = {span_pos + pos, size};
        }
        throw span_error("Invalid span source");
    }

    buff_t operator*() const
    {
        return get();
    }

    buff_t get(std::optional<span_attr_t> part = std::nullopt) const
    {
        auto [eventual_pos, eventual_size] = attr;
        if (part)
        {
            auto [pos, size] = *part;
            if (static_cast<size_t>(pos) + size > eventual_size)
            {
                throw span_error("Out of range error");
            }
            eventual_pos += pos;
            eventual_size = size;
        }
        if (std::holds_alternative<buff_t>(source))
        {
            auto& buff = std::get<buff_t>(source);
            return buff_t(buff.begin() + eventual_pos,
                          buff.begin() + eventual_pos + eventual_size);
        }
        else if (std::holds_alternative<path_t>(source))
        {
            const auto& path = std::get<path_t>(source);
            std::ifstream file(path, std::ios::binary);
            if (!file.is_open())
            {
                throw std::ios::failure(
                    "Failed to open file: " + path.string());
            }
            file.seekg(eventual_pos);
            buff_t buff(eventual_size);
            file.read(reinterpret_cast<char*>(buff.data()), buff.size());
            return buff;
        }
        throw span_error("Invalid span source");
    }

    size_t size() const
    {
        return attr.second;
    }

  private:
    span_source_t source;
    span_attr_t attr;
};

} // namespace ssharp::span
