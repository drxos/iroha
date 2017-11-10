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

#include "interactive/interactive_cli.hpp"

namespace iroha_cli {
  namespace interactive {

    void InteractiveCli::assign_main_handlers() {
      addCliCommand(menu_points_,
                    main_handler_map_,
                    TX_CODE,
                    "New transaction",
                    &InteractiveCli::startTx);
      addCliCommand(menu_points_,
                    main_handler_map_,
                    QRY_CODE,
                    "New query",
                    &InteractiveCli::startQuery);
      addCliCommand(menu_points_,
                    main_handler_map_,
                    MULTI_CODE,
                    "Check uncompleted multisig transaction",
                    &InteractiveCli::startMultiSig);
    }

    InteractiveCli::InteractiveCli(
        const std::string &account_name,
        uint64_t tx_counter,
        uint64_t qry_counter,
        const CliClient &client,
        const std::shared_ptr<iroha::model::ModelCryptoProvider> &provider)
        : creator_(account_name),
          tx_cli_(creator_, tx_counter, provider),
          query_cli_(creator_, qry_counter, provider),
          multiSigCli_(
              creator_, qry_counter, client, provider) {
      assign_main_handlers();
    }

    void InteractiveCli::parseMain(std::string line) {
      auto raw_command = parser::parseFirstCommand(std::move(line));
      if (not raw_command.has_value()) {
        handleEmptyCommand();
        return;
      }
      auto command_name = raw_command.value();

      auto val = findInHandlerMap(command_name, main_handler_map_);
      if (val.has_value()) {
        (this->*val.value())();
      }
    }

    void InteractiveCli::startQuery() { query_cli_.run(); }

    void InteractiveCli::startTx() { tx_cli_.run(); }

    void InteractiveCli::startMultiSig() { multiSigCli_.run(); }

    void InteractiveCli::run() {
      std::cout << "Welcome to Iroha-Cli. " << std::endl;
      // Parsing cycle
      while (true) {
        printMenu("Choose what to do:", menu_points_);
        auto line = promtString("> ");
        parseMain(line);
      }
    }

  }  // namespace interactive
}  // namespace iroha_cli
