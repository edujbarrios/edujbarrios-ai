#!/usr/bin/env python3
"""Run all declared algorithm tests; optional CUDA requires a GPU."""

import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def run(cmd, env=None):
    print("+", " ".join(map(str, cmd)), flush=True)
    subprocess.run([str(x) for x in cmd], cwd=ROOT, env=env, check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--language", choices=("c", "python", "rust", "r", "cuda"),
                        help="run tests for one language only (used by CI matrix)")
    parser.add_argument("--strict", action="store_true", help="fail if a declared CPU toolchain is absent")
    parser.add_argument("--with-cuda", action="store_true", help="compile and run declared CUDA tests (GPU required)")
    args = parser.parse_args()
    if subprocess.call([sys.executable, str(ROOT / "scripts" / "validate_repository.py")]) != 0:
        return 1
    specs = sorted((ROOT / "algorithms").glob("*/**/algorithm.json"))
    executed = 0
    with tempfile.TemporaryDirectory(prefix="edujbarrios-ai-") as tmp:
        for spec in specs:
            data = json.loads(spec.read_text(encoding="utf-8"))
            slug = data["slug"]
            for lang in data["languages"]:
                if args.language is not None and lang != args.language:
                    continue
                folder = spec.parent / f"{slug}_{lang}"
                if lang == "cuda" and not args.with_cuda:
                    print(f"SKIP {slug}/cuda (opt-in, requires NVIDIA GPU)")
                    continue
                binary = {"c": "cc", "rust": "rustc", "r": "Rscript", "cuda": "nvcc"}.get(lang)
                if binary and shutil.which(binary) is None:
                    message = f"Missing {binary}: {slug}/{lang}"
                    if args.strict or lang == "cuda":
                        raise RuntimeError(message)
                    print("SKIP", message)
                    continue
                output = Path(tmp) / f"{slug}_{lang}"
                if lang == "c":
                    run(["cc", "-std=c11", "-O2", "-Wall", "-Wextra", "-Werror",
                         "-pedantic", folder / f"{slug}.c", folder / f"test_{slug}.c",
                         "-lm", "-o", output])
                    run([output])
                elif lang == "python":
                    env = os.environ.copy()
                    env["PYTHONPATH"] = str(folder) + os.pathsep + env.get("PYTHONPATH", "")
                    run([sys.executable, "-m", "unittest", "discover", "-s",
                         folder, "-p", f"test_{slug}.py", "-v"], env=env)
                elif lang == "rust":
                    run(["rustc", "--edition=2021", "-D", "warnings", "--test",
                         folder / f"{slug}.rs", "-o", output])
                    run([output])
                elif lang == "r":
                    run(["Rscript", "--vanilla", folder / f"test_{slug}.R"])
                elif lang == "cuda":
                    run(["nvcc", "-std=c++17", folder / f"{slug}.cu",
                         folder / f"test_{slug}.cu", "-o", output])
                    run([output])
                executed += 1
    print(f"Passed {executed} language implementation test suite(s)"
          f" for {args.language or 'all requested CPU languages'}.")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, subprocess.CalledProcessError, RuntimeError) as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        sys.exit(1)
