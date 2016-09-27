# C Befunge Interpreter

## Supported Commands

| Command | Description                                                  |
| ------- | ------------------------------------------------------------ |
| 0...f   | push numerical value                                         |
| +       | pop a, b and push a + b                                      |
| -       | pop a, b and push b - a                                      |
| *       | pop a, b and push a * b                                      |
| /       | pop a, b and push b / a                                      |
| '       | push the next cell as an ascii                               |
| "       | start string mode, push next cells as ascii up to the next " |
| .       | pop numerical value from stack and print                     |
| ,       | pop ascii value from stack and print                         |
| :       | duplicate the top of the stack                               |
| k       | pop n, repeat next command n times                           |
| ^       | set instruction pointer delta to (0, 1)                      |
| v       | set instruction pointer delta to (0, -1)                     |
| <       | set instruction pointer delta to (-1, 0)                     |
| >       | set instruction pointer delta to (1, 0)                      |
| r       | reverses the instruction pointer delta                       |
| @       | halt                                                         |
