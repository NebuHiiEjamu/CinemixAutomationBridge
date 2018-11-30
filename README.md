## Code guidelines
- Indentation must be strict tabs, no space characters.
- #includes must use the forward-slash directory notation
- Variable names must be lower camel case and be prefixed with their primitive type (eg. `iNumElements`):
..* `i` for signed integers
..* `u` for unsigned integers
..* `f` for floating point numbers
..* `s` for either `char*`, `char[]` or String objects
..* `b` for booleans
..* `p` for pointers. This must precede the prefix of the type of data it points to, if any. (eg. `piSomeNum`)
..* `pv` for `void*` pointers
..* Class/struct instance variable names need not be prefixed
- Class private variable names must be additionally prefixed with `m_`
- POD array and `Array<T>` variable names must be pluralised
- Function names must be lower camel case
- Classes/structs/unions/enums/typedefs must be upper camel case
- Classes and structs must be prefixed with `C`
- Do NOT separate parentheses with spaces either before or after in the case of function declarations, definitions, and calls
- DO separate parentheses with spaces when using clauses (eg. `while`, `if`, `do`, `switch`, etc.)
- Hexadecimal literals must be uppercased except for the `0x` prefix (eg. `0xDEADBEEF`) and must not use padded zeroes
- Curly braces must be on their own lines
- Do not use curly braces on single-statement `if`, `while`, `do`, `for` clauses.
- Use the `auto` type when the variable type is rather verbose (eg. `std::vector<int>::iterator`)
- Constant and preprocessor macro/symbol names must be all uppercase and use underscores where appropriate, save for JUCE's builtins.
- Functions returning a pointer or reference must place the `*` or `&` after the return type in the signature without spacing
- `const` must come before the type in function parameters
- Pass object instances to functions by reference whenever possible
- Pointer and reference variable and function argument names must be prefixed with `*` or `&` without spaces, not with the type (eg. `int *piNum`)
- Floating point literals with no exponent must end with `.0f
- Type casting must use `static_cast<>`, `dynamic_cast<>`, `const_cast<>` or `reinterpret_cast<>`
- Single line curly brace statements (usually initialising an array variable) must have a single space delimiting its contents
- Operators must usually be delimited by single spaces except increment/decrement and operator overload function signatures
- Function modifiers (eg. `const`, `override`, `delete`) must have a single place separating them from the closing parenthesis of a function signature
- Do not space apart semicolons from expressions unless in the rare case of an preprocessor conditional, in which case it must be on its own line, indented appropriately
- Prefer object instances over pointers in class fields if possible
- Keep class fields aligned in such a way that uses the least amount of memory.
