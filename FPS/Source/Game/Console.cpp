#include "Console.h"

// -----------------------------------------------------------------------------
void Console::Initialize(int screenW, int screenH)
{
    width = screenW;
    height = screenH;

    // Simple built-in “help” command
    commands.push_back({ L"help", [this](auto&)
    {
        Log(L"Available commands:");
        for (auto& c : commands)
            Log(L"  " + c.name);
    } });
}

// -----------------------------------------------------------------------------
bool Console::HandleChar(wchar_t c)
{
    if (!visible) return false;

    if (c >= L' ' && c <= L'~')      // printable ASCII
        inputLine.push_back(c);

    return true;                     // swallow input
}

// -----------------------------------------------------------------------------
bool Console::HandleKeyDown(WPARAM key)
{
    if (!visible) return false;

    switch (key)
    {
    case VK_BACK:
        if (!inputLine.empty()) inputLine.pop_back();
        return true;

    case VK_RETURN:
        ExecuteCommand(inputLine);
        inputLine.clear();
        return true;

    case VK_ESCAPE:
        Toggle();                    // close console
        return true;

    default:
        return false;
    }
}

// -----------------------------------------------------------------------------
void Console::Log(const std::wstring& msg)
{
    buffer.push_back(msg);
    if (buffer.size() > 100)         // cap log size
        buffer.erase(buffer.begin());
}

// -----------------------------------------------------------------------------
void Console::ExecuteCommand(const std::wstring& line)
{
    Log(L"> " + line);

    std::wistringstream iss(line);
    std::vector<std::wstring> tokens;
    std::wstring tok;
    while (iss >> tok) tokens.push_back(tok);
    if (tokens.empty()) return;

    for (auto& cmd : commands)
        if (cmd.name == tokens[0]) { cmd.callback(tokens); return; }

    Log(L"Unknown command: " + tokens[0]);
}

// -----------------------------------------------------------------------------
void Console::Render(ID3D11DeviceContext* ctx)
{
    if (!visible) return;

    constexpr float lineH = 18.0f;
    constexpr float scale = 1.0f;
    float y = 10.0f;

    int start = (std::max)(0, int(buffer.size()) - 20);
    for (int i = start; i < (int)buffer.size(); ++i)
    {
        DrawText(buffer[i], 10.0f, y, scale, ctx);
        y += lineH;
    }
    DrawText(L"> " + inputLine, 10.0f, y, scale, ctx);
}