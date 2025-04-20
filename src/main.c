#include "common.h"

/*
 * entry
 */
void vm_main(void)
{
	/* initialize layer handle */
	layer_hdl[0] = -1;

	/* register system events handler */
	vm_reg_sysevt_callback(handle_sysevt);

	/* register keyboard events handler */
	vm_reg_keyboard_callback(handle_keyevt);

	/* Init MRE resource */
	vm_res_init();

	/* Initialize game state */
	reset_game();
	
	/* Initialize and start the game timer */
	game_timer = vm_create_timer_ex(GAME_SPEED_MS, timer_callback);
}

void handle_sysevt(VMINT message, VMINT param)
{
	switch (message)
	{
	case VM_MSG_CREATE:
	case VM_MSG_ACTIVE:
		if (layer_hdl[0] == -1)
		{
			layer_hdl[0] = vm_graphic_create_layer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, -1);
			vm_graphic_set_clip(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (game_timer == -1)
			{
				reset_game();
				game_timer = vm_create_timer_ex(GAME_SPEED_MS, timer_callback);
			}
		}
		break;

	case VM_MSG_PAINT:
		draw_game();
		break;

	case VM_MSG_INACTIVE:
		if (layer_hdl[0] != -1)
		{
			vm_graphic_delete_layer(layer_hdl[0]);
			layer_hdl[0] = -1;
		}
		if (game_timer != -1)
		{
			vm_delete_timer_ex(game_timer);
			game_timer = -1;
		}
		break;
	case VM_MSG_QUIT:
		if (layer_hdl[0] != -1)
		{
			vm_graphic_delete_layer(layer_hdl[0]);
			layer_hdl[0] = -1;
		}
		if (game_timer != -1)
		{
			vm_delete_timer_ex(game_timer);
			game_timer = -1;
		}
		vm_res_deinit();
		break;
	}
}

void handle_keyevt(VMINT event, VMINT keycode)
{
	if (event == VM_KEY_EVENT_DOWN)
	{
		if (game_over)
		{
			reset_game();
		}
		else
		{
			bird_vel = FLAP_STRENGTH;
		}
	}
	if (keycode == VM_KEY_RIGHT_SOFTKEY || keycode == VM_KEY_BACK)
	{
		vm_exit_app();
	}
}