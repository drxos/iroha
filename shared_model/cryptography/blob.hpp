/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_SHARED_MODEL_BLOB_HPP
#define IROHA_SHARED_MODEL_BLOB_HPP

#include "interfaces/model_primitive.hpp"
#include "utils/lazy_initializer.hpp"
#include "utils/string_builder.hpp"
#include <sstream>
#include <iomanip>

namespace shared_model {
  namespace crypto {

    static const std::string code = "0123456789abcdef";

    /**
     * Blob interface present user-friendly blob for working with low-level
     * binary stuff. Its length is not fixed in compile time.
     */
    class Blob : public interface::ModelPrimitive<Blob> {
     private:
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      Blob(std::string data)
          : data_(data), hex_([this] {
              std::ostringstream ret;

              for (std::string::size_type i = 0; i < data_.size(); ++i) {
                ret << std::hex << std::setfill('0') << std::setw(2)
                    << std::nouppercase
                    << (int)data_[i];
              }
              return ret.str();
            }){};
      /**
       * @return provides raw representation of blob
       */
      virtual const std::string &blob() const { return data_; }

      /**
       * @return provides human-readable representation of blob
       */
      virtual const std::string &hex() const { return hex_.get(); }

      /**
       * @return size of raw representation of blob
       */
      virtual size_t size() const { return data_.size(); };

      std::string toString() const override {
        return detail::PrettyStringBuilder()
            .init("Blob")
            .append(hex())
            .finalize();
      }

      bool operator==(const Blob &rhs) const override {
        return blob() == rhs.blob();
      }

      /**
       * Method perform transforming object to old-fashion blob_t format
       * @tparam BlobType - type of blob
       * @return blob_t array with own data
       * Design note: this method is deprecated and should be removed after
       * migration to shared model in whole project
       */
      template <typename BlobType>
      [[deprecated]] BlobType makeOldModel() const {
        return BlobType::from_string(blob());
      }

      Blob *copy() const override { return new Blob(data_); }

     protected:
      std::string data_;
      Lazy<std::string> hex_;
    };
  }  // namespace crypto
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_BLOB_HPP
