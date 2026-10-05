# White Noise POC

A minimal JUCE 9 VST3 instrument that generates continuous stereo white noise. It starts producing audio when loaded and has one output level parameter (0 to 100%). Its generic JUCE editor exposes the parameter without custom UI code.

## Get the Windows plug-in without installing Visual Studio

The public GitHub repository builds the Windows x64 VST3 on a free GitHub-hosted Windows runner. You do not need to install or pay for Visual Studio on your computer. Open the **Actions** tab, choose **Build Windows VST3**, and run the workflow. When it finishes, download the `White-Noise-Windows-x64-VST3` artifact and copy the included `White Noise.vst3` folder to **Program Files → Common Files → VST3**. Then scan for plug-ins in FL Studio.

The workflow also runs automatically when changes are pushed to `main`.

## Build on Linux

Requirements: CMake 3.22+, a C++17 compiler, Git, and JUCE's Linux development dependencies. The build downloads JUCE 9.0.3 from the JUCE GitHub repository the first time CMake is run.

```sh
cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The Linux VST3 bundle is created at:

```text
build/WhiteNoise_artefacts/Release/VST3/White Noise.vst3
```

Copy the bundle to `~/.vst3/`, then rescan plug-ins in your DAW.

## What the audio code does

For each output sample, the processor draws a fresh uniform random value in `[-1, 1]`, multiplies it by the smoothed Level parameter, and writes it to each channel. It allocates no memory in the audio callback. Noise is generated continuously rather than gated by MIDI notes.
