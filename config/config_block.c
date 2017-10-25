#include "config_block.h"
#include "gfx/action_menu/gfx_item_action.h"
#include "gfx/action_menu/gfx_action_menu.h"
#include "gfx/gfx_components/gfx_information.h"
#include "gfx/gfx_components/gfx_label.h"
#include "gfx/gfx_components/gfx_image.h"

#define CONFIG_SECTION_ADDRESS 0xA000

struct gfx_mono_bitmap splash_bitmap;

static struct gfx_frame *dashboard;
static struct gfx_frame *clock;
static struct gfx_frame *splash;

uint8_t size_of_menus;
uint8_t new_fonts_size;

static void memcpy_config(void *dst, void *src_addr, size_t size)
{
	memcpy_PF(dst, (uint_farptr_t) (0x00010000 + (uint16_t)src_addr), size);
}

static struct gfx_image_node *load_frame_images(struct cnf_image_node *cnf_image_pgmem)
{
	struct gfx_image_node *frame_image_last = 0;
	struct gfx_image_node *image_head = NULL;
	while (cnf_image_pgmem) {
		struct cnf_image_node cnf_image_node;
		struct gfx_image_node *gfx_image_node = malloc_locked(sizeof(struct gfx_image_node));
		if (gfx_image_node == NULL)
			return NULL;

		memcpy_config(&cnf_image_node, cnf_image_pgmem, sizeof(struct cnf_image_node));
		gfx_image_node->image.bitmap = malloc_locked(sizeof(struct gfx_mono_bitmap));
		//TODO: Possible memory leak here fix later
		if (gfx_image_node->image.bitmap == NULL)
			return NULL;

		gfx_image_init(&gfx_image_node->image, cnf_image_node.image.bitmap_progmem, cnf_image_node.image.height,
					   cnf_image_node.image.width, cnf_image_node.image.x, cnf_image_node.image.y);

		gfx_image_node->next = 0;
		if (image_head == NULL)
			image_head = gfx_image_node;
		else
			frame_image_last->next = gfx_image_node;
		frame_image_last = gfx_image_node;
		cnf_image_pgmem = cnf_image_node.next;
	}
	return image_head;
}

static struct gfx_label_node *load_frame_labels(struct cnf_label_node *cnf_label_pgmem)
{
	struct gfx_label_node *frame_label_last = 0;
	struct gfx_label_node *label_head = NULL;
	while (cnf_label_pgmem) {
		struct cnf_label_node cnf_label_node;
		struct gfx_label_node *gfx_label_node = malloc_locked(sizeof(struct gfx_label_node));
		if (gfx_label_node == NULL)
			return NULL;

		memcpy_config(&cnf_label_node, cnf_label_pgmem, sizeof(struct cnf_label_node));
		gfx_label_init(&gfx_label_node->label, cnf_label_node.label.text, cnf_label_node.label.x, cnf_label_node.label.y, cnf_label_node.font_id);
		gfx_label_node->next = 0;
		if (label_head == 0)
			label_head = gfx_label_node;
		else
			frame_label_last->next = gfx_label_node;
		frame_label_last = gfx_label_node;
		cnf_label_pgmem = cnf_label_node.next;
	}
	return label_head;
}

static struct gfx_information_node *load_frame_infos(struct cnf_info_node *cnf_info_pgmem)
{
	struct gfx_information_node *frame_information_last = 0;
	struct gfx_information_node *information_head = NULL;
	while (cnf_info_pgmem) {
		struct cnf_info_node cnf_info_node;
		struct gfx_information_node *gfx_information_node = malloc_locked(sizeof(struct gfx_information_node));
		if (gfx_information_node == NULL)
			return NULL;

		memcpy_config(&cnf_info_node, (void *)cnf_info_pgmem, sizeof(struct cnf_info_node));
		struct cnf_info cnf_info = cnf_info_node.info;
		if (gfx_information_init(&gfx_information_node->information, &cnf_info, cnf_info_node.font_id))
			return NULL;

		gfx_information_node->next = 0;
		if (information_head)
			frame_information_last->next = gfx_information_node;
		else
			information_head = gfx_information_node;
		frame_information_last = gfx_information_node;
		cnf_info_pgmem = cnf_info_node.next;
	}
	return information_head;
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

static bool is_action_frame(struct cnf_frame *cnf_frame)
{
	struct cnf_info_node cnf_info_node;
	memcpy_config(&cnf_info_node, (void *)cnf_frame->infos_head, sizeof(struct cnf_info_node));
	enum information_type info_type = cnf_info_node.info.info_type;

	return (info_type == SET_BRIGHTNESS) || (info_type == SET_SCREEN_SAVER_ENABLE) ||
		   (info_type == SET_SCREEN_SAVER_TIME) || (info_type == SET_SCREEN_SAVER_TYPE);
	return 0;
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

		struct cnf_frame cnf_frame;
		memcpy_config(&cnf_frame, config_action.frame, sizeof(cnf_frame));
		if (is_action_frame(&cnf_frame)) {
			gfx_action_frame_init(action->frame, load_frame_images(cnf_frame.images_head),
								  load_frame_labels(cnf_frame.labels_head),
								  load_frame_infos(cnf_frame.infos_head));
		} else {
			gfx_context_frame_init(action->frame, load_frame_images(cnf_frame.images_head),
								   load_frame_labels(cnf_frame.labels_head),
								   load_frame_infos(cnf_frame.infos_head));
		}

		return 0;
	case ACTION_TYPE_SHOW_MENU:
		action->menu_id = config_action.menu_id;
		return 0;
	default:
		return 0;
	}
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

void show_logo(struct gfx_frame *frame);//TODO: temporary forward declaration. Remove later.

static void splash_init(struct cnf_blk config_block)
{
	gfx_frame_init(splash, NULL, NULL, NULL);
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

void gfx_action_menu_display(struct gfx_action_menu *action_menu); //TODO: temporary forward declaration

int load_config_block(void)
{
	struct cnf_blk config_block;
	struct cnf_menu config_menu;
	struct cnf_menu_node cnf_menu;
	struct cnf_frame cnf_frame;

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
		memcpy_config(&cnf_frame, config_block.dashboard, sizeof(cnf_frame));
		gfx_frame_init(dashboard, load_frame_images(cnf_frame.images_head), load_frame_labels(cnf_frame.labels_head),
						load_frame_infos(cnf_frame.infos_head));
	}

	if (config_block.clock) {
		clock = malloc_locked(sizeof(struct gfx_frame));
		if (clock == NULL)
			return -1;
		memcpy_config(&cnf_frame, config_block.clock, sizeof(cnf_frame));
		gfx_frame_init(clock, load_frame_images(cnf_frame.images_head), load_frame_labels(cnf_frame.labels_head),
						load_frame_infos(cnf_frame.infos_head));
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
