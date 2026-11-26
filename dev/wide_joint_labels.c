#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
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
  // Give labels a little more room within the standard panel width.
  spacing.label += 20;
  return spacing;
}

// The native viewer's Reset button calls mj_resetData. The articulated
// scene uses its "home" key for both the joint positions and motor targets.
void mj_resetData(const mjModel *model, mjData *data) {
  if (model->names && strcmp(model->names, "openarm_v20_articulated") == 0 &&
      model->nkey > 0) {
    int home = mj_name2id(model, mjOBJ_KEY, "home");
    if (home >= 0) {
      mj_resetDataKeyframe(model, data, home);
      return;
    }
  }
  typedef void (*ResetData)(const mjModel *, mjData *);
  ResetData original = (ResetData)dlsym(RTLD_NEXT, "mj_resetData");
  if (!original) {
    abort();
  }
  original(model, data);
}
