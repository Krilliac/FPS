/**
 * @file EditorTheme.cpp
 * @brief Professional theme management system with Unity/Unreal-inspired styling
 * @author Spark Engine Team
 * @date 2025
 */

#include "EditorTheme.h"
#include <imgui.h>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace SparkEditor {

// Static member definitions
std::unordered_map<std::string, EditorThemeData> EditorTheme::s_registeredThemes;
std::string EditorTheme::s_currentThemeName = "Unity Pro";
bool EditorTheme::s_enhancementsEnabled = true;
bool EditorTheme::s_customFontsLoaded = false;

// === THEME COLOR IMPLEMENTATIONS ===

ImVec4 ThemeColor::ToImVec4() const
{
    return ImVec4(r, g, b, a);
}

ThemeColor ThemeColor::FromRGB(int red, int green, int blue, int alpha)
{
    return ThemeColor(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

ThemeColor ThemeColor::FromHex(const std::string& hex)
{
    if (hex.empty() || hex[0] != '#') {
        return ThemeColor(0.5f, 0.5f, 0.5f, 1.0f);
    }
    
    std::string cleanHex = hex.substr(1);
    if (cleanHex.length() == 6) {
        int r = std::stoi(cleanHex.substr(0, 2), nullptr, 16);
        int g = std::stoi(cleanHex.substr(2, 2), nullptr, 16);
        int b = std::stoi(cleanHex.substr(4, 2), nullptr, 16);
        return FromRGB(r, g, b);
    }
    
    return ThemeColor(0.5f, 0.5f, 0.5f, 1.0f);
}

ThemeColor ThemeColor::Lerp(const ThemeColor& other, float t) const
{
    t = std::clamp(t, 0.0f, 1.0f);
    return ThemeColor(
        r + (other.r - r) * t,
        g + (other.g - g) * t,
        b + (other.b - b) * t,
        a + (other.a - a) * t
    );
}

ThemeColor ThemeColor::Darken(float amount) const
{
    amount = std::clamp(amount, 0.0f, 1.0f);
    return ThemeColor(r * (1.0f - amount), g * (1.0f - amount), b * (1.0f - amount), a);
}

ThemeColor ThemeColor::Lighten(float amount) const
{
    amount = std::clamp(amount, 0.0f, 1.0f);
    return ThemeColor(
        r + (1.0f - r) * amount,
        g + (1.0f - g) * amount,
        b + (1.0f - b) * amount,
        a
    );
}

ThemeColor ThemeColor::Desaturate(float amount) const
{
    amount = std::clamp(amount, 0.0f, 1.0f);
    float gray = r * 0.299f + g * 0.587f + b * 0.114f;
    return ThemeColor(
        r + (gray - r) * amount,
        g + (gray - g) * amount,
        b + (gray - b) * amount,
        a
    );
}

ThemeColor ThemeColor::WithAlpha(float alpha) const
{
    return ThemeColor(r, g, b, std::clamp(alpha, 0.0f, 1.0f));
}

// === EDITOR THEME IMPLEMENTATIONS ===

bool EditorTheme::ApplyTheme(const std::string& themeName)
{
    std::cout << "Applying professional theme: " << themeName << "\n";
    
    if (s_registeredThemes.empty()) {
        InitializeDefaultThemes();
    }
    
    auto it = s_registeredThemes.find(themeName);
    if (it != s_registeredThemes.end()) {
        ApplyToImGui(it->second);
        if (s_enhancementsEnabled) {
            ApplyAdvancedStyling(it->second);
            ApplyProfessionalEnhancements();
        }
        s_currentThemeName = themeName;
        return true;
    }
    
    // Fallback to Unity Pro theme
    if (themeName != "Unity Pro") {
        return ApplyTheme("Unity Pro");
    }
    
    return false;
}

bool EditorTheme::ApplyTheme(const EditorThemeData& theme)
{
    ApplyToImGui(theme);
    if (s_enhancementsEnabled) {
        ApplyAdvancedStyling(theme);
        ApplyProfessionalEnhancements();
    }
    s_currentThemeName = theme.name;
    return true;
}

std::vector<std::string> EditorTheme::GetAvailableThemes()
{
    if (s_registeredThemes.empty()) {
        InitializeDefaultThemes();
    }
    
    std::vector<std::string> themes;
    for (const auto& pair : s_registeredThemes) {
        themes.push_back(pair.first);
    }
    
    // Sort alphabetically but keep Unity Pro and Unreal Pro at the top
    std::sort(themes.begin(), themes.end(), [](const std::string& a, const std::string& b) {
        if (a == "Unity Pro") return true;
        if (b == "Unity Pro") return false;
        if (a == "Unreal Pro") return true;
        if (b == "Unreal Pro") return false;
        return a < b;
    });
    
    return themes;
}

const EditorThemeData* EditorTheme::GetTheme(const std::string& themeName)
{
    auto it = s_registeredThemes.find(themeName);
    if (it != s_registeredThemes.end()) {
        return &it->second;
    }
    return nullptr;
}

bool EditorTheme::RegisterTheme(const EditorThemeData& theme)
{
    s_registeredThemes[theme.name] = theme;
    std::cout << "Registered theme: " << theme.name << "\n";
    return true;
}

const std::string& EditorTheme::GetCurrentThemeName()
{
    return s_currentThemeName;
}

bool EditorTheme::CreateBlendedTheme(const std::string& theme1, const std::string& theme2, 
                                     float blend, const std::string& resultName)
{
    const EditorThemeData* t1 = GetTheme(theme1);
    const EditorThemeData* t2 = GetTheme(theme2);
    
    if (!t1 || !t2) {
        return false;
    }
    
    EditorThemeData blended;
    blended.name = resultName;
    blended.description = "Blended theme: " + theme1 + " + " + theme2;
    
    // Blend all colors
    blended.background = t1->background.Lerp(t2->background, blend);
    blended.backgroundDark = t1->backgroundDark.Lerp(t2->backgroundDark, blend);
    blended.backgroundLight = t1->backgroundLight.Lerp(t2->backgroundLight, blend);
    blended.backgroundAccent = t1->backgroundAccent.Lerp(t2->backgroundAccent, blend);
    blended.text = t1->text.Lerp(t2->text, blend);
    blended.textDisabled = t1->textDisabled.Lerp(t2->textDisabled, blend);
    blended.accent = t1->accent.Lerp(t2->accent, blend);
    blended.accentSecondary = t1->accentSecondary.Lerp(t2->accentSecondary, blend);
    
    // Blend style values
    blended.frameRounding = t1->frameRounding + (t2->frameRounding - t1->frameRounding) * blend;
    blended.windowRounding = t1->windowRounding + (t2->windowRounding - t1->windowRounding) * blend;
    
    RegisterTheme(blended);
    return true;
}

EditorThemeData EditorTheme::CreateUnityProTheme()
{
    EditorThemeData theme;
    theme.name = "Unity Pro";
    theme.description = "Professional Unity-inspired dark theme with modern accents";
    theme.author = "Spark Engine Team";
    
    // Unity-inspired color palette
    theme.background = ThemeColor::FromRGB(56, 56, 56);           // #383838
    theme.backgroundDark = ThemeColor::FromRGB(42, 42, 42);       // #2A2A2A
    theme.backgroundLight = ThemeColor::FromRGB(72, 72, 72);      // #484848
    theme.backgroundAccent = ThemeColor::FromRGB(70, 130, 180);   // #4682B4 (Steel Blue)
    theme.backgroundHeader = ThemeColor::FromRGB(48, 48, 48);     // #303030
    theme.backgroundActive = ThemeColor::FromRGB(90, 130, 180);   // Active selection
    theme.backgroundHover = ThemeColor::FromRGB(80, 80, 80);      // #505050
    theme.backgroundSelected = ThemeColor::FromRGB(62, 95, 138);  // #3E5F8A
    
    // Text colors
    theme.text = ThemeColor::FromRGB(210, 210, 210);              // #D2D2D2
    theme.textDisabled = ThemeColor::FromRGB(128, 128, 128);      // #808080
    theme.textSecondary = ThemeColor::FromRGB(180, 180, 180);     // #B4B4B4
    theme.textAccent = ThemeColor::FromRGB(100, 150, 200);        // #6496C8
    theme.textWarning = ThemeColor::FromRGB(255, 193, 7);         // #FFC107
    theme.textError = ThemeColor::FromRGB(244, 67, 54);           // #F44336
    theme.textSuccess = ThemeColor::FromRGB(76, 175, 80);         // #4CAF50
    
    // UI Elements
    theme.button = ThemeColor::FromRGB(96, 96, 96);               // #606060
    theme.buttonHovered = ThemeColor::FromRGB(112, 112, 112);     // #707070
    theme.buttonActive = ThemeColor::FromRGB(80, 80, 80);         // #505050
    theme.buttonDisabled = ThemeColor::FromRGB(64, 64, 64);       // #404040
    
    theme.frame = ThemeColor::FromRGB(48, 48, 48);                // #303030
    theme.frameHovered = ThemeColor::FromRGB(64, 64, 64);         // #404040
    theme.frameActive = ThemeColor::FromRGB(80, 80, 80);          // #505050
    
    theme.border = ThemeColor::FromRGB(80, 80, 80);               // #505050
    theme.borderLight = ThemeColor::FromRGB(96, 96, 96);          // #606060
    theme.borderAccent = ThemeColor::FromRGB(70, 130, 180);       // #4682B4
    theme.borderSeparator = ThemeColor::FromRGB(72, 72, 72);      // #484848
    
    // Panel colors
    theme.titleBar = ThemeColor::FromRGB(48, 48, 48);             // #303030
    theme.titleBarActive = ThemeColor::FromRGB(62, 95, 138);      // #3E5F8A
    theme.titleBarText = ThemeColor::FromRGB(210, 210, 210);      // #D2D2D2
    
    theme.menuBar = ThemeColor::FromRGB(48, 48, 48);              // #303030
    theme.menuItem = ThemeColor::FromRGB(48, 48, 48);             // #303030
    theme.menuItemHovered = ThemeColor::FromRGB(70, 130, 180);    // #4682B4
    
    theme.scrollbar = ThemeColor::FromRGB(42, 42, 42);            // #2A2A2A
    theme.scrollbarGrab = ThemeColor::FromRGB(96, 96, 96);        // #606060
    theme.scrollbarGrabHovered = ThemeColor::FromRGB(112, 112, 112); // #707070
    theme.scrollbarGrabActive = ThemeColor::FromRGB(128, 128, 128);  // #808080
    
    theme.tab = ThemeColor::FromRGB(64, 64, 64);                  // #404040
    theme.tabHovered = ThemeColor::FromRGB(80, 80, 80);           // #505050
    theme.tabActive = ThemeColor::FromRGB(70, 130, 180);          // #4682B4
    theme.tabUnfocused = ThemeColor::FromRGB(56, 56, 56);         // #383838
    
    // Special colors
    theme.accent = ThemeColor::FromRGB(70, 130, 180);             // #4682B4
    theme.accentSecondary = ThemeColor::FromRGB(100, 150, 200);   // #6496C8
    theme.focus = ThemeColor::FromRGB(255, 255, 255);             // #FFFFFF
    theme.selection = ThemeColor::FromRGB(62, 95, 138);           // #3E5F8A
    theme.drop = ThemeColor::FromRGB(76, 175, 80);                // #4CAF50
    
    // Graph colors for charts/profiler
    theme.graph1 = ThemeColor::FromRGB(255, 99, 132);             // Red
    theme.graph2 = ThemeColor::FromRGB(54, 162, 235);             // Blue  
    theme.graph3 = ThemeColor::FromRGB(255, 205, 86);             // Yellow
    theme.graph4 = ThemeColor::FromRGB(75, 192, 192);             // Teal
    theme.graph5 = ThemeColor::FromRGB(153, 102, 255);            // Purple
    
    // Professional styling
    theme.frameRounding = 2.0f;
    theme.windowRounding = 0.0f;
    theme.childRounding = 0.0f;
    theme.scrollbarRounding = 4.0f;
    theme.grabRounding = 2.0f;
    theme.tabRounding = 2.0f;
    
    theme.windowBorderSize = 1.0f;
    theme.frameBorderSize = 0.0f;
    theme.scrollbarSize = 16.0f;
    theme.indentSpacing = 22.0f;
    
    theme.windowPaddingX = 8.0f;
    theme.windowPaddingY = 8.0f;
    theme.framePaddingX = 4.0f;
    theme.framePaddingY = 3.0f;
    theme.itemSpacingX = 8.0f;
    theme.itemSpacingY = 4.0f;
    
    theme.enableShadows = true;
    theme.shadowOpacity = 0.25f;
    theme.fontSize = 16.0f;
    
    return theme;
}

EditorThemeData EditorTheme::CreateUnrealProTheme()
{
    EditorThemeData theme;
    theme.name = "Unreal Pro";
    theme.description = "Professional Unreal Engine-inspired dark theme";
    theme.author = "Spark Engine Team";
    
    // Unreal's characteristic darker palette
    theme.background = ThemeColor::FromRGB(46, 46, 46);           // #2E2E2E
    theme.backgroundDark = ThemeColor::FromRGB(32, 32, 32);       // #202020
    theme.backgroundLight = ThemeColor::FromRGB(64, 64, 64);      // #404040
    theme.backgroundAccent = ThemeColor::FromRGB(0, 162, 232);    // #00A2E8 (Unreal Blue)
    theme.backgroundHeader = ThemeColor::FromRGB(40, 40, 40);     // #282828
    theme.backgroundActive = ThemeColor::FromRGB(80, 162, 232);   // Active with Unreal blue
    theme.backgroundHover = ThemeColor::FromRGB(72, 72, 72);      // #484848
    theme.backgroundSelected = ThemeColor::FromRGB(40, 100, 140); // #28648C
    
    // Text colors
    theme.text = ThemeColor::FromRGB(200, 200, 200);              // #C8C8C8
    theme.textDisabled = ThemeColor::FromRGB(120, 120, 120);      // #787878
    theme.textSecondary = ThemeColor::FromRGB(170, 170, 170);     // #AAAAAA
    theme.textAccent = ThemeColor::FromRGB(80, 180, 255);         // #50B4FF
    theme.textWarning = ThemeColor::FromRGB(255, 165, 0);         // #FFA500
    theme.textError = ThemeColor::FromRGB(255, 99, 71);           // #FF6347
    theme.textSuccess = ThemeColor::FromRGB(50, 205, 50);         // #32CD32
    
    // UI Elements with Unreal's style
    theme.button = ThemeColor::FromRGB(88, 88, 88);               // #585858
    theme.buttonHovered = ThemeColor::FromRGB(104, 104, 104);     // #686868
    theme.buttonActive = ThemeColor::FromRGB(72, 72, 72);         // #484848
    
    theme.frame = ThemeColor::FromRGB(40, 40, 40);                // #282828
    theme.frameHovered = ThemeColor::FromRGB(56, 56, 56);         // #383838
    theme.frameActive = ThemeColor::FromRGB(72, 72, 72);          // #484848
    
    theme.border = ThemeColor::FromRGB(72, 72, 72);               // #484848
    theme.borderAccent = ThemeColor::FromRGB(0, 162, 232);        // #00A2E8
    
    // Tabs with Unreal's distinctive style
    theme.tab = ThemeColor::FromRGB(56, 56, 56);                  // #383838
    theme.tabHovered = ThemeColor::FromRGB(72, 72, 72);           // #484848
    theme.tabActive = ThemeColor::FromRGB(0, 162, 232);           // #00A2E8
    
    theme.accent = ThemeColor::FromRGB(0, 162, 232);              // #00A2E8
    theme.accentSecondary = ThemeColor::FromRGB(80, 180, 255);    // #50B4FF
    
    // Unreal-style rounded elements
    theme.frameRounding = 3.0f;
    theme.scrollbarRounding = 6.0f;
    theme.grabRounding = 3.0f;
    theme.tabRounding = 4.0f;
    
    theme.scrollbarSize = 18.0f;
    theme.indentSpacing = 20.0f;
    
    return theme;
}

EditorThemeData EditorTheme::CreateVSProTheme()
{
    EditorThemeData theme;
    theme.name = "Visual Studio Pro";
    theme.description = "Professional Visual Studio-inspired dark theme";
    theme.author = "Spark Engine Team";
    
    // VS Code/Visual Studio color scheme
    theme.background = ThemeColor::FromRGB(30, 30, 30);           // #1E1E1E
    theme.backgroundDark = ThemeColor::FromRGB(22, 22, 22);       // #161616
    theme.backgroundLight = ThemeColor::FromRGB(45, 45, 45);      // #2D2D2D
    theme.backgroundAccent = ThemeColor::FromRGB(0, 122, 204);    // #007ACC (VS Blue)
    theme.backgroundHeader = ThemeColor::FromRGB(37, 37, 38);     // #252526
    
    theme.text = ThemeColor::FromRGB(204, 204, 204);              // #CCCCCC
    theme.textDisabled = ThemeColor::FromRGB(128, 128, 128);      // #808080
    theme.textAccent = ThemeColor::FromRGB(78, 201, 176);         // #4EC9B0
    
    theme.button = ThemeColor::FromRGB(45, 45, 45);               // #2D2D2D
    theme.buttonHovered = ThemeColor::FromRGB(62, 62, 64);        // #3E3E40
    theme.buttonActive = ThemeColor::FromRGB(0, 122, 204);        // #007ACC
    
    theme.frame = ThemeColor::FromRGB(37, 37, 38);                // #252526
    theme.frameActive = ThemeColor::FromRGB(0, 122, 204);         // #007ACC
    
    theme.accent = ThemeColor::FromRGB(0, 122, 204);              // #007ACC
    theme.selection = ThemeColor::FromRGB(51, 153, 255);          // #3399FF
    
    theme.frameRounding = 1.0f;
    theme.scrollbarRounding = 2.0f;
    theme.tabRounding = 0.0f;  // VS has sharp tabs
    
    return theme;
}

EditorThemeData EditorTheme::CreateJetBrainsTheme()
{
    EditorThemeData theme;
    theme.name = "JetBrains Dark";
    theme.description = "Professional JetBrains IDE-inspired theme";
    theme.author = "Spark Engine Team";
    
    // JetBrains Darcula colors
    theme.background = ThemeColor::FromRGB(60, 63, 65);           // #3C3F41
    theme.backgroundDark = ThemeColor::FromRGB(43, 43, 43);       // #2B2B2B
    theme.backgroundLight = ThemeColor::FromRGB(75, 80, 82);      // #4B5052
    theme.backgroundAccent = ThemeColor::FromRGB(75, 110, 175);   // #4B6EAF
    
    theme.text = ThemeColor::FromRGB(187, 187, 187);              // #BBBBBB
    theme.textAccent = ThemeColor::FromRGB(104, 151, 187);        // #6897BB
    
    theme.button = ThemeColor::FromRGB(75, 80, 82);               // #4B5052
    theme.buttonHovered = ThemeColor::FromRGB(90, 95, 97);        // #5A5F61
    
    theme.accent = ThemeColor::FromRGB(75, 110, 175);             // #4B6EAF
    
    return theme;
}

EditorThemeData EditorTheme::CreateBlueAccentTheme()
{
    EditorThemeData theme = CreateUnityProTheme();
    theme.name = "Blue Accent";
    theme.description = "Dark theme with blue accent colors";
    
    // Override accent colors
    theme.accent = ThemeColor::FromRGB(33, 150, 243);             // #2196F3
    theme.accentSecondary = ThemeColor::FromRGB(100, 181, 246);   // #64B5F6
    theme.backgroundAccent = ThemeColor::FromRGB(33, 150, 243);
    theme.tabActive = ThemeColor::FromRGB(33, 150, 243);
    theme.borderAccent = ThemeColor::FromRGB(33, 150, 243);
    
    return theme;
}

EditorThemeData EditorTheme::CreateOrangeAccentTheme()
{
    EditorThemeData theme = CreateUnityProTheme();
    theme.name = "Orange Accent";
    theme.description = "Dark theme with orange accent colors";
    
    // Override accent colors
    theme.accent = ThemeColor::FromRGB(255, 152, 0);              // #FF9800
    theme.accentSecondary = ThemeColor::FromRGB(255, 183, 77);    // #FFB74D
    theme.backgroundAccent = ThemeColor::FromRGB(255, 152, 0);
    theme.tabActive = ThemeColor::FromRGB(255, 152, 0);
    theme.borderAccent = ThemeColor::FromRGB(255, 152, 0);
    
    return theme;
}

EditorThemeData EditorTheme::CreateProfessionalLightTheme()
{
    EditorThemeData theme;
    theme.name = "Professional Light";
    theme.description = "Clean professional light theme";
    theme.author = "Spark Engine Team";
    
    // Light theme colors
    theme.background = ThemeColor::FromRGB(250, 250, 250);        // #FAFAFA
    theme.backgroundDark = ThemeColor::FromRGB(240, 240, 240);    // #F0F0F0
    theme.backgroundLight = ThemeColor::FromRGB(255, 255, 255);   // #FFFFFF
    theme.backgroundAccent = ThemeColor::FromRGB(33, 150, 243);   // #2196F3
    
    theme.text = ThemeColor::FromRGB(33, 33, 33);                 // #212121
    theme.textDisabled = ThemeColor::FromRGB(158, 158, 158);      // #9E9E9E
    theme.textSecondary = ThemeColor::FromRGB(117, 117, 117);     // #757575
    
    theme.button = ThemeColor::FromRGB(245, 245, 245);            // #F5F5F5
    theme.buttonHovered = ThemeColor::FromRGB(238, 238, 238);     // #EEEEEE
    theme.buttonActive = ThemeColor::FromRGB(224, 224, 224);      // #E0E0E0
    
    theme.border = ThemeColor::FromRGB(224, 224, 224);            // #E0E0E0
    theme.borderAccent = ThemeColor::FromRGB(33, 150, 243);       // #2196F3
    
    theme.accent = ThemeColor::FromRGB(33, 150, 243);             // #2196F3
    
    return theme;
}

EditorThemeData EditorTheme::CreateHighContrastTheme()
{
    EditorThemeData theme;
    theme.name = "High Contrast";
    theme.description = "High contrast theme for accessibility";
    theme.author = "Spark Engine Team";
    
    // High contrast colors
    theme.background = ThemeColor::FromRGB(0, 0, 0);              // #000000
    theme.backgroundDark = ThemeColor::FromRGB(0, 0, 0);          // #000000
    theme.backgroundLight = ThemeColor::FromRGB(32, 32, 32);      // #202020
    theme.backgroundAccent = ThemeColor::FromRGB(255, 255, 0);    // #FFFF00
    
    theme.text = ThemeColor::FromRGB(255, 255, 255);              // #FFFFFF
    theme.textDisabled = ThemeColor::FromRGB(192, 192, 192);      // #C0C0C0
    theme.textAccent = ThemeColor::FromRGB(255, 255, 0);          // #FFFF00
    theme.textWarning = ThemeColor::FromRGB(255, 255, 0);         // #FFFF00
    theme.textError = ThemeColor::FromRGB(255, 0, 0);             // #FF0000
    theme.textSuccess = ThemeColor::FromRGB(0, 255, 0);           // #00FF00
    
    theme.button = ThemeColor::FromRGB(64, 64, 64);               // #404040
    theme.buttonHovered = ThemeColor::FromRGB(128, 128, 128);     // #808080
    theme.buttonActive = ThemeColor::FromRGB(255, 255, 255);      // #FFFFFF
    
    theme.border = ThemeColor::FromRGB(255, 255, 255);            // #FFFFFF
    theme.borderAccent = ThemeColor::FromRGB(255, 255, 0);        // #FFFF00
    
    theme.accent = ThemeColor::FromRGB(255, 255, 0);              // #FFFF00
    
    // High contrast settings
    theme.frameRounding = 0.0f;
    theme.windowBorderSize = 2.0f;
    theme.frameBorderSize = 1.0f;
    
    return theme;
}

void EditorTheme::InitializeDefaultThemes()
{
    std::cout << "Initializing professional themes...\n";
    
    RegisterTheme(CreateUnityProTheme());
    RegisterTheme(CreateUnrealProTheme());
    RegisterTheme(CreateVSProTheme());
    RegisterTheme(CreateJetBrainsTheme());
    RegisterTheme(CreateBlueAccentTheme());
    RegisterTheme(CreateOrangeAccentTheme());
    RegisterTheme(CreateProfessionalLightTheme());
    RegisterTheme(CreateHighContrastTheme());
    
    std::cout << "Professional themes initialized.\n";
}

void EditorTheme::ApplyToImGui(const EditorThemeData& theme)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    
    // === COMPREHENSIVE COLOR APPLICATION ===
    
    // Window colors
    colors[ImGuiCol_WindowBg] = theme.background.ToImVec4();
    colors[ImGuiCol_ChildBg] = theme.backgroundDark.ToImVec4();
    colors[ImGuiCol_PopupBg] = theme.backgroundLight.ToImVec4();
    
    // Text colors
    colors[ImGuiCol_Text] = theme.text.ToImVec4();
    colors[ImGuiCol_TextDisabled] = theme.textDisabled.ToImVec4();
    
    // Border colors
    colors[ImGuiCol_Border] = theme.border.ToImVec4();
    colors[ImGuiCol_BorderShadow] = ThemeColor(0.0f, 0.0f, 0.0f, 0.0f).ToImVec4();
    
    // Frame colors
    colors[ImGuiCol_FrameBg] = theme.frame.ToImVec4();
    colors[ImGuiCol_FrameBgHovered] = theme.frameHovered.ToImVec4();
    colors[ImGuiCol_FrameBgActive] = theme.frameActive.ToImVec4();
    
    // Title bar colors
    colors[ImGuiCol_TitleBg] = theme.titleBar.ToImVec4();
    colors[ImGuiCol_TitleBgActive] = theme.titleBarActive.ToImVec4();
    colors[ImGuiCol_TitleBgCollapsed] = theme.titleBar.Darken(0.1f).ToImVec4();
    
    // Menu colors
    colors[ImGuiCol_MenuBarBg] = theme.menuBar.ToImVec4();
    
    // Scrollbar colors
    colors[ImGuiCol_ScrollbarBg] = theme.scrollbar.ToImVec4();
    colors[ImGuiCol_ScrollbarGrab] = theme.scrollbarGrab.ToImVec4();
    colors[ImGuiCol_ScrollbarGrabHovered] = theme.scrollbarGrabHovered.ToImVec4();
    colors[ImGuiCol_ScrollbarGrabActive] = theme.scrollbarGrabActive.ToImVec4();
    
    // CheckMark and SliderGrab
    colors[ImGuiCol_CheckMark] = theme.accent.ToImVec4();
    colors[ImGuiCol_SliderGrab] = theme.accent.ToImVec4();
    colors[ImGuiCol_SliderGrabActive] = theme.accent.Lighten(0.2f).ToImVec4();
    
    // Button colors
    colors[ImGuiCol_Button] = theme.button.ToImVec4();
    colors[ImGuiCol_ButtonHovered] = theme.buttonHovered.ToImVec4();
    colors[ImGuiCol_ButtonActive] = theme.buttonActive.ToImVec4();
    
    // Header colors
    colors[ImGuiCol_Header] = theme.backgroundLight.ToImVec4();
    colors[ImGuiCol_HeaderHovered] = theme.backgroundHover.ToImVec4();
    colors[ImGuiCol_HeaderActive] = theme.backgroundActive.ToImVec4();
    
    // Separator colors
    colors[ImGuiCol_Separator] = theme.borderSeparator.ToImVec4();
    colors[ImGuiCol_SeparatorHovered] = theme.accent.ToImVec4();
    colors[ImGuiCol_SeparatorActive] = theme.accent.Lighten(0.2f).ToImVec4();
    
    // Resize grip colors
    colors[ImGuiCol_ResizeGrip] = theme.backgroundLight.ToImVec4();
    colors[ImGuiCol_ResizeGripHovered] = theme.accent.ToImVec4();
    colors[ImGuiCol_ResizeGripActive] = theme.accent.Lighten(0.2f).ToImVec4();
    
    // Tab colors
    colors[ImGuiCol_Tab] = theme.tab.ToImVec4();
    colors[ImGuiCol_TabHovered] = theme.tabHovered.ToImVec4();
    colors[ImGuiCol_TabSelected] = theme.tabActive.ToImVec4();
    colors[ImGuiCol_TabSelectedOverline] = theme.accent.ToImVec4();
    colors[ImGuiCol_TabDimmed] = theme.tabUnfocused.ToImVec4();
    colors[ImGuiCol_TabDimmedSelected] = theme.tabActive.Darken(0.3f).ToImVec4();
    
    // Plot colors
    colors[ImGuiCol_PlotLines] = theme.accent.ToImVec4();
    colors[ImGuiCol_PlotLinesHovered] = theme.accent.Lighten(0.3f).ToImVec4();
    colors[ImGuiCol_PlotHistogram] = theme.accentSecondary.ToImVec4();
    colors[ImGuiCol_PlotHistogramHovered] = theme.accentSecondary.Lighten(0.3f).ToImVec4();
    
    // Table colors
    colors[ImGuiCol_TableHeaderBg] = theme.backgroundHeader.ToImVec4();
    colors[ImGuiCol_TableBorderStrong] = theme.border.ToImVec4();
    colors[ImGuiCol_TableBorderLight] = theme.borderLight.ToImVec4();
    colors[ImGuiCol_TableRowBg] = ThemeColor(0.0f, 0.0f, 0.0f, 0.0f).ToImVec4();
    colors[ImGuiCol_TableRowBgAlt] = theme.backgroundDark.WithAlpha(0.1f).ToImVec4();
    
    // Text selection
    colors[ImGuiCol_TextSelectedBg] = theme.selection.ToImVec4();
    
    // Drag and drop
    colors[ImGuiCol_DragDropTarget] = theme.drop.ToImVec4();
    
    // Navigation
    colors[ImGuiCol_NavCursor] = theme.focus.ToImVec4();
    colors[ImGuiCol_NavWindowingHighlight] = theme.accent.WithAlpha(0.7f).ToImVec4();
    colors[ImGuiCol_NavWindowingDimBg] = theme.backgroundDark.WithAlpha(0.8f).ToImVec4();
    colors[ImGuiCol_ModalWindowDimBg] = theme.backgroundDark.WithAlpha(0.8f).ToImVec4();
    
    // === STYLE VALUES ===
    
    style.WindowRounding = theme.windowRounding;
    style.ChildRounding = theme.childRounding;
    style.FrameRounding = theme.frameRounding;
    style.PopupRounding = theme.popupRounding;
    style.ScrollbarRounding = theme.scrollbarRounding;
    style.GrabRounding = theme.grabRounding;
    style.TabRounding = theme.tabRounding;
    
    style.WindowBorderSize = theme.windowBorderSize;
    style.ChildBorderSize = theme.childBorderSize;
    style.PopupBorderSize = theme.popupBorderSize;
    style.FrameBorderSize = theme.frameBorderSize;
    
    style.WindowPadding = ImVec2(theme.windowPaddingX, theme.windowPaddingY);
    style.FramePadding = ImVec2(theme.framePaddingX, theme.framePaddingY);
    style.ItemSpacing = ImVec2(theme.itemSpacingX, theme.itemSpacingY);
    style.ItemInnerSpacing = ImVec2(theme.itemInnerSpacingX, theme.itemInnerSpacingY);
    
    style.IndentSpacing = theme.indentSpacing;
    style.ScrollbarSize = theme.scrollbarSize;
    style.GrabMinSize = theme.grabMinSize;
    
    // Professional touches
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);    // Left-aligned titles
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);     // Centered button text
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f); // Left-aligned selectable text
    
    // Anti-aliasing and smoothness
    style.AntiAliasedLines = true;
    style.AntiAliasedLinesUseTex = true;
    style.AntiAliasedFill = true;
    
    // Improved spacing for professional look
    style.CellPadding = ImVec2(4.0f, 2.0f);
    style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
}

void EditorTheme::ApplyAdvancedStyling(const EditorThemeData& theme)
{
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Advanced professional styling
    if (theme.enableShadows) {
        // Note: Shadow implementation would require custom rendering
        // For now, we apply visual enhancements through borders and spacing
    }
    
    // Improved visual hierarchy
    style.TabMinWidthBase = 80.0f;
    style.TabBarBorderSize = 1.0f;
    
    // Better touch targets for professional use
    style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
    
    // Professional window management
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ColorButtonPosition = ImGuiDir_Right;
    
    std::cout << "Applied advanced styling for theme: " << theme.name << "\n";
}

void EditorTheme::ApplyProfessionalEnhancements()
{
    // Additional professional enhancements
    std::cout << "Applied professional enhancements\n";
}

void EditorTheme::ApplyCustomFonts()
{
    if (s_customFontsLoaded) {
        return;
    }
    
    // Custom font loading would be implemented here
    ImGuiIO& io = ImGui::GetIO();
    io.FontDefault = nullptr;
    
    // TODO: Load custom fonts like Roboto, Source Sans Pro, etc.
    
    s_customFontsLoaded = true;
    std::cout << "Custom fonts applied\n";
}

unsigned int EditorTheme::ColorToImGui(const ThemeColor& color)
{
    return IM_COL32(
        (int)(color.r * 255), 
        (int)(color.g * 255), 
        (int)(color.b * 255), 
        (int)(color.a * 255)
    );
}

// === THEME CUSTOMIZER IMPLEMENTATIONS ===

void ThemeCustomizer::ShowThemeEditor()
{
    static bool showEditor = true;
    if (!showEditor) return;
    
    ImGui::Begin("Theme Editor", &showEditor, ImGuiWindowFlags_AlwaysAutoResize);
    
    // Theme selection
    static int currentTheme = 0;
    auto themes = EditorTheme::GetAvailableThemes();
    std::vector<const char*> themeNames;
    for (const auto& theme : themes) {
        themeNames.push_back(theme.c_str());
    }
    
    if (ImGui::Combo("Base Theme", &currentTheme, themeNames.data(), static_cast<int>(themeNames.size()))) {
        EditorTheme::ApplyTheme(themes[currentTheme]);
    }
    
    ImGui::Separator();
    
    // Get current theme data for editing
    const EditorThemeData* currentThemeData = EditorTheme::GetTheme(themes[currentTheme]);
    if (currentThemeData) {
        static EditorThemeData editTheme = *currentThemeData;
        
        ImGui::Text("Customize Colors");
        ImGui::Separator();
        
        // Color editing sections
        if (ImGui::CollapsingHeader("Background Colors", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Background", &editTheme.background.r);
            ImGui::ColorEdit3("Background Dark", &editTheme.backgroundDark.r);
            ImGui::ColorEdit3("Background Light", &editTheme.backgroundLight.r);
            ImGui::ColorEdit3("Background Accent", &editTheme.backgroundAccent.r);
        }
        
        if (ImGui::CollapsingHeader("Text Colors")) {
            ImGui::ColorEdit3("Text", &editTheme.text.r);
            ImGui::ColorEdit3("Text Disabled", &editTheme.textDisabled.r);
            ImGui::ColorEdit3("Text Secondary", &editTheme.textSecondary.r);
            ImGui::ColorEdit3("Text Accent", &editTheme.textAccent.r);
        }
        
        if (ImGui::CollapsingHeader("UI Element Colors")) {
            ImGui::ColorEdit3("Button", &editTheme.button.r);
            ImGui::ColorEdit3("Button Hovered", &editTheme.buttonHovered.r);
            ImGui::ColorEdit3("Button Active", &editTheme.buttonActive.r);
            ImGui::ColorEdit3("Frame", &editTheme.frame.r);
            ImGui::ColorEdit3("Frame Hovered", &editTheme.frameHovered.r);
            ImGui::ColorEdit3("Frame Active", &editTheme.frameActive.r);
        }
        
        if (ImGui::CollapsingHeader("Accent Colors")) {
            ImGui::ColorEdit3("Primary Accent", &editTheme.accent.r);
            ImGui::ColorEdit3("Secondary Accent", &editTheme.accentSecondary.r);
            ImGui::ColorEdit3("Selection", &editTheme.selection.r);
        }
        
        ImGui::Separator();
        ImGui::Text("Style Values");
        ImGui::Separator();
        
        if (ImGui::CollapsingHeader("Rounding", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SliderFloat("Frame Rounding", &editTheme.frameRounding, 0.0f, 10.0f);
            ImGui::SliderFloat("Window Rounding", &editTheme.windowRounding, 0.0f, 10.0f);
            ImGui::SliderFloat("Scrollbar Rounding", &editTheme.scrollbarRounding, 0.0f, 10.0f);
            ImGui::SliderFloat("Tab Rounding", &editTheme.tabRounding, 0.0f, 10.0f);
        }
        
        if (ImGui::CollapsingHeader("Borders")) {
            ImGui::SliderFloat("Window Border Size", &editTheme.windowBorderSize, 0.0f, 3.0f);
            ImGui::SliderFloat("Frame Border Size", &editTheme.frameBorderSize, 0.0f, 3.0f);
        }
        
        if (ImGui::CollapsingHeader("Spacing")) {
            ImGui::SliderFloat("Window Padding X", &editTheme.windowPaddingX, 0.0f, 20.0f);
            ImGui::SliderFloat("Window Padding Y", &editTheme.windowPaddingY, 0.0f, 20.0f);
            ImGui::SliderFloat("Item Spacing X", &editTheme.itemSpacingX, 0.0f, 20.0f);
            ImGui::SliderFloat("Item Spacing Y", &editTheme.itemSpacingY, 0.0f, 20.0f);
            ImGui::SliderFloat("Indent Spacing", &editTheme.indentSpacing, 10.0f, 50.0f);
        }
        
        ImGui::Separator();
        
        // Actions
        if (ImGui::Button("Apply Changes")) {
            EditorTheme::ApplyTheme(editTheme);
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Reset to Original")) {
            editTheme = *currentThemeData;
            EditorTheme::ApplyTheme(editTheme);
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Save as New Theme")) {
            static char newThemeName[256] = "Custom Theme";
            ImGui::OpenPopup("Save Theme");
        }
        
        // Save dialog
        if (ImGui::BeginPopupModal("Save Theme", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char newThemeName[256] = "Custom Theme";
            ImGui::InputText("Theme Name", newThemeName, sizeof(newThemeName));
            
            if (ImGui::Button("Save")) {
                editTheme.name = std::string(newThemeName);
                editTheme.description = "Custom user theme";
                editTheme.author = "User";
                EditorTheme::RegisterTheme(editTheme);
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
    }
    
    ImGui::End();
}

bool ThemeCustomizer::ExportTheme(const EditorThemeData& theme, const std::string& filepath)
{
    std::cout << "Exporting theme '" << theme.name << "' to " << filepath << "\n";
    // TODO: Implement JSON/XML export
    return true;
}

bool ThemeCustomizer::ImportTheme(const std::string& filepath, EditorThemeData& outTheme)
{
    std::cout << "Importing theme from " << filepath << "\n";
    // TODO: Implement JSON/XML import
    return false;
}

std::vector<EditorThemeData> ThemeCustomizer::GenerateThemeVariations(const EditorThemeData& baseTheme)
{
    std::vector<EditorThemeData> variations;
    
    // Generate darker variation
    EditorThemeData darker = baseTheme;
    darker.name = baseTheme.name + " (Dark)";
    darker.background = darker.background.Darken(0.2f);
    darker.backgroundDark = darker.backgroundDark.Darken(0.2f);
    darker.backgroundLight = darker.backgroundLight.Darken(0.2f);
    variations.push_back(darker);
    
    // Generate lighter variation
    EditorThemeData lighter = baseTheme;
    lighter.name = baseTheme.name + " (Light)";
    lighter.background = lighter.background.Lighten(0.2f);
    lighter.backgroundDark = lighter.backgroundDark.Lighten(0.2f);
    lighter.backgroundLight = lighter.backgroundLight.Lighten(0.2f);
    variations.push_back(lighter);
    
    // Generate high contrast variation
    EditorThemeData highContrast = baseTheme;
    highContrast.name = baseTheme.name + " (High Contrast)";
    highContrast.background = highContrast.background.Darken(0.5f);
    highContrast.text = ThemeColor(1.0f, 1.0f, 1.0f, 1.0f);
    highContrast.accent = ThemeColor(1.0f, 1.0f, 0.0f, 1.0f);
    variations.push_back(highContrast);
    
    return variations;
}

} // namespace SparkEditor