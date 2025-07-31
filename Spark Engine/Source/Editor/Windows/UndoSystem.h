#pragma once
#include <vector>
#include <functional>
namespace SparkEngine {
class UndoSystem {
    std::vector<std::function<void()>> m_undo, m_redo;
public:
    void Execute(const std::function<void()>& d,const std::function<void()>& u){d(); m_undo.push_back(u); m_redo.clear();}
    void Undo(){ if(m_undo.size()){m_undo.back()(); m_undo.pop_back();} }
    void Redo(){ if(m_redo.size()){m_redo.back()(); m_redo.pop_back();} }
};
}
