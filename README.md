# 🔍 INSPECTR

**INSPECTR** is a lightweight Windows system monitoring CLI that provides real-time CPU, memory, storage, process, and GPU insights with JSON/CSV export support.

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

## 🖥️ Platform Compatibility

INSPECTR is designed and tested primarily on **Windows 11**.

- Fully supported on Windows 11
- Compatible with Windows 10, though minor formatting or metric discrepancies may occur due to OS-level reporting differences

---

## 📊 Validation & Accuracy

INSPECTR was validated against Windows Task Manager across 30 samples on Windows 10/11 systems.

- CPU usage mean deviation: ±2.3 percentage points
- Maximum observed CPU deviation: ~22.3 percentage points
- Memory usage deviation: consistently under ±1%
- Hardware identification accuracy: 100%

Observed CPU variance is attributed to differing sampling intervals and instantaneous versus averaged measurements between INSPECTR and Task Manager.


## 📬 Contact

- 🔗 **LinkedIn**: [Karanveer Panesar](https://www.linkedin.com/in/kpanesar88)
- 💻 **GitHub**: [github.com/kpanesar](https://github.com/kpanesar)
- 📧 **Email**: [karanveerpanesar04@gmail.com](mailto:karanveerpanesar04@gmail.com)

---

## 📄 License

This project is licensed under the **MIT License**.  
See the `LICENSE` file for details.



