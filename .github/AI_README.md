# SparkEngine AI Prompt Library

This directory contains a comprehensive AI prompt library designed specifically 
for SparkEngine development. The library provides standardized prompts for 
various AI assistants including GitHub Copilot, GPT-4/5, Claude, Perplexity, 
and Gemini.

## Quick Start

1. **GitHub Copilot**: Automatically loads `.github/copilot-instructions.md`
   - Use `#prompt:<name>` to load specific prompt files
   - Example: `#prompt:02_editor_ui_tasks`

2. **Other AI Assistants**: Use the assistant-specific prompt files
   - `OPENAI_PROMPTS.md` for GPT-4/5
   - `CLAUDE_PROMPTS.md` for Claude
   - `PERPLEXITY_PROMPTS.md` for research queries
   - `GEMINI_PROMPTS.md` for Gemini

## File Structure

### Core Configuration
- `.github/copilot-instructions.md` - Global Copilot settings
- `.github/prompts/` - Reusable prompt files for specific tasks

### Assistant-Specific Guides
- `COPILOT_CHAT_CHEATSHEET.md` - Quick Copilot commands
- `PERPLEXITY_PROMPTS.md` - Research prompt templates
- `OPENAI_PROMPTS.md` - GPT-4/5 system prompts
- `CLAUDE_PROMPTS.md` - Claude-specific prompts
- `GEMINI_PROMPTS.md` - Gemini integration patterns

### Documentation
- `docs/ai-integration/` - Comprehensive guides and examples
- `AI_INTEGRATION_GUIDE.md` - Complete usage instructions

## SparkEngine Context

This prompt library is tailored specifically for SparkEngine's architecture:
- Enhanced DirectX 11 graphics with console integration
- Professional 3D audio system with spatial positioning
- Thread-safe camera system with real-time controls
- Advanced ImGui editor with docking and theming
- Comprehensive console system with 200+ commands
- Object pooling and performance optimization

## Development Workflow

1. **Start with Context**: Load project overview first
2. **Use Specific Prompts**: Choose task-appropriate prompts
3. **Apply Standards**: Use refactoring rules for consistency
4. **Test and Validate**: Generate tests and documentation
5. **Optimize**: Ensure performance requirements are met

## Best Practices

- Always request complete, compile-ready code
- Ensure thread safety and proper synchronization
- Include console integration for new features
- Add comprehensive error handling and assertions
- Generate professional XML documentation
- Validate performance impact and memory usage

For detailed usage instructions, see `docs/ai-integration/AI_INTEGRATION_GUIDE.md`.
