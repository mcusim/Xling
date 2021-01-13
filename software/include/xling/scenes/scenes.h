#ifndef XG_SCENES_H_
#define XG_SCENES_H_ 1

#include "xling/graphics.h"
#include "xling/scenes/anim.h"
#include "xling/scenes/5e62d51889446e4ab43915294297563486a73008.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"
#include "xling/scenes/9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc.h"
#include "xling/scenes/7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54.h"
#include "xling/scenes/7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54.h"
#include "xling/scenes/9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc.h"
#include "xling/scenes/9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc.h"
#include "xling/scenes/9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"

void XG_SCNKBD_smoking_02(XG_ButtonState_e, void *);

XG_Layer_t XG_SCNL_smoking_02[] = {
	 /* 0 */ { .obj = &XG_ANM_Ears, .obj_type = XG_OT_Animation },
	 /* 1 */ { .obj = &XG_ANM_RightEye, .obj_type = XG_OT_Animation },
	 /* 2 */ { .obj = &XG_ANM_Paws, .obj_type = XG_OT_Animation },
	 /* 3 */ { .obj = &XG_ANM_Tail, .obj_type = XG_OT_Animation },
	 /* 4 */ { .obj = &XG_ANM_CalmLegs, .obj_type = XG_OT_Animation },
	 /* 5 */ { .obj = &XG_ANM_Legs, .obj_type = XG_OT_Animation },
	 /* 6 */ { .obj = &XG_ANM_RightEyeMirror, .obj_type = XG_OT_Animation },
	 /* 7 */ { .obj = &XG_ANM_EarsMirror, .obj_type = XG_OT_Animation },
	 /* 8 */ { .obj = &XG_ANM_PawsMirror, .obj_type = XG_OT_Animation },
	 /* 9 */ { .obj = &XG_ANM_TailMirror, .obj_type = XG_OT_Animation },
	 /* 10 */ { .obj = &XG_ANM_CalmLegsMirror, .obj_type = XG_OT_Animation },
	 /* 11 */ { .obj = &XG_ANM_LegsMirror, .obj_type = XG_OT_Animation },
	 /* 12 */ { .obj = &XG_ANM_ExyWalking, .obj_type = XG_OT_Animation },
	 /* 13 */ { .obj = &XG_ANM_ExyWalkingMirror, .obj_type = XG_OT_Animation },
	 /* 14 */ { .obj = &XG_IMG_5e62d51889446e4ab43915294297563486a73008, .obj_type = XG_OT_Image, .base_pt = { 30, 61 } },
	 /* 15 */ { .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .obj_type = XG_OT_Image, .base_pt = { 26, 3 } },
	 /* 16 */ { .obj = &XG_IMG_9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc, .obj_type = XG_OT_Image, .base_pt = { 9, 3 } },
	 /* 17 */ { .obj = &XG_IMG_7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54, .obj_type = XG_OT_Image, .base_pt = { 35, 14 } },
	 /* 18 */ { .obj = &XG_IMG_7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54, .obj_type = XG_OT_Image, .base_pt = { 26, 47 } },
	 /* 19 */ { .obj = &XG_IMG_9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc, .obj_type = XG_OT_Image, .base_pt = { 28, 25 } },
	 /* 20 */ { .obj = &XG_IMG_9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc, .obj_type = XG_OT_Image, .base_pt = { 3, 36 } },
	 /* 21 */ { .obj = &XG_IMG_9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc, .obj_type = XG_OT_Image, .base_pt = { 37, 36 } },
	 /* 22 */ { .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .obj_type = XG_OT_Image, .base_pt = { 18, 14 } },
	 /* 23 */ { .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .obj_type = XG_OT_Image, .base_pt = { 11, 25 } },
	 /* 24 */ { .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .obj_type = XG_OT_Image, .base_pt = { 20, 36 } },
	 /* 25 */ { .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .obj_type = XG_OT_Image, .base_pt = { 9, 47 } },
};

XG_Scene_t XG_SCN_smoking_02 = {
	.layers = XG_SCNL_smoking_02,
	.layers_n = 26,
	.kbd_cbk = XG_SCNKBD_smoking_02,
};

#endif /* XG_SCENES_H_ */