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

| Flag                     | Description                                |
|:-------------------------|:-------------------------------------------|
| `-h` `-help`             | Print help message                         |
| `-l` `-list`             | List available targets in the build file   |
| `-i` `-init [file]`      | Generates an initial buildfile             |
| `-v` `-version`          | Print version information and exit         |
| `-d` `-debug`            | Print debugging information                |
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
<project name="MyProject" default="build" description="My project">
    <target name="build" description="Build the project" depends="prepare">
        <echo msg="Building..." />
    </target>
    <target name="prepare">
        <echo msg="Preparing..." />
    </target>
</project>
```

- Targets with a `description` attribute appear under **Main targets** in `-l` output.
- Targets without a `description` appear under **Subtargets**.
- The `default` attribute on `<project>` sets the default target.
- The `depends` attribute lists comma-separated prerequisite targets.

See `example-buildfiles/` for examples.

## Status

Early development. Currently implemented:

- `-h` / `-help`
- `-l` / `-list`
- `-i` / `-init`
- `-v` / `-version`
- `-d` / `-debug`
- `-f` / `-buildfile`
