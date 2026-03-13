# C+ v0.0.2 Comprehensive Test
REM This is a comment

FUNCTION fib(n)
    IF n <= 1 THEN
        RETURN n
    END IF
    RETURN fib(n - 1) + fib(n - 2)
END FUNCTION

PRINT "Testing Recursion (Fibonacci 10):"
PRINT fib(10)

PRINT "Testing Boolean Logic:"
LET x = 10
LET y = 20
IF x < 15 AND y > 15 THEN
    PRINT "Logic AND works"
END IF

IF x > 15 OR y > 15 THEN
    PRINT "Logic OR works"
END IF

PRINT "Testing Built-in Math:"
PRINT SQRT(144)
PRINT ROUND(3.14159)

PRINT "Testing Error Handling (TRY/CATCH):"
TRY
    LET zero = 0
    PRINT 10 / zero
CATCH
    PRINT "Caught division by zero error!"
END TRY
