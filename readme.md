# Advanced OS
## Assignment #1 - File Explorer

File Explorer should work in two modes -
1. Normal mode (default mode) - used to explore the current directory and navigate the filesystem
2. Command mode - used to enter shell commands

The application displays data starting from the top-left corner of the terminal window, line-by-
line.
It is able to handle text rendering if the terminal window is resized.


## Normal mode:
Normal mode is the default mode of your application. It should have the following functionalities -
1. Display a list of directories and files in the current folder
a. Every file in the directory should be displayed on a new line with the following
attributes for each file -
i.
File Name
ii.
File Size
iii.
Ownership (user and group) and Permissions
iv.
Last modified
b. The file explorer should show entries “.” and “..” for current and parent directory
respectively
c. The file explorer should handle scrolling in the case of vertical overflow using keys k &
l.
d. User should be able to navigate up and down in the file list using the corresponding
up and down arrow keys
2. Open directories and files When
enter key is pressed -
a. Directory - Clear the screen and navigate into the directory and show the directory
contents as specified in point 1
b. File - Open the file in vi editor
3. Traversal
a. Go back - Left arrow key should take the user to the previously visited directory
b. Go forward - Right arrow key should take the user to the next directory
c. Up one level - Backspace key should take the user up one level
d. Home - h key should take the user to the home folder (the folder where the
application was started)


#### Points 

- In Normal Mode I am Showing 14 files at a time in a window and if thare are more than 14 files then we already have functionality of Verticle Overflow Scrolling Function.
- In Resize Functionality, I have provided resize facility but we can resize window till (100 X 35) lesser than this it will not work beacause my window is filled with other informations also like currentdir path, Home path, current file pointer etc.
- Home - The folder where the application was started.
- Root(/) - Absolute root of the system. 
- In my Command mode, when we press 'q' it will not exit because if in case the file name is having 'q' in its name then while writing such command with file having 'q' will cause sudden exit from application. Therefore I have implemented 'q' to exit in Normal Mode only.



