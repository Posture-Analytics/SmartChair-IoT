### Code guide - Arduino/C++ 11

#### Comments

* Redundant comments should be avoided when the function call is self-explanatory. Example:
    ```c++
    // Print the buffer state
    printBufferState();
    ```

* Comments in the function definition should also be avoided, as it is difficult to keep the comments in the declaration and definition updated.

* Comments should start with a capital letter and should be a well-structured sentence.

* Inline comments are avoided, but can be made with two spaces before and one after the slashes.

* Comments should prioritize why the code does something instead of what it does, except in complex cases. However, if the code seems complex, it is recommended to break it down into simpler functions. Note: some people avoid breaking into functions if it means creating a short function with many parameters (unnecessary verbosity).

* It is recommended to use imperative verbs instead of the present indicative. Example:
    ```c++
    // Not recommended:
    // Gets the current date and stores it in a struct tm
    localtime(&seconds);

    // Recommended:
    // Get the current date and store it in a struct tm
    localtime_r(&seconds, &timeInfo);
    ```


### Style standardization

* Variable names should be as short as possible while maintaining the comprehension of their usage. They should follow the `camelCase` convention.

* Class names should be nouns and follow the `PascalCase` convention.

* There must be spaces between operators and punctuation. Example:
    ```c++
    for(int i=0;i<5;i++){  // not very readable
    for (int i = 0; i < 5; i++) {  // better
    ```

* Opening braces should be on the same line. Example:
    ```c++
    if (condition)  // valid choice, but the other one is already in use
    {
        // code
    }
    if (condition) {
        // code
    }
    ```

* Very long lines are avoided. The limit is 100 characters.

* It is recommended to use an editor that supports automatic removal of trailing whitespace. In Visual Studio Code, the option is `"files.trimTrailingWhitespace"`.

* It is also recommended that the file have a single blank line at the end.


#### General structure

The code must be modular, splitting the performed tasks into classes whenever possible, to facilitate expansion and compilation. Some recommendations can be made for better performance and predictability:

* Preserve encapsulation. Any attributes whose external access is not necessary should be private.

* Do not pass objects by copy. Whenever using a non-primitive type, use reference or pointer. Examples:
    ```c++
    void addData(HeavyJsonObject* data);
    // Copying primitive types is "cheaper"
    int increment(int value) {
        return value + 1;
    }
    ```

* Use `const` whenever possible, including at the end of function declarations that are members of objects. Examples:
    ```c++
    // Does not modify the object that has the function nor the object passed as a parameter
    void printData(const HeavyJsonObject& data) const;
    // Modifies only the parameter
    void addDataTo(HugeJsonObject& data) const;
    ```

* Avoid heap allocations. Dynamic memory opens up opportunities for memory leak and its allocation has a higher cost. This includes the use of the String class, especially in concatenation operations.

* Reserve the necessary memory when the size is known. Especially for the case above, if a single array is needed and its maximum size is known, it can be allocated only once and reused. Leave at least one character at the end for the null terminator (`'\0'`). Example:
    ```c++
    char buffer[32];
    long value = 2'147'483'647;
    snprintf(buffer, sizeof(buffer), "The value is %d", value);
    ```

Despite the recommendations, in cases where an operation is performed rarely or outside a loop, it is advantageous to use the method that provides the greatest readability and ease of maintenance.


#### Motivation

The motivation comes from experience with [cpplint](https://github.com/cpplint/cpplint) and [pylint](https://pypi.org/project/pylint/), but the decisions can be discussed and changed to a consensus among the developers.

Disclaimer: I don't follow everything I wrote here, but I try to.
