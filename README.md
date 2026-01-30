# 🔍 INSPECTR

**INSPECTR** is a lightweight, Windows-focused system monitoring CLI that provides real-time insights into CPU, memory, storage, processes, and GPU information — with optional JSON and CSV export support.

---

## ✨ Features

- Real-time CPU usage, cores, threads, and base frequency
- Memory usage tracking (used / total / percentage)
- Multi-drive storage monitoring with labels and usage
- GPU information (name, VRAM usage, utilization*)
- Top memory-consuming processes
- ANSI-colored terminal output (clean & minimal)
- JSON and CSV export modes
- Configurable refresh intervals
- One-shot snapshot mode for scripting and automation

> ⚠️ GPU utilization accuracy depends on backend and driver support.

---

## 📦 Installation

Clone the repository and build using **Visual Studio (MSVC)** or another Windows-compatible C++ compiler.

```bash
git clone https://github.com/kpanesar88/INSPECTR.git
cd inspectr
```

🚀 Usage
Run live monitoring (default)

```
inspectr
```

Run once (snapshot mode)
```
inspectr --once
```
Set a custom refresh interval (milliseconds)
```
inspectr --interval 1500
```
Export output as JSON
```
inspectr --once --json
```
Export output as CSV
```
inspectr --once --csv
```
```
Redirect output to a file
inspectr --once --json > output.json
inspectr --once --csv  > output.csv
```



