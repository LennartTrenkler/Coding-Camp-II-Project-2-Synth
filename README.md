# Project 2 – OpenFrameworks Synthesizer

A real-time audio synthesizer with live waveform visualisation, built with OpenFrameworks in C++.

## Features

- Five oscillator types: Sine, Triangle, Square, Sawtooth, Noise
- Adjustable frequency and volume via on-screen buttons
- Real-time waveform visualisation with animated cursor glow
- Frequency-based wave colour (blue → purple → red)
- RMS amplitude bar

## How to Run

**Windows (Visual Studio)**
1. Open `Project2.sln`
2. Select Debug x64
3. Build and run

**Mac (Xcode)**
1. Open the `.xcodeproj` file
2. Select your Mac as run target
3. Build and run with Cmd+R

## Controls

| Key | Action |
|-----|--------|
| 1 | Sine wave |
| 2 | Triangle wave |
| 3 | Square wave |
| 4 | Sawtooth wave |
| 5 | Noise |

Frequency and volume can also be adjusted with the on-screen − / + buttons.

## Built With

- [OpenFrameworks](https://openframeworks.cc/) v0.12
- C++17
- Xcode (Mac) / Visual Studio (Windows)
