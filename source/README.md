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

## Commands

```bash
# Minifies shader into "readable" minified shader code
..\shader_minifier.exe .\shader.fx --format indented -o shader_mini.fx

# Minifies shader into "unreadable" minified shader code
..\shader_minifier.exe .\shader.fx --format c-array -o shader.inl

# Generate assembler code from sointu song file
..\sointu-compile.exe -arch 386 -e "h,asm" uglyverse.yml
```
## Externals

1. PPPE - To inspect files - https://www.mzrst.com/
2. Crinkler - Replacement linker to produce compact binaries - https://github.com/runestubbe/Crinkler
3. Shader minifier - Minifies shader code - https://github.com/laurentlb/Shader_Minifier
4. Sointu - Produces music for compact binaries - https://github.com/vsariola/sointu
5. 4Klang - Produces music for compact binaries - https://github.com/hzdgopher/4klang



..\shader_minifier.exe .\shader.fx -o shader.h