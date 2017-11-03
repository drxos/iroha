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

#ifndef IROHA_SHARED_MODEL_ADD_PEER_HPP
#define IROHA_SHARED_MODEL_ADD_PEER_HPP

#include "interfaces/common_objects/types.hpp"
#include "interfaces/hashable.hpp"
#include "model/commands/add_peer.hpp"

namespace shared_model {
  namespace interface {

    /**
     * Add new peer to Iroha
     */
    class AddPeer : public Hashable<AddPeer, iroha::model::AddPeer> {
     public:
      /**
       * @return Peer key, acts like peer identifier
       */
      virtual const types::PubkeyType &peerKey() const = 0;

      /// Type of peer address
      using AddressType = std::string;
      /**
       * @return New peer's address
       */
      virtual const AddressType &peerAddress() const = 0;

      std::string toString() const override {
        return detail::PrettyStringBuilder()
            .init("AddPeer")
            .append("peer_key", peerKey().toString())
            .append("peer_address", peerAddress())
            .finalize();
      }

      OldModelType *makeOldModel() const override {
        auto oldModel = new iroha::model::AddPeer;
        oldModel->address = peerAddress();
        // TODO: check if this is right
        oldModel->peer_key.from_string(peerKey().blob());
        return oldModel;
      }
    };
  }  // namespace interface
}  // namespace shared_model

#endif  // IROHA_SHARED_MODEL_ADD_PEER_HPP