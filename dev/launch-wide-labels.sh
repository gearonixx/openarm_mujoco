#!/usr/bin/env bash
set -euo pipefail

repo="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
python="$repo/.venv/bin/python"
mujoco_dir="$("$python" -c 'import pathlib, mujoco; print(pathlib.Path(mujoco.__file__).parent)')"
build_dir="$(mktemp -d)"
trap 'rm -rf -- "$build_dir"' EXIT
mujoco_library=("$mujoco_dir"/libmujoco.so.*)
cc -shared -fPIC -I "$mujoco_dir/include" "$repo/dev/wide_joint_labels.c" \
  -o "$build_dir/wide_joint_labels.so" "${mujoco_library[0]}" -Wl,-rpath,"$mujoco_dir" -ldl
cd -- "$repo"
export OPENARM_MUJOCO_LIBRARY="${mujoco_library[0]}"
LD_PRELOAD="$build_dir/wide_joint_labels.so${LD_PRELOAD:+:$LD_PRELOAD}" \
  "$repo/.venv/bin/openarm-mujoco-launch" "$@"
