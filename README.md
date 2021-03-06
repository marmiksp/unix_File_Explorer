# Terminal Based File Explorer

This File Explorer is compatible with any [UNIX](https://unix.org) based Operating System.

## Installation and Execution

Use the BASH Terminal to install C++.

```bash
sudo apt-get install g++
```
Compile `main.cpp` using 
```bash
c++ main.cpp
```
Then Execute using
```bash
./a.out
```

## Modes
This application will work in two modes :

1. `Normal Mode` : used to explore the current directory and navigate the filesystem.

<img src="Screenshots/Normal_Mode.png">

2. `Command Mode` : used to enter and execute shell commands.

<img src="Screenshots/Command_Mode.png">

The **Home** of the application is the directory where the application will start.

## Switching Between Modes
Press `:` to switch from **NORMAL MODE** to **COMMAND MODE**.
Press `esc` to switch from **COMMAND MODE** to **NORMAL MODE**.

## Command Mode
Unless a Directory is Specified in the end of command. All commands will work on Current Working Directory.

1. `Copy` - copy <source_file(s)> <destination_directory> or <source_directory(s)> <destination_directory> 

2. `Move` - move <source_file(s)> <destination_directory>

3. `Rename` - rename <old_filename> <new_filename>

4. `Create File` - create_file <file_name> <destination_path>

5. `Create Directory` - create_dir <dir_name> <destination_path>

6. `Delete File or Directory ` - delete <file_path> or <dir_path> (<directory_to_delete_in>)

7. `Goto` - goto <location>

8. `Search` - search <file_name> or <directory_name>  (<directory_to_search_in>)

## Normal Mode
Number of Directory Contents visible at a time can be changed by changing `maxcontentrows` macro in the code.
1. `↑` `↓`  to Navigate  the contents of the Directory 
2. `←` `→`  to Navigate between previously visited Directories.
4. `BACKSPACE` to go one Level Up the Current Working Directory.
5. `h` to goto **HOME** Directory.
6. `ENTER` to Open a Directory or a File currently pointed by the Cursor.
7. `ctrl+q` to Quit the application.
8. `k` `l` to scroll contents up and down respectively (Not a recommended way of navigation).

## Assumptions
1. Terminal Window is Completely Resizable as contents will shown according to Window Size during the Execution of the Program. But we can resize window till `(100 X (maxcontentrows + 20))` lesser than this it will not work beacause my window is filled with other informations also like currentdir path, Home path, current file pointer etc.
2. Root(/) - Absolute root of the system.
3. `./` is assumed to be **RELATIVE DIRECTORY**.
4. `/` starting paths are assumed to be **Absolute Paths**.
5. `~` is assumed to **HOME DIRECTORY** (where Application starts).
6. Example for Command: `create_file foo.txt .` , `rename foo.txt bar.txt` etc