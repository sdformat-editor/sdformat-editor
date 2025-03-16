#include "commands/ModifyAttributeCommand.h"

template class ModifyAttributeCommand<bool>;
template class ModifyAttributeCommand<std::string>;

template <typename T> ModifyAttributeCommand<T>::ModifyAttributeCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ParamPtr attribute_to_modify, T new_value) : gui(gui), sdformatParser(sdformatParser), attribute_to_modify(attribute_to_modify), new_value(new_value) {}

template <typename T> bool ModifyAttributeCommand<T>::Execute()
{
    if (!attribute_to_modify->Get(old_value)) return false;
    if (attribute_to_modify->Set(new_value))
    {
        this->is_currently_undoable = true;
        this->is_currently_redoable = false;

        return true;
    }
    return true;
}

template <typename T> bool ModifyAttributeCommand<T>::ExecuteUndo()
{
    if (attribute_to_modify->Set(old_value))
    {
        this->is_currently_undoable = false;
        this->is_currently_redoable = true;

        return true;
    }
    return false;
}

template <typename T> bool ModifyAttributeCommand<T>::ExecuteRedo()
{
    if (attribute_to_modify->Set(new_value))
    {
        this->is_currently_undoable = true;
        this->is_currently_redoable = false;
    
        return true;
    }
    return false;
}

template <typename T> bool ModifyAttributeCommand<T>::IsUndoable()
{
    return this->is_currently_undoable;
}

template <typename T> bool ModifyAttributeCommand<T>::IsRedoable()
{
    return this->is_currently_redoable;
}

template <typename T> bool ModifyAttributeCommand<T>::IsThreaded()
{
    // Stub implementation
    return false;
}

template <typename T> bool ModifyAttributeCommand<T>::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}