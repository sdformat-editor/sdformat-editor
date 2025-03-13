/*
* sdformat-editor
* Copyright 2025 sdformat-editor
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Developer: Zaid Duraid, Ean Wheeler, Evan Vokey
*/

#ifndef COMMAND_FACTORY_HH_
#define COMMAND_FACTORY_HH_

// SDFormat dependencies
#include <sdf/sdf.hh>
#include "interfaces/command_interface.h"

#include "commands/DeleteElementCommand.h"
#include "commands/OpenFileCommand.h"
#include "commands/SaveFileCommand.h"
#include "interfaces/command_factory_interface.h"

/// \brief Implementation of CommandFactoryI
class CommandFactory : public CommandFactoryI
{
    public: CommandFactory(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser);

    public: std::unique_ptr<CommandI> MakeOpenFileCommand();
    public: std::unique_ptr<CommandI> MakeDeleteElementCommand(sdf::ElementPtr element_to_delete);
    public: std::unique_ptr<CommandI> MakeSaveFileCommand(bool force_save_as_not_save);


    private: std::shared_ptr<GUII> gui;
    private: std::shared_ptr<SDFormatParserI> sdformatParser;
};

#endif