#!/usr/bin/env python3
"""Validate algorithm metadata, README presence and language directory layout."""

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOGUE = ROOT / "algorithms"
LANGUAGES = {"c": ".c", "python": ".py", "rust": ".rs", "r": ".R", "cuda": ".cu"}
LEVELS = {"foundation", "classical_ml", "neural_networks", "information_retrieval", "vector_search", "ai_systems", "large_language_models", "computer_vision", "vision_language_models"}
NAME = re.compile(r"^[a-z][a-z0-9]*(?:_[a-z0-9]+)*$")


def validate() -> int:
    errors = []
    count = 0
    collections = sorted(CATALOGUE.iterdir()) + [ROOT / name for name in ("llms", "vlms") if (ROOT / name).is_dir()]
    for category in collections:
        if not category.is_dir():
            continue
        if not (category / "README.md").is_file():
            errors.append(f"{category}: missing category README")
        for algorithm in sorted(category.iterdir()):
            if not algorithm.is_dir():
                continue
            count += 1
            slug = algorithm.name
            if not NAME.fullmatch(slug):
                errors.append(f"{algorithm}: slug must be snake_case")
            if not (algorithm / "README.md").is_file():
                errors.append(f"{algorithm}: missing algorithm README")
            metadata = algorithm / "algorithm.json"
            if not metadata.is_file():
                errors.append(f"{algorithm}: missing algorithm.json")
                continue
            try:
                data = json.loads(metadata.read_text(encoding="utf-8"))
            except (OSError, ValueError) as exc:
                errors.append(f"{metadata}: {exc}")
                continue
            if data.get("slug") != slug:
                errors.append(f"{metadata}: slug does not match folder")
            if data.get("level") not in LEVELS:
                errors.append(f"{metadata}: invalid level")
            if not isinstance(data.get("title"), str) or not data["title"].strip():
                errors.append(f"{metadata}: title is required")
            if not isinstance(data.get("summary"), str) or not data["summary"].strip():
                errors.append(f"{metadata}: summary is required")
            languages = data.get("languages")
            if not isinstance(languages, list) or not languages or len(languages) != len(set(languages)):
                errors.append(f"{metadata}: languages must be a nonempty unique list")
                continue
            for lang in languages:
                if lang not in LANGUAGES:
                    errors.append(f"{metadata}: unsupported language {lang!r}")
                    continue
                folder = algorithm / f"{slug}_{lang}"
                suffix = LANGUAGES[lang]
                if not (folder / f"{slug}{suffix}").is_file():
                    errors.append(f"{folder}: missing implementation {slug}{suffix}")
                if lang != "rust" and not (folder / f"test_{slug}{suffix}").is_file():
                    errors.append(f"{folder}: missing test test_{slug}{suffix}")
            known_folders = {f"{slug}_{lang}" for lang in languages if lang in LANGUAGES}
            for child in algorithm.iterdir():
                if child.is_dir() and child.name not in known_folders:
                    errors.append(f"{child}: language folder not declared in metadata")
    for error in errors:
        print("ERROR:", error, file=sys.stderr)
    print(f"Validated {count} algorithm(s); {len(errors)} error(s).")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(validate())
