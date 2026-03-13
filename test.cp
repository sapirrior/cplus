// 1. Define a recursive function for factorials
def factorial(n) {
    if (n <= 1) {
        return 1;
    };
    return n * factorial(n - 1);
};

// 2. Demonstrate Array support and mutation
let numbers = [10, 20, 30];
numbers[1] = 50; 

// 3. Complex logic using elif and boolean operators
def analyze(val) {
    if (val > 100 or val == 0) {
        print("Value is out of range");
    } elif (val > 50 and val <= 100) {
        print("Value is high");
    } else {
        print("Value is normal");
    };
};

// 4. Execution and Output
print("--- C+ Language Test ---");
print("Factorial of 5: " + factorial(5));
print("Modified Array: " + numbers);

analyze(numbers[1]); // Should print "Value is high"
analyze(0);          // Should print "Value is out of range"

let i = 0;
while (i < 3) {
    print("Loop iteration: " + i);
    i = i + 1;
};
