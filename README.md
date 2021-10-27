# Folder-stats

Multithread app for creating short folder statistics

## Windows

Visual Studio 2019 project file is located in *VS 2019/folder_stats*\
Solution was tested on Windows 11

## Linux project usage
### Linux libraries for compilation
`sudo apt-get install gcc`\
`sudo apt-get install libgtest-dev`\
`sudo apt-get install cmake`

### Running on Linux:

Solution was tested on docker Ubuntu(20.04) container

#### Compilation:
execute `make` in *Linux/code* folder
#### Runnning:
execute `./folder_stats.out` in *Linux/code* folder after compilation

## Benchmarks Ubuntu vs Windows
![Benchmarks](https://user-images.githubusercontent.com/46225321/139062899-f5964f70-13f7-4fb3-9b33-70782a7cd555.png)
