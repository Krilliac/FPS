# SparkEngine AI Integration Guide

Welcome to the SparkEngine AI-powered development workflow. This guide explains 
how to use the comprehensive AI prompt library for efficient development.

## Quick Start

1. **Load Project Context**: Always start with project overview
   ```
   #prompt:00_project_overview
   ```

2. **Choose Your Task**: Use specific prompts for focused development
   - Editor work: `#prompt:02_editor_ui_tasks`
   - Graphics: `#prompt:03_graphics_dx11_tasks`
   - Audio: `#prompt:04_audio_system_tasks`
   - Performance: `#prompt:08_performance_memory`

3. **Apply Standards**: Use refactoring rules for code quality
   ```
   #prompt:01_code_refactor_rules
   /fix Apply to selected code
   ```

## File Structure
```
.github/
copilot-instructions.md        # Global Copilot configuration
prompts/                       # Reusable prompt library
00_project_overview.prompt.md
01_code_refactor_rules.prompt.md
02_editor_ui_tasks.prompt.md
... (more specialized prompts)

docs/ai-integration/               # Detailed documentation
assistant-guides/              # Per-assistant usage guides
examples/                      # Example workflows
best-practices/                # Development guidelines

COPILOT_CHAT_CHEATSHEET.md        # Quick Copilot reference
PERPLEXITY_PROMPTS.md             # Research prompt templates
OPENAI_PROMPTS.md                 # GPT-4/5 system prompts
CLAUDE_PROMPTS.md                 # Claude AI prompts
GEMINI_PROMPTS.md                 # Gemini prompts
```

## Best Practices

### Code Quality
- Always request complete, compile-ready code
- Ensure thread safety and proper synchronization
- Add comprehensive error handling and assertions
- Include console integration for new features
- Generate tests for new functionality

### Performance
- Request performance impact analysis
- Ask for memory usage optimization
- Ensure 60+ FPS maintenance with editor active
- Use object pooling for frequently allocated objects

### Documentation
- Generate XML documentation for all public APIs
- Update README and integration guides
- Create usage examples for complex features
- Document thread safety and performance characteristics

## Common Workflows

### Adding New Feature
1. Load project context: `#prompt:00_project_overview`
2. Use relevant system prompt (graphics, audio, etc.)
3. Request implementation with console integration
4. Ask for tests and documentation
5. Request performance analysis

### Refactoring Existing Code
1. Load refactoring rules: `#prompt:01_code_refactor_rules`
2. Use `/fix` with selected code
3. Request impact analysis
4. Generate tests for refactored code

### Performance Optimization
1. Load performance prompt: `#prompt:08_performance_memory`
2. Request profiling and analysis
3. Implement optimizations
4. Validate performance improvements

## Assistant-Specific Tips

### GitHub Copilot
- Use prompt files for consistency
- Leverage Agent mode for multi-file changes
- Reference specific files with `#file:` syntax
- Use slash commands for quick tasks

### Perplexity
- Use for research and best practices
- Request citations and code examples
- Compare different implementation approaches
- Research latest techniques and optimizations

### GPT-4/5, Claude, Gemini
- Use system prompts for context
- Request detailed analysis and explanations
- Generate documentation and guides
- Perform architectural reviews

## Support and Feedback
This AI prompt library is designed to evolve with the project. Suggest 
improvements, report issues, and contribute new prompts as the engine develops.
