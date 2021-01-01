#ifndef XG_SCENES_H_
#define XG_SCENES_H_ 1

#include "xling/graphics.h"
#include "xling/scenes/anim.h"
#include "xling/scenes/db75fa2ae742b70c44314a1f345e9f4848e17174.h"
#include "xling/scenes/fbdfa39e32d0758abedc3af42beab5d79f9b4fa7.h"
#include "xling/scenes/5e62d51889446e4ab43915294297563486a73008.h"
#include "xling/scenes/7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54.h"
#include "xling/scenes/7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54.h"
#include "xling/scenes/9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc.h"
#include "xling/scenes/9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"

void XG_SCNKBD_smoking_02(XG_ButtonState_e, void *);

XG_Layer_t XG_SCNL_smoking_02[] = {
	{ .obj = &XG_ANM_RightEye, .obj_type = XG_OT_Animation },
	{ .obj = &XG_ANM_Ears, .obj_type = XG_OT_Animation },
	{ .obj = &XG_ANM_Tail, .obj_type = XG_OT_Animation },
	{ .obj = &XG_IMG_db75fa2ae742b70c44314a1f345e9f4848e17174, .obj_type = XG_OT_Image, .base_pt = { 6, 6 } },
	{ .obj = &XG_ANM_HorDots, .obj_type = XG_OT_Animation },
	{ .obj = &XG_IMG_fbdfa39e32d0758abedc3af42beab5d79f9b4fa7, .obj_type = XG_OT_Image, .base_pt = { 63, 2 } },
	{ .obj = &XG_IMG_5e62d51889446e4ab43915294297563486a73008, .obj_type = XG_OT_Image, .base_pt = { 30, 61 } },
	{ .obj = &XG_IMG_7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54, .obj_type = XG_OT_Image, .base_pt = { 35, 14 } },
	{ .obj = &XG_IMG_7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54, .obj_type = XG_OT_Image, .base_pt = { 26, 47 } },
	{ .obj = &XG_IMG_9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc, .obj_type = XG_OT_Image, .base_pt = { 28, 25 } },
	{ .obj = &XG_IMG_9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc, .obj_type = XG_OT_Image, .base_pt = { 37, 36 } },
	{ .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .obj_type = XG_OT_Image, .base_pt = { 18, 14 } },
	{ .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .obj_type = XG_OT_Image, .base_pt = { 11, 25 } },
	{ .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .obj_type = XG_OT_Image, .base_pt = { 20, 36 } },
	{ .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .obj_type = XG_OT_Image, .base_pt = { 9, 47 } },
};

const XG_Scene_t XG_SCN_smoking_02 = {
	.layers = XG_SCNL_smoking_02,
	.layers_n = 15,
	.kbd_cbk = XG_SCNKBD_smoking_02,
};

#endif /* XG_SCENES_H_ */