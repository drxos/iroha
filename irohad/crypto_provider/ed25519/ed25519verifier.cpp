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

#include "ed25519verifier.hpp"

namespace iroha {
  namespace crypto {
    namespace ed25519 {

      bool Ed25519Verifier::verify(
          const message_t &m,
          const Ed25519Verifier::pubkey_t &p,
          const Ed25519Verifier::signature_t &s) const {
        return ed25519::verify(m, p, s);
      }
    }
  }
}