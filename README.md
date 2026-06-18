# phingc

A C reimplementation of [Phing](https://www.phing.info/), the PHP build tool. Written in C23 as a learning project, targeting POSIX systems (macOS, Linux, WSL on Windows).

## Requirements

- CMake 4.2+
- A C23-compatible compiler (Clang or GCC)
- libxml2

### macOS

```bash
brew install cmake libxml2
```

### Linux / WSL

```bash
apt-get install cmake libxml2-dev
```

## Building

```bash
git clone <repository-url>
cd phingc
cmake -B cmake-build-debug
cmake --build cmake-build-debug
```

The binary will be at `cmake-build-debug/phingc`.

## Usage

```
phingc [options] [target [target2 [target3] ...]]
```

### Options

| Flag | Description |
|---|---|
| `-h` `-help` | Print help message |
| `-l` `-list` | List available targets in the build file |
| `-v` `-version` | Print version information and exit |
| `-d` `-debug` | Print debugging information |
| `-f` `-buildfile <file>` | Use given buildfile (default: `build.xml`) |

### Examples

Run the default target using `build.xml` in the current directory:

```bash
./phingc
```

List all targets in a specific build file:

```bash
./phingc -f path/to/build.xml -l
```

Print the version:

```bash
./phingc -v
```

## Build File Format

phingc uses the same XML build file format as Phing:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project name="MyProject" default="build">
    <target name="build">
        <echo msg="Building..." />
    </target>
</project>
```

See `example-buildfiles/` for examples.

## Status

Early development. Currently implemented:

- `-h` / `-help`
- `-v` / `-version`
- `-d` / `-debug`
- `-f` / `-buildfile`
- `-l` / `-list` (partial — target listing without dependency resolution)
