# CallMeWin32kDriver
Load your driver like win32k.sys

![Q`NXJ7G@89G@K)6~5H8JA@6](https://user-images.githubusercontent.com/13917777/184930976-1ee5dd35-04a0-4d98-85a4-1f51074b9784.png)

![~MF %CSVW(FCL8H1G4UJ@6Y](https://user-images.githubusercontent.com/13917777/184935919-f9ee10ec-fbc3-48ef-8545-6269d078d0b1.png)


## Motivation
This feature was analyzed from a certain PUBG cheat driver.


## What it can do?
- Protection against direct dump by Anti-Rootkit tools
- Bypass MmCopyMemory
- Hide world does not trigger PG

## How to detect?
- Attach a GUI process before using MmCopyMemory

## Compile
- Visual Studio 2022 & WDK10
- llvm-msvc [[link]](https://github.com/NewWorldComingSoon/llvm-msvc-build)

## Some discussions on UnknownCheats
https://www.unknowncheats.me/forum/anti-cheat-bypass/511107-load-driver-win32k-sys.html
