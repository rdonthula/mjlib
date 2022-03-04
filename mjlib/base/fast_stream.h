// Copyright 2015-2020 Josh Pieper, jjp@pobox.com.
//
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

#include <cstring>
#include <string>
#include <vector>

#include "mjlib/base/stream.h"

namespace mjlib {
namespace base {

/// A very simple std::ostringstream replacement for binary data only.
class FastOStringStream : public WriteStream {
 public:
  void write(const std::string_view& data) override {
    std::size_t old_size = data_.size();
    data_.resize(data_.size() + data.size());
    std::memcpy(&data_[old_size], data.data(), data.size());
  }

  std::string str() const { return std::string(&data_[0], data_.size()); }
  std::string_view view() const { return std::string_view(&data_[0], data_.size()); }

  void clear() { data_.clear(); }

  class vector {
   public:
    void resize(std::size_t new_size) {
      size_ = new_size;
      if (new_size > base_.size()) { base_.resize(new_size); }
    }

    void reserve(std::size_t new_size) {
      if (new_size > base_.size()) { base_.resize(new_size); }
    }

    void clear() { size_ = 0; }

    char& back() { return base_[size_ - 1]; }
    const char& back() const { return base_[size_ - 1]; }
    char& operator[](size_t index) { return base_[index]; }
    const char& operator[](size_t index) const { return base_[index]; }

    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return base_.size(); }
    char* data() { return base_.data(); }
    const char* data() const { return base_.data(); }

    private:
    std::size_t size_ = 0;
    std::vector<char> base_;
  };

  vector* data() { return &data_; }
  const vector* data() const { return &data_; }

 private:
  vector data_;
};

class FastIStringStream : public ReadStream {
 public:
  FastIStringStream(const std::string& data) : data_(data) {}

  void ignore(std::streamsize length) override {
    last_read_ = std::min<std::streamsize>(data_.size() - offset_, length);
    offset_ += last_read_;
  }

  void read(const string_span& buffer) override {
    last_read_ = std::min<std::streamsize>(data_.size() - offset_, buffer.size());
    std::memcpy(buffer.data(), &data_[offset_], last_read_);
    offset_ += last_read_;
  }

  std::streamsize gcount() const override { return last_read_; }

  size_t offset() const { return offset_; }
  size_t remaining() const { return data_.size() - offset_; }

  const std::string data_;
  size_t offset_ = 0;
  std::streamsize last_read_ = 0;
};
}
}
