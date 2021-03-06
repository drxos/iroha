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

#include <iostream>
#include <model/tx_responses/stateless_response.hpp>
#include <torii/processor/transaction_processor_impl.hpp>
#include <utility>

namespace iroha {
  namespace torii {

    using validation::StatelessValidator;
    using model::TransactionResponse;
    using network::PeerCommunicationService;

    TransactionProcessorImpl::TransactionProcessorImpl(
        std::shared_ptr<PeerCommunicationService> pcs,
        std::shared_ptr<StatelessValidator> validator)
        : pcs_(std::move(pcs)), validator_(std::move(validator)) {
      log_ = logger::log("TxProcessor");
    }

    void TransactionProcessorImpl::transactionHandle(
        std::shared_ptr<model::Transaction> transaction) {
      log_->info("handle transaction");
      model::TransactionStatelessResponse response;
      response.transaction = *transaction;
      response.passed = false;

      if (validator_->validate(*transaction)) {
        response.passed = true;
        pcs_->propagate_transaction(transaction);
      }
      log_->info("stateless validation status: {}", response.passed);
      notifier_.get_subscriber().on_next(
          std::make_shared<model::TransactionStatelessResponse>(response));
    }

    rxcpp::observable<std::shared_ptr<model::TransactionResponse>>
    TransactionProcessorImpl::transactionNotifier() {
      return notifier_.get_observable();
    }

  }  // namespace torii
}  // namespace iroha
