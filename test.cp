// C+ JS-Style Test Script
let x = 10;
let y = 20;

if (x < y) {
    print("x is less than y!");
} else {
    print("x is greater!");
}

let arr = [1, 2, 3];
print("Original Array:");
print(arr);

arr[0] = 100;
print("Modified First Element:");
print(arr[0]);

fn factorial(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

print("Factorial of 5 is:");
print(factorial(5));

try {
    print("Attempting to divide by zero...");
    let z = 0;
    print(10 / z);
} catch {
    print("Successfully caught division by zero!");
}
