# Code Refactoring Rules and Standards
Professional code quality standards and refactoring guidelines.

## Modern C++20 Standards
1. **Memory Management**
   - Use smart pointers (unique_ptr, shared_ptr) over raw pointers
   - RAII principles for resource management
   - Avoid manual memory allocation/deallocation
   - Implement move semantics for performance

2. **Type Safety**
   - Use auto keyword judiciously for type deduction
   - Prefer enum class over plain enums
   - Use constexpr for compile-time constants
   - Strong typing for function parameters

3. **Performance Optimization**
   - Pass large objects by const reference
   - Use move semantics for temporary objects
   - Prefer algorithms over raw loops
   - Cache-friendly data structures

## Thread Safety Requirements
- All public APIs must be thread-safe
- Use std::mutex for synchronization
- Document thread safety guarantees
- Avoid global mutable state

## Error Handling
- Use exceptions for exceptional cases
- Prefer ASSERT macros for development validation
- Return error codes or optional for expected failures
- Document all error conditions

## Console Integration
- All systems must provide console commands
- Real-time parameter adjustment capability
- Professional logging with rate limiting
- Command help and auto-completion

## Code Organization
- Single responsibility principle
- Dependency injection for testability
- Clear interface/implementation separation
- Professional documentation standards