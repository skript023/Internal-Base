<p align="center"><a title="Jeremy Kratz, Public domain, via Wikimedia Commons" href="https://commons.wikimedia.org/wiki/File:ISO_C%2B%2B_Logo.svg"><img width="256" alt="ISO C++ Logo" src="https://upload.wikimedia.org/wikipedia/commons/thumb/1/18/ISO_C%2B%2B_Logo.svg/256px-ISO_C%2B%2B_Logo.svg.png"></a></p>

<h1 align="center">Internal Base</h1>
<p align="center">
  <a href="https://github.com/skript023/Internal-Base/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/skript023/Internal-Base.svg?style=flat-square"/>
   </a>
  <a href="https://github.com/skript023/Gottvergessen-Loader/actions">
      <img src="https://img.shields.io/github/workflow/status/skript023/Internal-Base/CI/main?style=flat-square"/>
   </a>
  <br>
  An internal cheat base strictly for educational purpose only.
</p>

## Features
* ImGui–based user interface
* Log console
* Thread pool
* HTTP Request

## Building
To build Internal Base you need:
* Visual Studio 2022
* [Premake 5.0](https://premake.github.io) in your PATH

To set up the build environment, run the following commands in a terminal:
```dos
git clone https://github.com/skript023/Internal-Base.git --recurse-submodules
GenerateProjects.bat
```
Now, you will be able to open the solution, and simply build it in Visual Studio.