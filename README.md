# Project-eDot

RPG, turn-based and bullet hell game made for Computer Science A-level NEA  

## Dependencies
- Qt6 <
- QMake 3 <
- Linux: gcc 11.2 <
- Windows: MSVC 2019 < or MinGW 11.2 <

## Build instructions
1. Clone the repository and enter into the directory
```
git clone https://github.com/YaoReTian/Project-eDot.git
cd Project-eDot
```  
### Linux:  
2. Enter into the directory and build
```
qmake
make
```
3. Run the compiled binary
```
./Project-eDot
```  
### Windows:  
2. Locate the file location of qmake.exe (usually by default located at ```C:\Qt\{qt-version}\{c++ compiler version}\bin\qmake.exe```) 
3. Compile with qmake and MSVC or MinGW

MSVC
```
{file_path_to_qmake}\qmake.exe Project-eDot.pro -spec win32-msvc {current directory} qmake_all
```
MinGW
```
{file_path_to_qmake}\qmake.exe Project-eDot.pro
make
```
4. Execute compiled binary 
```
./Project-eDot.exe
```
