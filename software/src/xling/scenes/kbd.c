#include <stdio.h>

/* Xling headers. */
#include "xling/graphics.h"
#include "xling/msg.h"

void
XG_SCNKBD_smoking_02(XG_ButtonState_e stat, void *arg)
{
	static uint8_t show_stat = 0;
	static uint8_t stat_lock = 0;
	XG_SceneCtx_t *scene_ctx = (XG_SceneCtx_t *)arg;
	XG_Scene_t *scene = scene_ctx->scene;
	XG_Text_t *text = scene_ctx->text;
	XG_Point_t pt = { 0, 0 };
	XG_Animation_t *dots = NULL;
	XG_Animation_t *thought_bubble = NULL;

	if (scene->layers[4].obj_type == XG_OT_Animation) {
		dots = (XG_Animation_t *) scene->layers[4].obj;
	}
	if (scene->layers[5].obj_type == XG_OT_Animation) {
		thought_bubble = (XG_Animation_t *) scene->layers[5].obj;
	}

	switch (stat) {
	case XG_BTN_LEFT_PRESSED:
		for (uint8_t i = 6; i <= 14; i++) {
			scene->layers[i].base_pt.x--;
		}
		break;
	case XG_BTN_RIGHT_PRESSED:
		for (uint8_t i = 6; i <= 14; i++) {
			scene->layers[i].base_pt.x++;
		}
		break;
	case XG_BTN_CENTER_PRESSED:
		if (stat_lock == 0) {
			stat_lock = 1;
			show_stat = (show_stat == 0) ? 1 : 0;

			if (dots != NULL) {
				dots->active = dots->active == 1 ? 0 : 1;
			}
			if (thought_bubble != NULL) {
				thought_bubble->active =
				    thought_bubble->active == 1 ? 0 : 1;
			}
		}
		break;
	case XG_BTN_CENTER_RELEASED:
		if (stat_lock == 1) {
			stat_lock = 0;
		}
		break;
	default:
		/* All of the other keyboard events are ignored silently. */
		break;
	}

	if (show_stat == 1) {
		snprintf(text->text, text->text_sz, "%lu ms",
		    scene_ctx->frame_delay);
		pt.x = 85;
		pt.y = 20;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, "%u%%", scene_ctx->bat_lvl);
		pt.y = 31;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, (scene_ctx->bat_stat == 1)
		    ? "yes" : "no");
		pt.y = 42;
		XG_Print(scene_ctx->canvas, text, pt);
	}
}

void
XG_SCNKBD_walking_01(XG_ButtonState_e stat, void *arg)
{
	static uint8_t show_stat = 0;
	static uint8_t stat_lock = 0;
	XG_SceneCtx_t *scene_ctx = (XG_SceneCtx_t *)arg;
	XG_Text_t *text = scene_ctx->text;
	XG_Point_t pt = { 0, 0 };

	switch (stat) {
	case XG_BTN_CENTER_PRESSED:
		if (stat_lock == 0) {
			stat_lock = 1;
			show_stat = (show_stat == 0) ? 1 : 0;
		}
		break;
	case XG_BTN_CENTER_RELEASED:
		if (stat_lock == 1) {
			stat_lock = 0;
		}
		break;
	default:
		break;
	}

	if (show_stat == 1) {
		snprintf(text->text, text->text_sz, "%lu ms",
		    scene_ctx->frame_delay);
		pt.x = 85;
		pt.y = 20;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, "%u%%", scene_ctx->bat_lvl);
		pt.y = 31;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, (scene_ctx->bat_stat == 1)
		    ? "yes" : "no");
		pt.y = 42;
		XG_Print(scene_ctx->canvas, text, pt);
	}
}
