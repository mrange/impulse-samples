# 4KiB Windows Demo example
A non-optimal example on how to use tools that are available on github but not commonly known to build tiny executables.

I haven't included all trade-secrets I know. I want to leave the joy of discovery to others as well.

## Code License: WTFPL

Unless other specified all code is licensed under: [WTFPL](https://www.wtfpl.net/)

## Music License: TBD

Music provided by Virgill as an example on how to use Sointu to create tiny songs.

## Building

1. Open solution in VS2022.
2. Hit Ctrl-F5
3. Should work!

## Tools used to make it happen

### Crinkler

Github: https://github.com/runestubbe/Crinkler

Crinkler is a linker for Visual Studio that focuses on producing small binaries by applying compression and smart techniques.

Read the release notes for some pretty amazing facts on how Crinkler make your program small.

> Note: Crinkler produces a report at the build called `Report.html` that gives insight in what exactly takes up space in the final executable.

### Shader minifier

Github: https://github.com/laurentlb/Shader_Minifier

Rewrites my fat shader code into slim easy to compress shader code.

```bash
# Minifies shader into "readable" minified shader code
..\shader_minifier.exe .\shader.fx --format indented -o shader_mini.fx

# Minifies shader into "unreadable" minified shader code meant to be included in your project
..\shader_minifier.exe .\shader.fx --format c-array -o shader.inl
```

### Sointu

Github: https://github.com/vsariola/sointu

Music program that allows skilled musicians to produce amazing music that takes very little space.

Made by amazing Pestis.

```bash
# Generate assembler code from sointu song file
..\sointu-compile.exe -arch 386 -e "h,asm" uglyverse.yml
```

### 4Klang

Github: https://github.com/hzdgopher/4klang

Another tool to produce amazing music that is very compact. The only downside: You need to have some talent as a musician!


### PPPE

Download: https://www.mzrst.com/

Program that let's you inspect Windows executables.

Replacement to the classic `Depends.exe` that I can't make work anymore.

1. PPPE - To inspect files - https://www.mzrst.com/
3. Shader minifier - Minifies shader code - https://github.com/laurentlb/Shader_Minifier
4. Sointu - Produces music for compact binaries - https://github.com/vsariola/sointu
5. 4Klang - Produces music for compact binaries - https://github.com/hzdgopher/4klang

