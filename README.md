# White Noise POC

A minimal JUCE 9 VST3 instrument that generates continuous stereo white noise. It starts producing audio when loaded and has one output level parameter (0 to 100%). Its generic JUCE editor exposes the parameter without custom UI code.

## Build and install the Windows VST3

The easiest route does not require a compiler on your computer. This public repo uses a free GitHub-hosted Windows runner to build the x64 plug-in.

- The build runs automatically when C++ source or build files are pushed to `main`.
- To build manually, open the repo's **Actions** tab, choose **Build Windows VST3**, and select **Run workflow** on `main`.
- When the run completes, download its `White-Noise-Windows-x64-VST3` artifact. Inside it, use the `White Noise.vst3` folder; the neighboring `.lib` and `.exp` files are build byproducts.
- Copy `White Noise.vst3` to `C:/Program Files/Common Files/VST3`.
- In FL Studio, open **Options → File settings → Manage plugins**, run **Find installed plugins** with **Verify plugins** enabled, then add **White Noise** from **Installed → Generators → VST3**.

The workflow in [`.github/workflows/build-windows-vst3.yml`](.github/workflows/build-windows-vst3.yml) runs these commands on the Windows runner:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel 4
```

The VST3 bundle is built at `build/WhiteNoise_artefacts/Release/VST3/White Noise.vst3`. CMake downloads JUCE 9.0.3 during configuration. For public repositories, GitHub's standard Windows-hosted runners are free and unlimited ([runner details](https://docs.github.com/en/actions/reference/runners/github-hosted-runners#standard-github-hosted-runners-for-public-repositories)).

## Copies installed on the Windows workstation

The built bundle has been copied to these locations:

- `C:/Program Files/Common Files/VST3/White Noise.vst3` — the standard Windows VST3 folder used by FL Studio's plug-in scan.
- `C:/Program Files/Image-Line/FL Studio 2024/Plugins/VST3/White Noise.vst3` — an additional copy placed inside the requested FL Studio installation folder.

Keep the first copy for FL Studio to scan. Image-Line's [plug-in installation guide](https://www.image-line.com/fl-studio-learning/fl-studio-online-manual/html/basics_externalplugins.htm) says VST3 plug-ins belong in the standard Windows VST3 folders, not the FL Studio installation's legacy `Plugins/VST` folder.

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
