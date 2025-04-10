# NewsReader CLI

A command-line interface application for reading news from various sources.

## Features

- Command-line parsing
- Extensible command system
- Help and version commands
- RSS feed parsing for New York Times and other news sources

## Building from Source

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- libcurl development headers
- libxml2 development headers

### Build Instructions

1. Clone the repository:
   ```
   git clone <repository-url>
   cd NewsReader
   ```

2. Create a build directory:
   ```
   mkdir build
   cd build
   ```

3. Configure and build:
   ```
   cmake ..
   cmake --build .
   ```

4. Run the application:
   ```
   ./bin/newsreader
   ```

## Usage

```
newsreader [command] [options]
```

### Available Commands

- `help`: Display help information
- `version`: Display version information
- `news`: Display latest news from New York Times
- `example`: An example command

For more information on a specific command, use:
```
newsreader help [command]
```

## License

[Specify your license here]

## Contributing

[Specify contribution guidelines here]
