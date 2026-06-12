#include <math.h>
#include "distortion.h"
#include "keyvalue.h"

using namespace upc;
using namespace std;

Distortion::Distortion(const std::string &param) {
  KeyValue kv(param);

  if (!kv.to_float("drive", drive))
    drive = 1.0;

  if (drive < 1e-6f) drive = 1e-6f;
  norm = tanhf(drive);
}

void Distortion::command(unsigned int comm) {
  (void) comm;
}

void Distortion::operator()(std::vector<float> &x) {
  for (unsigned int i = 0; i < x.size(); i++)
    x[i] = tanhf(drive * x[i]) / norm;
}
