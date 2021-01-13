#include <stdio.h>

/* Xling headers. */
#include "xling/graphics.h"
#include "xling/msg.h"

void
XG_SCNKBD_smoking_02(XG_ButtonState_e stat, void *arg)
{
	static uint8_t show_stat = 0;
	static uint8_t stat_lock = 0;
	static uint8_t right = 1;
	XG_SceneCtx_t *scene_ctx = (XG_SceneCtx_t *)arg;
	XG_Scene_t *scene = scene_ctx->scene;
	XG_Text_t *text = scene_ctx->text;
	XG_Point_t pt = { 0, 0 };
	XG_Animation_t *anim;

	switch (stat) {
	case XG_BTN_LEFT_PRESSED:
		/* Does Exy need to turn around? */
		if (right) {
			/* Turn around */
			for (uint8_t i = 0; i <= 5; i++) {
				anim = (XG_Animation_t *) scene->layers[i].obj;
				anim->active = 0;
			}
			anim = (XG_Animation_t *) scene->layers[12].obj;
			anim->active = 0;
			for (uint8_t i = 6; i <= 11; i++) {
				anim = (XG_Animation_t *) scene->layers[i].obj;
				anim->active = 1;
			}
			anim = (XG_Animation_t *) scene->layers[13].obj;
			anim->active = 1;
		}
		right = 0;

		/* Move all of the Exy animations */
		for (uint8_t i = 0; i <= 13; i++) {
			if (scene->layers[i].obj_type == XG_OT_Animation) {
				anim = (XG_Animation_t *) scene->layers[i].obj;
				for (uint8_t j = 0; j < anim->frames_n; j++) {
					anim->frames[j].base_pt.x -= 2;
				}
			}
		}

		/* Enable legs animation */
		anim = (XG_Animation_t *) scene->layers[10].obj;
		anim->active = 0;
		anim = (XG_Animation_t *) scene->layers[11].obj;
		anim->active = 1;

		/* Paws up! */
		anim = (XG_Animation_t *) scene->layers[8].obj;
		anim->frames[0].alt_chance = 0;
		anim->frames[1].alt_chance = 100;
		break;
	case XG_BTN_LEFT_RELEASED:
		if (!right) {
			/* Disable legs animation */
			anim = (XG_Animation_t *) scene->layers[10].obj;
			anim->active = 1;
			anim = (XG_Animation_t *) scene->layers[11].obj;
			anim->active = 0;

			/* Paws down! */
			anim = (XG_Animation_t *) scene->layers[8].obj;
			anim->frames[0].alt_chance = 100;
			anim->frames[1].alt_chance = 0;
		}
		break;
	case XG_BTN_RIGHT_PRESSED:
		/* Does Exy need to turn around? */
		if (!right) {
			/* Turn around */
			for (uint8_t i = 0; i <= 5; i++) {
				anim = (XG_Animation_t *) scene->layers[i].obj;
				anim->active = 1;
			}
			anim = (XG_Animation_t *) scene->layers[12].obj;
			anim->active = 1;
			for (uint8_t i = 6; i <= 11; i++) {
				anim = (XG_Animation_t *) scene->layers[i].obj;
				anim->active = 0;
			}
			anim = (XG_Animation_t *) scene->layers[13].obj;
			anim->active = 0;
		}
		right = 1;

		/* Move all of the Exy animations */
		for (uint8_t i = 0; i <= 13; i++) {
			if (scene->layers[i].obj_type == XG_OT_Animation) {
				anim = (XG_Animation_t *) scene->layers[i].obj;
				for (uint8_t j = 0; j < anim->frames_n; j++) {
					anim->frames[j].base_pt.x += 2;
				}
			}
		}

		/* Enable legs animation */
		anim = (XG_Animation_t *) scene->layers[4].obj;
		anim->active = 0;
		anim = (XG_Animation_t *) scene->layers[5].obj;
		anim->active = 1;

		/* Paws up! */
		anim = (XG_Animation_t *) scene->layers[2].obj;
		anim->frames[0].alt_chance = 0;
		anim->frames[1].alt_chance = 100;
		break;
	case XG_BTN_RIGHT_RELEASED:
		if (right) {
			/* Disable legs animation */
			anim = (XG_Animation_t *) scene->layers[4].obj;
			anim->active = 1;
			anim = (XG_Animation_t *) scene->layers[5].obj;
			anim->active = 0;

			/* Paws down! */
			anim = (XG_Animation_t *) scene->layers[2].obj;
			anim->frames[0].alt_chance = 100;
			anim->frames[1].alt_chance = 0;
		}
		break;
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
		/* All of the other keyboard events are ignored silently. */
		break;
	}

	if (show_stat == 1) {
		snprintf(text->text, text->text_sz, "%lu ms",
		    scene_ctx->frame_delay << 1);
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

void
XG_SCNKBD_test_brick(XG_ButtonState_e stat, void *arg)
{
	static uint8_t show_stat = 0;
	static uint8_t stat_lock = 0;
	static uint8_t y_lock = 0;
	XG_SceneCtx_t *scene_ctx = (XG_SceneCtx_t *)arg;
	XG_Scene_t *scene = scene_ctx->scene;
	XG_Text_t *text = scene_ctx->text;
	XG_Point_t pt = { 0, 0 };

	switch (stat) {
	case XG_BTN_LEFT_PRESSED:
		if (y_lock == 0) {
			y_lock = 1;
			scene->layers[0].base_pt.x--;
		}
		break;
	case XG_BTN_LEFT_RELEASED:
		if (y_lock == 1) {
			y_lock = 0;
		}
		break;
	case XG_BTN_RIGHT_PRESSED:
		if (y_lock == 0) {
			y_lock = 1;
			scene->layers[0].base_pt.y--;
		}
		break;
	case XG_BTN_RIGHT_RELEASED:
		if (y_lock == 1) {
			y_lock = 0;
		}
		break;
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
		/* All of the other keyboard events are ignored silently. */
		break;
	}

	if (show_stat == 1) {
		snprintf(text->text, text->text_sz, "%lu ms",
		    scene_ctx->frame_delay);
		pt.x = 85;
		pt.y = 10;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, "%u%%", scene_ctx->bat_lvl);
		pt.y = 21;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, (scene_ctx->bat_stat == 1)
		    ? "yes" : "no");
		pt.y = 32;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, "(%d,%d)",
		    scene->layers[0].base_pt.x, scene->layers[0].base_pt.y);
		pt.y = 43;
		XG_Print(scene_ctx->canvas, text, pt);
	}
}
