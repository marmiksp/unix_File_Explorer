# Advanced OS
## File Explorer

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
d. User is able to navigate up and down in the file list using the corresponding
up and down arrow keys
2. Open directories and files When
enter key is pressed -
a. Directory - Clear the screen and navigate into the directory and show the directory
contents as specified in point 1
b. File - Open the file in vi editor
3. Traversal
a. Go back - Left arrow key will take the user to the previously visited directory
b. Go forward - Right arrow key will take the user to the next directory
c. Up one level - Backspace key will take the user up one level
d. Home - h key should take the user to the home folder (the folder where the
application was started)

## Command Mode:
The application enters the Command button whenever “:” (colon) key is pressed. In the
command mode, the user is able to enter different commands. All commands appear in the
status bar at the bottom.
1. Copy - ‘copy <source_file(s)> <destination_directory>’
Move - ‘move <source_file(s)> <destination_directory>’
Rename - ‘rename <old_filename> <new_filename>’
a. Eg - copy foo.txt bar.txt baz.mp4 ~/foobar
move foo.txt bar.txt baz.mp4 ~/foobar
rename foo.txt bar.txt
b. Assume that the destination directory exists and you have write permissions.
c. Copying/Moving directories is also implemented
d. The file ownership and permissions remains intact
2. Create File - ‘create_file <file_name> <destination_path>’
Create Directory - ‘create_dir <dir_name> <destination_path>’
a. Eg - create_file foo.txt ~/foobar
create_file foo.txt .
create_dir foo ~/foobar
3. Delete File - ‘delete_file <file_path>’
Delete Directory - ‘delete_dir <dir_path>’
a. On deleting directory, it will recursively delete all content present inside it.
4. Goto - ‘goto <location>’
a. Eg - goto <directory_path>5. Search - ‘search <file_name>’ or ‘search <directory_name>’
a. Search for a given file or folder under the current directory recursively.
b. Output should be True or False depending on whether the file or folder exists
6. On pressing ESC key, the application should go back to Normal Mode
7. On pressing q key, the application will close

#### Points to be Noted 

- In Normal Mode I am Showing 14 files at a time in a window and if thare are more than 14 files then we already have functionality of Verticle Overflow Scrolling Function.
- In Resize Functionality, I have provided resize facility but we can resize window till (100 X 35) lesser than this it will not work beacause my window is filled with other informations also like currentdir path, Home path, current file pointer etc.
- Home - The folder where the application was started.
- Root(/) - Absolute root of the system. 
- In my Command mode, when we press 'q' it will not exit because if in case the file name is having 'q' in its name then while writing such command with file having 'q' will cause sudden exit from application. Therefore I have implemented 'q' to exit in Normal Mode only.



