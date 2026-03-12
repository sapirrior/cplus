# Expressions & Operators

Expressions are pieces of code that "calculate" a value. C+ supports a variety of operators to work with numbers and strings.

## Arithmetic Operators
Used for mathematical calculations:
- `+` (Addition / String Concatenation)
- `-` (Subtraction)
- `*` (Multiplication)
- `/` (Division)

```basic
LET result = (10 + 5) * 2
PRINT "Hello " + "World"
```

## Comparison Operators
Used to compare two values, returning 1 for "true" and 0 for "false":
- `=` (Equal)
- `<>` (Not Equal)
- `<` (Less Than)
- `>` (Greater Than)
- `<=` (Less Than or Equal)
- `>=` (Greater Than or Equal)

```basic
IF score = 100 THEN
    PRINT "Perfect!"
END IF
```

## Order of Operations
C+ follows standard mathematical precedence (multiplication and division before addition and subtraction). Use parentheses `()` to group expressions and control the order of calculation.
