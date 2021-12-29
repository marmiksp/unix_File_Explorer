# Advanced OS
## Assignment #1 - File Explorer

File Explorer should work in two modes -
    1. Normal mode (default mode) - used to explore the current directory and navigate the filesystem
2. Command mode - used to enter shell commands


#### Points 

- In Normal Mode I am Showing 14 files at a time in a window and if thare are more than 14 files then we already have functionality of Verticle Overflow Scrolling Function.
- In Resize Functionality, I have provided resize facility but we can resize window till (100 X 35) lesser than this it will not work beacause my window is filled with other informations also like currentdir path, Home path, current file pointer etc.
- Home - The folder where the application was started.
- Root(/) - Absolute root of the system. 
- In my Command mode, when we press 'q' it will not exit because if in case the file name is having 'q' in its name then while writing such command with file having 'q' will cause sudden exit from application. Therefore I have implemented 'q' to exit in Normal Mode only.



