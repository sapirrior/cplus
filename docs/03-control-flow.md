# Control Flow in C+

Control flow allows your program to make decisions and repeat actions using `IF` statements and `FOR` loops.

## The IF Statement
The `IF` statement evaluates a condition. If the condition is "truthy" (any non-zero number or non-empty string), it executes the code between `THEN` and `END IF`.

```basic
LET x = 10
IF x > 5 THEN
    PRINT "x is high!"
END IF
```

Note: Every `IF` must end with `END IF`.

## The FOR Loop
The `FOR` loop repeats a block of code for a specific range of numbers.

```basic
FOR i = 1 TO 5
    PRINT i
NEXT
```

### How it works:
- `i = 1`: The loop starts with `i` equal to 1.
- `TO 5`: The loop continues as long as `i` is less than or equal to 5.
- `NEXT`: At the end of each pass, `i` increases by 1, and the loop starts again.
- Every `FOR` loop must end with the `NEXT` keyword.
