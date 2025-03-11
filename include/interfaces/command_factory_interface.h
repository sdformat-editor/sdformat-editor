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

#ifndef COMMAND_FACTORY_INTERFACE_HH_
#define COMMAND_FACTORY_INTERFACE_HH_

// SDFormat dependencies
#include <sdf/sdf.hh>
#include <memory>
#include "interfaces/command_interface.h"

/// \brief Abstract interface for command factory
class CommandFactoryI
{
  public: virtual ~CommandFactoryI() = default;

  /// \brief Create an open file command
  /// \return Unique pointer to a command interface 
  public: virtual std::unique_ptr<CommandI> MakeOpenFileCommand() = 0;

  /// \brief Create a delete element command
  /// \param[in] element_to_delete The SDF element to delete
  /// \return Unique pointer to a command interface
  public: virtual std::unique_ptr<CommandI> MakeDeleteElementCommand(
      sdf::ElementPtr element_to_delete) = 0;
};

#endif // COMMAND_FACTORY_INTERFACE_HH_