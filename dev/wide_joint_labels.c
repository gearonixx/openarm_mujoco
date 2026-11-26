#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include "mujoco/mujoco.h"

mjuiThemeSpacing mjui_themeSpacing(int index) {
  typedef mjuiThemeSpacing (*ThemeSpacing)(int);
  void *library = dlopen(getenv("OPENARM_MUJOCO_LIBRARY"), RTLD_LAZY | RTLD_NOLOAD);
  ThemeSpacing original = library ? (ThemeSpacing)dlsym(library, "mjui_themeSpacing") : NULL;
  if (!original) {
    abort();
  }
  mjuiThemeSpacing spacing = original(index);
  dlclose(library);
  // Both spacing themes need room for full OpenArm joint names.
  spacing.total += 112;
  spacing.label += 112;
  return spacing;
}
