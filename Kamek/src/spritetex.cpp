#include <game.h>

void sandPillarZOrder(dStageActor_c *self, int param_2) {
    self->checkZoneBoundaries(param_2); // replaced

    self->pos.z = -6000.0f;
    OSReport("sandpillar Z pos: %f\n", self->pos.z);
}