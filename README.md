<img width="1600" height="400" alt="peek1" src="https://github.com/user-attachments/assets/18e0ecf3-3684-46d4-8031-9f7be75c3ebe" />


# PEek
 
PEek is a PE file analysis tool for Windows, written in C++ with a Dear ImGui interface on DirectX 11.

The program inspects Windows PE binaries and identifies the compiler, the packer or protector

<img width="1108" height="582" alt="logo" src="https://github.com/user-attachments/assets/528460d0-417f-470e-a8c7-92de5145fb67" />

## Features
Compiler detection: MSVC, GCC, Go, Rust, Delphi, Clang

Packer and protector detection

Signature matching with wildcard byte patterns

.NET detection through the COM descriptor directory

Compile timestamp parsing with a heuristic for stale or forged `TimeDateStamp` values

## Building
PEek targets Windows

The project depends on Dear ImGui for the user interface, nanosvg for rendering the SVG background, and Capstone, which is linked but not currently used.

## Architecture
 
The analysis is split into modules from `f1mainmodule.cpp` to `f9packer.cpp`. Signature matching is implemented in `signature_scanner`, timestamp parsing and its heuristics in `timedatestampcheck`, and .NET detection in `dotnetdetect`. The interface is implemented in `uicore.cpp`.
 
Patterns in `signaturefinder` are stored as `short[]` arrays, where `-1` marks a wildcard byte.

## Background
 
The background supports SVG only. The original source file was lost. To use a custom background, place an SVG file next to the compiled binary.

    LoadSVGTexture(g_pd3dDevice, "logo.svg",
        (int)(700 * main_scale), (int)(500 * main_scale),
        &g_logoSRV, &g_logoW, &g_logoH);

## Signature sources
 
Compiler and packer signatures are derived from Detect It Easy (DIE) scripts and PE-bear `SIG.txt`.

https://github.com/horsicq/detect-it-easy

https://github.com/hasherezade/pe-bear/blob/main/SIG.txt

## Project status
 
PEek is no longer actively developed. Forks and contributions are welcome. The following work remains open.

### External signature database
 
Most signatures are hardcoded. Some hardcoded entries are intentional and should remain in the code. The remaining signatures should be moved to an external JSON file. This requires a well-designed JSON format, a loader, and integration with `signature_scanner`. Once the database exists, new signatures can be added without modifying the code. Signatures for ASProtect, Obsidium, tElock, Yoda's, Upack and MoleBox are missing, as are generic SFX heuristics.

### Capstone
 
Capstone is linked but not used. Disassembly-based analysis was planned and not implemented.

### Analyzer quality
 
Detection accuracy can be improved and the number of false positives reduced.
 
## License
 
See [LICENSE](https://github.com/dd1d3/PEek/blob/main/LICENSE).
