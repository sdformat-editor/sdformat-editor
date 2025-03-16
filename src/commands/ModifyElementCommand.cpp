#include "commands/ModifyElementCommand.h"

template class ModifyElementCommand<std::string>;
template class ModifyElementCommand<bool>;

template <typename T> ModifyElementCommand<T>::ModifyElementCommand(std::shared_ptr<GUII> gui, std::shared_ptr<SDFormatParserI> sdformatParser, sdf::ElementPtr element_to_modify, T new_value) : gui(gui), sdformatParser(sdformatParser), element_to_modify(element_to_modify), new_value(new_value) {}

template <typename T> bool ModifyElementCommand<T>::Execute()
{
    if (!element_to_modify->GetValue()->Get(old_value)) return false;
    if (element_to_modify->Set(new_value))
    {
        this->is_currently_undoable = true;
        this->is_currently_redoable = false;

        return true;
    }
    
    return false;
}

template <typename T> bool ModifyElementCommand<T>::ExecuteUndo()
{
    if (element_to_modify->Set(old_value))
    {
        this->is_currently_undoable = false;
        this->is_currently_redoable = true;
        
        return true;
    } 
    return false;
}

template <typename T> bool ModifyElementCommand<T>::ExecuteRedo()
{
    if (element_to_modify->Set(new_value))
    {
        this->is_currently_undoable = true;
        this->is_currently_redoable = false;
    
        return true;
    }
    return false;
}

template <typename T> bool ModifyElementCommand<T>::IsUndoable()
{
    return this->is_currently_undoable;
}

template <typename T> bool ModifyElementCommand<T>::IsRedoable()
{
    return this->is_currently_redoable;
}

template <typename T> bool ModifyElementCommand<T>::IsThreaded()
{
    // Stub implementation
    return false;
}

template <typename T> bool ModifyElementCommand<T>::ChangesProgramStateIrreversibly()
{
    // Stub implementation
    return false;
}