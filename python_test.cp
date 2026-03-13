// C+ Python-style Keywords Test with Brackets
def greet(name) {
    if (name == None) {
        print("Hello, Stranger!");
    } elif (name == "Admin") {
        print("Welcome, System Administrator.");
    } else {
        print("Hello, " + name + "!");
    }
}

greet("Admin");
greet(None);
greet("User");

let i = 0;
while (i < 10) {
    i = i + 1;
    if (i == 5) {
        print("Skipping 5...");
        continue;
    }
    if (i == 8) {
        print("Breaking at 8...");
        break;
    }
    print(i);
}

if (True and not False) {
    print("Boolean logic with 'and' and 'not' works!");
}

pass; // Test pass statement
