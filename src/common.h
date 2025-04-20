#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "vmtimer.h"
#include "vm4res.h"

#include "game.h"

VMINT		layer_hdl[1];
extern VMINT game_timer;

void handle_sysevt(VMINT message, VMINT param);
void handle_keyevt(VMINT event, VMINT keycode);