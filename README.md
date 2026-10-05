# White Noise POC

A minimal JUCE 9 VST3 instrument that generates continuous stereo white noise. It starts producing audio as soon as the instrument is loaded and has one output level parameter (0 to 100%). The small generic JUCE editor exposes the parameter without custom UI code.

## Build on Linux

Requirements: CMake 3.22+, a C++17 compiler, Git, and JUCE's Linux development dependencies. The build downloads JUCE 9.0.3 from the JUCE GitHub repository the first time CMake is run.

```sh
cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The VST3 bundle is created at:

```text
build/WhiteNoise_artefacts/Release/VST3/White Noise.vst3
```

Copy the bundle to `~/.vst3/`, then rescan plug-ins in your DAW. The VST3 produced here is a Linux build; a Windows FL Studio installation needs a Windows build of the same source.

## What the audio code does

For each output sample, the processor draws a fresh uniform random value in `[-1, 1]`, multiplies it by the smoothed Level parameter, and writes it to each channel. It allocates no memory in the audio callback. Noise is generated continuously rather than gated by MIDI notes.
