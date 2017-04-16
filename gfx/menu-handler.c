#include "menu-handler.h"
#include "gfx_components/gfx_information.h"
#include "gfx_components/gfx_label.h"
#include "gfx_components/gfx_image.h"
#include "action_menu/gfx_item_action.h"
#include "action_menu/gfx_action_menu.h"
#include "screen_saver/screen_saver.h"

struct gfx_mono_bitmap splash_bitmap;

struct gfx_frame *dashboard;

struct gfx_frame *clock;

struct gfx_frame *splash;

bool ok_button = false;
bool left_button = false;
bool right_button = false;

uint8_t size_of_menus;

uint8_t new_fonts_size;

void memcpy_config(void *dst, void *src_addr, size_t size)
{
	memcpy_PF(dst, (uint_farptr_t) (0x00010000 + (uint16_t)src_addr), size);
}

static void action_types_init(void)
{
	for (int i = 0; i < size_of_menus; i++) {
		struct gfx_action_menu *menu = action_menus[i];
		for (int j = 0; j < menu->menu->num_elements; j++) {
			struct gfx_item_action *action = &menu->actions[j];
			if (action->type == ACTION_TYPE_SHOW_MENU)
				set_menu_by_id(&(action->menu), action->menu_id);
		}
	}
}

static int load_action(struct gfx_item_action *action, struct cnf_action config_action, struct cnf_frame * cnf_dashboard)
{
	action->type = config_action.type;
	switch(config_action.type){
	case ACTION_TYPE_SET_BIOS_STATE:
	case ACTION_TYPE_SHOW_FRAME:
		if (config_action.frame == cnf_dashboard) {
			action->frame = dashboard;
			return 0;
		}

		action->frame = malloc_locked(sizeof(struct gfx_frame));
		if (action->frame == NULL)
			return -1;

		if (gfx_frame_init(action->frame, config_action.frame, false))
			return -1;

		break;
	case ACTION_TYPE_SHOW_MENU:
		action->menu_id = config_action.menu_id;
		break;
	default:
		break;
	}
	return 0;
}

void show_logo(struct gfx_frame *frame)
{
	update_screen_timer();
	clear_screen();
	frame_present = frame;
	display_state = DISPLAY_LOGO;
	gfx_mono_generic_put_bitmap(&splash_bitmap, 0, 0);
	gfx_mono_ssd1306_put_framebuffer();
}

static int graphic_item_init(struct gfx_image *menu_image, struct cnf_image * image_node)
{
	menu_image->bitmap = malloc_locked(sizeof(struct gfx_mono_bitmap));
	if (menu_image->bitmap == NULL)
		return -1;

	menu_image->bitmap->height = image_node->height;
	menu_image->bitmap->width = image_node->width;
	menu_image->bitmap->data.progmem = image_node->bitmap_progmem;
	menu_image->bitmap->type = GFX_MONO_BITMAP_SECTION;
	return 0;
}

static void splash_init(struct cnf_blk config_block)
{
	gfx_frame_init(splash, NULL, true);
	splash->draw = show_logo;
	splash_bitmap.width = config_block.splash_width;
	splash_bitmap.height = config_block.splash_height;
	splash_bitmap.data.progmem = config_block.splash;
	splash_bitmap.type = GFX_MONO_BITMAP_SECTION;
}

static int load_fonts(struct cnf_font_node *cnf_font_node)
{
	struct cnf_font_node font_node;
	struct glcd_FontConfig_t *font;
	if (fonts == NULL)
		return -1;

	while (cnf_font_node != 0) {
		memcpy_config(&font_node, cnf_font_node, sizeof(struct cnf_font_node));
		font = malloc_locked(sizeof(struct glcd_FontConfig_t));
		if (font == NULL)
			return -1;

		font->font_table = font_node.font.font_table;
		font->width = font_node.font.width;
		font->height = font_node.font.height;
		font->start_char = font_node.font.start_char;
		font->end_char = font_node.font.end_char;
		font->table_type = font_node.font.table_type;
		glcd_add_font(font, font_node.id);
		cnf_font_node = font_node.next;
	}
	return 0;
}

static int set_graphic_view(struct gfx_action_menu *action_menu, struct cnf_image_node *cnf_graphic_item_node)
{
	struct cnf_image_node cnf_image;
	struct gfx_mono_menu *mono_menu = action_menu->menu;
	if (cnf_graphic_item_node == 0) {
		action_menu->graphic_items_head = 0;
		return 0;
	}

	action_menu->graphic_items_head = malloc_locked(sizeof(struct gfx_image_node));
	if (action_menu->graphic_items_head == NULL)
		return -1;

	struct gfx_image_node *image_node = action_menu->graphic_items_head;
	for (uint8_t i = 0; i < mono_menu->num_elements; i++){
		if (cnf_graphic_item_node == 0)
			break;
		memcpy_config(&cnf_image, cnf_graphic_item_node, sizeof(struct cnf_image_node));
		if (graphic_item_init(&image_node->image, &cnf_image.image))
			return -1;

		cnf_graphic_item_node = cnf_image.next;
		if (i < mono_menu->num_elements - 1){
			image_node->next = malloc_locked(sizeof(struct gfx_image_node));
			if (image_node->next == NULL)
				return -1;

			image_node = image_node->next;
		} else {
			image_node->next = 0;
		}
	}

	return 0;

}

static int set_mono_menu(struct gfx_action_menu *action_menu, struct gfx_mono_menu *menu)
{
	struct gfx_mono_menu *mono_menu = malloc_locked(sizeof(struct gfx_mono_menu));
	if (mono_menu == NULL)
		return -1;

	memcpy_config(mono_menu, menu, sizeof(struct gfx_mono_menu));
	action_menu->is_progmem = true;
	action_menu->menu= mono_menu;
	action_menu->actions = malloc_locked(sizeof(struct gfx_item_action) * mono_menu->num_elements);
	if (action_menu->actions == NULL)
		return -1;

	return 0;
}

static int set_actions(struct gfx_action_menu * menu, struct cnf_action_node *cnf_action_node, struct cnf_frame * cnf_dashboard)
{
	struct cnf_action_node action_node;
	uint8_t action_index = 0;
	while (cnf_action_node) {
		memcpy_config(&action_node, cnf_action_node, sizeof(struct cnf_action_node));
		if (load_action(&(menu->actions[action_index]), action_node.action, cnf_dashboard))
			return -1;

		action_index++;
		cnf_action_node = action_node.next;
	}

	return 0;
}

int load_config_block(void)
{
	struct cnf_blk config_block;
	struct cnf_menu config_menu;
	struct cnf_menu_node cnf_menu;

	memcpy_config(&config_block, (void *)CONFIG_SECTION_ADDRESS, sizeof(struct cnf_blk));
	size_of_menus = config_block.menu_size;
	new_fonts_size = config_block.font_size;
	glcd_fonts_init(new_fonts_size);

	if (new_fonts_size > 0 && config_block.fonts_head && load_fonts(config_block.fonts_head))
			return -1;

	dashboard = clock = splash = NULL;
	if (config_block.dashboard) {
		dashboard = malloc_locked(sizeof(struct gfx_frame));
		if (dashboard == NULL)
			return -1;
		gfx_frame_init(dashboard, config_block.dashboard, true);
	}

	if (config_block.clock) {
		clock = malloc_locked(sizeof(struct gfx_frame));
		if (clock == NULL)
			return -1;
		gfx_frame_init(clock, config_block.clock, true);
	}

	if (config_block.splash) {
		splash = malloc_locked(sizeof(struct gfx_frame));
		if (splash == NULL)
			return -1;
		splash_init(config_block);
	}

	action_menus = malloc_locked(sizeof(struct gfx_action_menu *) * size_of_menus);
	if (action_menus == NULL)
		return -1;

	struct cnf_menu_node *cnf_menu_node = config_block.menus_head;
	for (int i = 0; i < size_of_menus; i++) {
		action_menus[i] = malloc_locked(sizeof(struct gfx_action_menu));
		if (action_menus[i] == NULL)
			return -1;
	}

	for (int i = 0; i < size_of_menus; i++) {
		if (cnf_menu_node == 0)
			break;

		memcpy_config(&cnf_menu, cnf_menu_node, sizeof(struct cnf_menu_node));
		memcpy_config(&config_menu, cnf_menu.menu, sizeof(struct cnf_menu));
		action_menus[config_menu.id]->id = config_menu.id;
		if ((set_mono_menu(action_menus[config_menu.id], config_menu.menu)) ||
				(set_graphic_view(action_menus[config_menu.id], config_menu.images_items_head)) ||
				(set_actions(action_menus[i], config_menu.actions_head, config_block.dashboard))) {
			return -1;
		}
		action_menus[config_menu.id]->draw = gfx_action_menu_display;
		cnf_menu_node = cnf_menu.next;
	}
	action_types_init();
	return 0;
}

void set_menu_by_id(struct gfx_action_menu **menu, uint8_t index)
{
	if (index < size_of_menus)
		*menu = action_menus[index];
}

void handle_back_to_menu(void)
{
	reset_screen_saver();
	clear_screen();
	frame_present = 0;
	enable_screen_saver_mode();
	present_menu->draw(present_menu);
}

static void handle_side_button(uint8_t keycode)
{
	switch(display_state) {
	case DISPLAY_DIM:
		present_menu->draw(present_menu);
		return;
	default:
		if (frame_present) {
			if (frame_present->information_head && frame_present->information_head->information.handle_buttons)
				frame_present->information_head->information.handle_buttons(keycode);
			else
				frame_present->handle_buttons(keycode);
		} else {
			gfx_action_menu_process_key(present_menu, keycode, false);
		}

		return;
	}
}

static void handle_buttons(void *data)
{
	if (ok_button) {
		handle_side_button(GFX_MONO_MENU_KEYCODE_ENTER);
		return;
	}

	if (left_button) {
		handle_side_button(GFX_MONO_MENU_KEYCODE_DOWN);
		return;
	}

	if (right_button) {
		handle_side_button(GFX_MONO_MENU_KEYCODE_UP);
		return;
	}
}

struct work button_work = { .do_work = handle_buttons, .data = NULL, .next = NULL, };

void handle_button_pressed(void)
{
	left_button = gpio_pin_is_high(FP_LEFT_BUTTON);
	right_button = gpio_pin_is_high(FP_RIGHT_BUTTON);
	ok_button = gpio_pin_is_high(FP_OK_BUTTON);
	insert_work(&button_work);
}
