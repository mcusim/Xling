#ifndef XG_ANIMATIONS_H_
#define XG_ANIMATIONS_H_ 1

#include "xling/graphics.h"
#include "xling/scenes/cda80050443c3d3b4622d46783d3ca1355b840e2.h"
#include "xling/scenes/7085f522b619a48bd1ae7c5a154eaba47eab189b.h"
#include "xling/scenes/7d62f01f91025380b4525ada427c269766a3ae51.h"
#include "xling/scenes/7085f522b619a48bd1ae7c5a154eaba47eab189b.h"
#include "xling/scenes/cda80050443c3d3b4622d46783d3ca1355b840e2.h"
#include "xling/scenes/6b53756df52ddcd5199bdd41761a58d4593499a6.h"
#include "xling/scenes/6128d681586ecf7fee7503f1d21dcd7d8121a887.h"
#include "xling/scenes/6b53756df52ddcd5199bdd41761a58d4593499a6.h"
#include "xling/scenes/cda80050443c3d3b4622d46783d3ca1355b840e2.h"
#include "xling/scenes/3f7e477d19ce67747b695ccce516715f626a877f.h"
#include "xling/scenes/09cde6c58ed0dc50f8f3b4706451e11b7c3277f2.h"
#include "xling/scenes/d96242ecde607c3dd52e6b2064f0dc7636a3dd11.h"
#include "xling/scenes/09cde6c58ed0dc50f8f3b4706451e11b7c3277f2.h"
#include "xling/scenes/3f7e477d19ce67747b695ccce516715f626a877f.h"
#include "xling/scenes/a67af497250a7436be1c08c7c621b1cacf48049a.h"
#include "xling/scenes/fb81c01f57e31f70357ee4c783af21b590680490.h"
#include "xling/scenes/aa8a17b7257cb3dfedba755f6152500515f7579f.h"
#include "xling/scenes/fb81c01f57e31f70357ee4c783af21b590680490.h"
#include "xling/scenes/a67af497250a7436be1c08c7c621b1cacf48049a.h"
#include "xling/scenes/fa3c88736f0ad1d5f17de17b093a80aa7bc97ffc.h"
#include "xling/scenes/713a49aa74b5c96be26961c2bde4c405975ce865.h"
#include "xling/scenes/fa3c88736f0ad1d5f17de17b093a80aa7bc97ffc.h"
#include "xling/scenes/a67af497250a7436be1c08c7c621b1cacf48049a.h"
#include "xling/scenes/8abb630a7eb5d726a2db9c2a2564790ec846d6cf.h"
#include "xling/scenes/37962fc3c16c97c2a15ba962effc87003c177664.h"
#include "xling/scenes/2e4e0b4778b421c6cd5d99be60a11576d4471ab4.h"
#include "xling/scenes/293d37a43f3094693fd4434737be41d353345f53.h"
#include "xling/scenes/127f9caa05ca5cfab9ebd7282154c786bc17babf.h"
#include "xling/scenes/a4a22152faab8db4a63de422ae0d5852fd35b04a.h"
#include "xling/scenes/a4a22152faab8db4a63de422ae0d5852fd35b04a.h"
#include "xling/scenes/a4a22152faab8db4a63de422ae0d5852fd35b04a.h"
#include "xling/scenes/fbdfa39e32d0758abedc3af42beab5d79f9b4fa7.h"

XG_AnimFrame_t XG_ANMF_RightEye[] = {
	{ .base_pt = { 40, 13 }, .alt = 8, .img = &XG_IMG_cda80050443c3d3b4622d46783d3ca1355b840e2, .alt_chance = 98, .stay = 4, },
	{ .base_pt = { 40, 13 }, .alt = 0, .img = &XG_IMG_7085f522b619a48bd1ae7c5a154eaba47eab189b, .alt_chance = 0, .stay = 0, },
	{ .base_pt = { 40, 13 }, .alt = 0, .img = &XG_IMG_7d62f01f91025380b4525ada427c269766a3ae51, .alt_chance = 0, .stay = 24, },
	{ .base_pt = { 40, 13 }, .alt = 0, .img = &XG_IMG_7085f522b619a48bd1ae7c5a154eaba47eab189b, .alt_chance = 100, .stay = 0, },
	{ .base_pt = { 40, 13 }, .alt = 0, .img = &XG_IMG_cda80050443c3d3b4622d46783d3ca1355b840e2, .alt_chance = 0, .stay = 0, },
	{ .base_pt = { 40, 13 }, .alt = 0, .img = &XG_IMG_6b53756df52ddcd5199bdd41761a58d4593499a6, .alt_chance = 0, .stay = 0, },
	{ .base_pt = { 40, 13 }, .alt = 0, .img = &XG_IMG_6128d681586ecf7fee7503f1d21dcd7d8121a887, .alt_chance = 0, .stay = 0, },
	{ .base_pt = { 40, 13 }, .alt = 0, .img = &XG_IMG_6b53756df52ddcd5199bdd41761a58d4593499a6, .alt_chance = 100, .stay = 0, },
	{ .base_pt = { 40, 13 }, .alt = 4, .img = &XG_IMG_cda80050443c3d3b4622d46783d3ca1355b840e2, .alt_chance = 3, .stay = 0, },
};
XG_Animation_t XG_ANM_RightEye = { .frames = XG_ANMF_RightEye, .frames_n = 9, .frame_idx = 0, .stay_cnt = 0, .active = 1, };

XG_AnimFrame_t XG_ANMF_Ears[] = {
	{ .base_pt = { 35, 6 }, .alt = 4, .img = &XG_IMG_3f7e477d19ce67747b695ccce516715f626a877f, .alt_chance = 96, .stay = 8, },
	{ .base_pt = { 35, 6 }, .alt = 0, .img = &XG_IMG_09cde6c58ed0dc50f8f3b4706451e11b7c3277f2, .alt_chance = 0, .stay = 1, },
	{ .base_pt = { 35, 6 }, .alt = 0, .img = &XG_IMG_d96242ecde607c3dd52e6b2064f0dc7636a3dd11, .alt_chance = 0, .stay = 8, },
	{ .base_pt = { 35, 6 }, .alt = 0, .img = &XG_IMG_09cde6c58ed0dc50f8f3b4706451e11b7c3277f2, .alt_chance = 0, .stay = 1, },
	{ .base_pt = { 35, 6 }, .alt = 0, .img = &XG_IMG_3f7e477d19ce67747b695ccce516715f626a877f, .alt_chance = 96, .stay = 8, },
};
XG_Animation_t XG_ANM_Ears = { .frames = XG_ANMF_Ears, .frames_n = 5, .frame_idx = 0, .stay_cnt = 0, .active = 1, };

XG_AnimFrame_t XG_ANMF_Tail[] = {
	{ .base_pt = { 6, 8 }, .alt = 8, .img = &XG_IMG_a67af497250a7436be1c08c7c621b1cacf48049a, .alt_chance = 75, .stay = 4, },
	{ .base_pt = { 6, 8 }, .alt = 0, .img = &XG_IMG_fb81c01f57e31f70357ee4c783af21b590680490, .alt_chance = 0, .stay = 0, },
	{ .base_pt = { 6, 8 }, .alt = 0, .img = &XG_IMG_aa8a17b7257cb3dfedba755f6152500515f7579f, .alt_chance = 0, .stay = 16, },
	{ .base_pt = { 6, 8 }, .alt = 0, .img = &XG_IMG_fb81c01f57e31f70357ee4c783af21b590680490, .alt_chance = 100, .stay = 0, },
	{ .base_pt = { 6, 8 }, .alt = 0, .img = &XG_IMG_a67af497250a7436be1c08c7c621b1cacf48049a, .alt_chance = 0, .stay = 4, },
	{ .base_pt = { 6, 8 }, .alt = 0, .img = &XG_IMG_fa3c88736f0ad1d5f17de17b093a80aa7bc97ffc, .alt_chance = 0, .stay = 0, },
	{ .base_pt = { 6, 8 }, .alt = 0, .img = &XG_IMG_713a49aa74b5c96be26961c2bde4c405975ce865, .alt_chance = 0, .stay = 4, },
	{ .base_pt = { 6, 8 }, .alt = 0, .img = &XG_IMG_fa3c88736f0ad1d5f17de17b093a80aa7bc97ffc, .alt_chance = 100, .stay = 0, },
	{ .base_pt = { 6, 8 }, .alt = 4, .img = &XG_IMG_a67af497250a7436be1c08c7c621b1cacf48049a, .alt_chance = 25, .stay = 4, },
};
XG_Animation_t XG_ANM_Tail = { .frames = XG_ANMF_Tail, .frames_n = 9, .frame_idx = 0, .stay_cnt = 0, .active = 1, };

XG_AnimFrame_t XG_ANMF_CalmLegs[] = {
	{ .base_pt = { 37, 48 }, .alt = 0, .img = &XG_IMG_8abb630a7eb5d726a2db9c2a2564790ec846d6cf, .alt_chance = 0, .stay = 0, },
};
XG_Animation_t XG_ANM_CalmLegs = { .frames = XG_ANMF_CalmLegs, .frames_n = 1, .frame_idx = 0, .stay_cnt = 0, .active = 1, };

XG_AnimFrame_t XG_ANMF_Legs[] = {
	{ .base_pt = { 37, 48 }, .alt = 0, .img = &XG_IMG_37962fc3c16c97c2a15ba962effc87003c177664, .alt_chance = 0, .stay = 1, },
	{ .base_pt = { 37, 48 }, .alt = 0, .img = &XG_IMG_2e4e0b4778b421c6cd5d99be60a11576d4471ab4, .alt_chance = 0, .stay = 1, },
	{ .base_pt = { 37, 48 }, .alt = 0, .img = &XG_IMG_293d37a43f3094693fd4434737be41d353345f53, .alt_chance = 0, .stay = 1, },
};
XG_Animation_t XG_ANM_Legs = { .frames = XG_ANMF_Legs, .frames_n = 3, .frame_idx = 0, .stay_cnt = 0, .active = 0, };

XG_AnimFrame_t XG_ANMF_ExyWalking[] = {
	{ .base_pt = { 6, 6 }, .alt = 0, .img = &XG_IMG_127f9caa05ca5cfab9ebd7282154c786bc17babf, .alt_chance = 0, .stay = 0, },
};
XG_Animation_t XG_ANM_ExyWalking = { .frames = XG_ANMF_ExyWalking, .frames_n = 1, .frame_idx = 0, .stay_cnt = 0, .active = 1, };

XG_AnimFrame_t XG_ANMF_HorDots[] = {
	{ .base_pt = { 75, 8 }, .alt = 0, .img = &XG_IMG_a4a22152faab8db4a63de422ae0d5852fd35b04a, .alt_chance = 0, .stay = 16, },
	{ .base_pt = { 79, 8 }, .alt = 0, .img = &XG_IMG_a4a22152faab8db4a63de422ae0d5852fd35b04a, .alt_chance = 0, .stay = 16, },
	{ .base_pt = { 83, 8 }, .alt = 0, .img = &XG_IMG_a4a22152faab8db4a63de422ae0d5852fd35b04a, .alt_chance = 0, .stay = 16, },
};
XG_Animation_t XG_ANM_HorDots = { .frames = XG_ANMF_HorDots, .frames_n = 3, .frame_idx = 0, .stay_cnt = 0, .active = 0, };

XG_AnimFrame_t XG_ANMF_ThoughtBubble[] = {
	{ .base_pt = { 63, 2 }, .alt = 0, .img = &XG_IMG_fbdfa39e32d0758abedc3af42beab5d79f9b4fa7, .alt_chance = 0, .stay = 0, },
};
XG_Animation_t XG_ANM_ThoughtBubble = { .frames = XG_ANMF_ThoughtBubble, .frames_n = 1, .frame_idx = 0, .stay_cnt = 0, .active = 0, };

#endif /* XG_ANIMATIONS_H_ */