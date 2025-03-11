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

#include "commands/DeleteElementCommand.h"

DeleteElementCommand::DeleteElementCommand(std::shared_ptr<GUII> gui, 
                                        std::shared_ptr<SDFormatParserI> sdformatParser,
                                        sdf::ElementPtr element_to_delete)
    : gui(gui), sdformatParser(sdformatParser)
{
    // Initialize any needed members
}

bool DeleteElementCommand::execute()
{
    // Stub implementation
    return true;
}

bool DeleteElementCommand::executeUndo()
{
    // Stub implementation
    return true;
}

bool DeleteElementCommand::executeRedo()
{
    // Stub implementation
    return true;
}

bool DeleteElementCommand::undo()
{
    // Stub implementation
    return false;
}

bool DeleteElementCommand::redo()
{
    // Stub implementation
    return false;
}

bool DeleteElementCommand::threaded()
{
    // Stub implementation
    return true;
}