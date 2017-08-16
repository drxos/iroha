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

#include <algorithm>
#include "consensus/yac/storage/yac_common.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      bool hasSupermajority(uint64_t current, uint64_t all) {
        if (current > all)
          return false;
        auto f = (all - 1) / 3.0;
        return current >= 2 * f + 1;
      }

      bool sameProposals(const std::vector<VoteMessage> &votes) {
        if (votes.empty()) {
          return false;
        }

        auto first = votes.at(0);
        return std::all_of(votes.begin(), votes.end(),
                           [&first](auto current) {
                             return first.hash == current.hash;
                           });
      }

      nonstd::optional<ProposalHash>
      getProposalHash(const std::vector<VoteMessage> &votes) {
        if (not sameProposals(votes)) {
          return nonstd::nullopt;
        }

        return votes.at(0).hash.proposal_hash;
      }
    } // namespace yac
  } // namespace consensus
} // namespace iroha