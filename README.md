# UICP

```
make
./uicp origin destination
```

https://github.com/raysan5/raylib/wiki/Working-on-macOS

## Building Library with Xcode

This guide has been written using the following software:
- OSX El Capitan (10.11.3)
- Xcode 7.2.1 (7C1002) 

_Steps:_

1) Get a Mac with OSX version 10.11.3.

2) Install *Apple Developer Tools*. Those tools include Xcode, in our case version 7.2.1. 

3) Install raylib library

##### With Homebrew

- If you don't want to build it yourself, install Homebrew by executing the following command in Terminal.app:  
```
    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
- Once Homebrew is installed, run the following command in Terminal:
```
    brew install raylib
```
- raylib installs a pkg-config file, which describes the necessary compilation and linker flags to use it with `yourgame`:
```
cc yourgame.c `pkg-config --libs --cflags raylib`
```


