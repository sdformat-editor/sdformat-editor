#include "command_factory.h"

CommandFactory::CommandFactory(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser)
{
    // TODO: Constructor Implementation
    this->gui = gui;
    this->sdformatParser = sdformatParser;
}

std::unique_ptr<CommandI> CommandFactory::MakeOpenFileCommand()
{
    return std::make_unique<OpenFileCommand>(this->gui, this->sdformatParser);
}

std::unique_ptr<CommandI> CommandFactory::MakeDeleteElementCommand(sdf::ElementPtr element_to_delete)
{
    return std::make_unique<DeleteElementCommand>(this->gui, this->sdformatParser, element_to_delete);
}