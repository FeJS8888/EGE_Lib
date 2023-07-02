#define MAXCLONESCOUNT 500
#define MAXELEMENTCOUNT 100
#include "FeEGELib.h"
using namespace FeEGE;
int main() {

//	initgraph(-1,-1,INIT_NOBORDER | INIT_NOFORCEEXIT | INIT_TOPMOST | INIT_RENDERMANUAL);
	initgraph(-1,-1,INIT_NOBORDER | INIT_NOFORCEEXIT | INIT_RENDERMANUAL);
	
	PIMAGE block = newimage();
	getimage(block,".\\resources\\image\\block.png");
	putimage(nullptr,0,0,block);
	delay_ms(0);

	PlaySound(TEXT(".\\resources\\music\\test.wav"),NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);

	initpen();
	pen::font(50,"幼圆");
	pen::color(0xffffffff);
	pen::print(260,600,"点击进入时空传送门——属于2020级11班的回忆");

	Element* tp_door = newElement("tp_door",".\\resources\\image\\tpdoor\\tpdoor (1).png",getwidth() >> 1,(getheight() >> 1) - 100);
	tp_door->show();
	PIMAGE tp_door_image_stack[12];
	for(int i = 2; i <= 13; ++ i) {
		tp_door_image_stack[i - 1] = newimage();
		getimage(tp_door_image_stack[i - 1],(".\\resources\\image\\tpdoor\\tpdoor (" + to_string(i) + ").png").c_str());
		tp_door->add_image(tp_door_image_stack[i - 1]);
//		tp_door->
	}
	tp_door->set_scale(90);
	tp_door->remove_color(0x010102);
	tp_door->listen("frame",[](Element* e) {
		if(frame % 3 == 0) e->nextimage();

		long long statu = e->get_variable(2);
		if(statu != 0) {
			e->set_variable(2,statu - 1);
			e->setalpha(e->getalpha() - 5);
		} else if(e->get_variable(3)) {
			e->set_variable(3,false);
			pen::clear_all();
			e->hide();
			getElementById("close")->clone()->set_variable(2,-1);
//			pen::print(1400,950,"时");
		}
	});
	tp_door->listen("on_mouse_put_on",[](Element* e) {
		e->set_scale(100);
	});
	tp_door->listen("on_mouse_move_away",[](Element* e) {
		e->set_scale(90);
	});
	tp_door->listen("on_click",[](Element* e) {
//		cout<<"YES"<<endl;
		if(!e->get_variable(3) && e->get_variable(2) == 0) {
			e->set_variable(3,true);
			e->set_variable(2,51);
		}
	});

	Element* close = newElement("close",".\\remources\\image\\blank.png",0,0);
	close->hide();
	close->listen("frame",[](Element* e) {
		if(getkey(VK_ESCAPE)) closeGraph = true;
	});
	close->listen("clones:on_clone",[](Element* e) {
		push_schedule([]() {
			Element* e = getElementById("close_0");
			long long statu = e->get_variable(2);
			e->set_variable(2,0);
			switch(statu) {
				case(-1): {
					e->listen("frame",[](Element* e) {
						e->set_variable(2,e->get_variable(2) + 1);
						const int x = 900,y = 650;
						switch(e->get_variable(2)) { //打表
							case(2): {
								Sleep(2000);
								break;
							}
							case(3): {
								pen::print(x,y,"▂");
//					cout<<"P"<<endl;
								break;
							}
							case(83): {
								pen::clear_char(x,y);
								pen::print(x,y,"时▂");
								break;
							}
							case(163): {
								pen::clear_chars(x,y,2);
								pen::print(x,y,"时间▂");
								break;
							}
							case(243): {
								pen::clear_chars(x,y,3);
								pen::print(x,y,"时间:▂");
								break;
							}
							case(443): {
								pen::clear_chars(x,y,4);
								pen::print(x,y,"时间:");
								break;
							}
							case(643): {
								pen::clear_chars(x,y,3);
								pen::print(x,y,"时间:▂");
								break;
							}
							case(843): {
								pen::clear_chars(x,y,4);
								pen::print(x,y,"时间:");
								break;
							}
							case(1043): {
								pen::clear_chars(x,y,3);
								pen::print(x,y,"时间:▂");
								break;
							}
							case(1093): {
								pen::clear_chars(x,y,4);
								pen::print(x,y,"时间:2▂");
								break;
							}
							case(1143): {
								pen::clear_chars(x,y,5);
								pen::print(x,y,"时间:20▂");
								break;
							}
							case(1193): {
								pen::clear_chars(x,y,6);
								pen::print(x,y,"时间:202▂");
								break;
							}
							case(1243): {
								pen::clear_chars(x,y,7);
								pen::print(x,y,"时间:2020▂");
								break;
							}
							case(1293): {
								pen::clear_chars(x,y,8);
								pen::print(x,y,"时间:2020/▂");
								break;
							}
							case(1343): {
								pen::clear_chars(x,y,9);
								pen::print(x,y,"时间:2020/9▂");
								break;
							}
							case(1393): {
								pen::clear_chars(x,y,10);
								pen::print(x,y,"时间:2020/9/▂");
								break;
							}
							case(1443): {
								pen::clear_chars(x,y,11);
								pen::print(x,y,"时间:2020/9/3▂");
								break;
							}
							case(1493): {
								pen::clear_chars(x,y,12);
								pen::print(x,y,"时间:2020/9/30");
								break;
							}
							case(1495): {
								Sleep(1000);
								break;
							}
							case(1496): {
								getElementById("white")->clone();
								e->deletethis();
								break;
							}
						}
					});
					break;
				}
			}
		});
	});
//		cout<<e->get_variable(2)<<endl;


	Element* white = newElement("white",".\\resources\\image\\white.png",getwidth() >> 1,getheight() >> 1);
	white->hide();
	white->listen("clones:on_clone",[](Element* e) {
		e->setalpha(0);
		e->show();
		e->set_variable(2,25);
		e->listen("frame",[](Element* e) {
			if(e->get_variable(2) > 0) {
				e->increase_alpha(10);
				e->decrease_variable(2,1);
			} else if(e->get_variable(2) < -1) {
				e->decrease_alpha(10);
				e->increase_variable(2,1);
			} else if(e->get_variable(2) == -1) {
				getElementById("white_0")->deletethis();
			} else if(e->get_variable(2) == 0) {
				push_schedule([]() {
					Sleep(250);
					getElementById("white_0")->set_variable(2,-26);
					pen::clear_all();
				});
			}
		});
	});
	
//	Element* images = newElement("images",".\\resources\\image\\images\\2020年运动会\\image (1).jpg",getwidth() >> 1,getheight() >> 1);
//	images->hide();
//	PIMAGE images_image_stack[51];
//	for(int i = 2;i <= 50;++ i){
////		cout<<i<<endl;
//		images_image_stack[i] = newimage();
//		getimage(images_image_stack[i],TEXT((".\\resources\\image\\images\\2020年运动会\\image (" + to_string(i) + ").jpg").c_str()));
//		images->add_image(images_image_stack[i]);
//	}
//	images->listen("frame",[](Element* e){
//		if(frame % 5 == 0) e->nextimage();	
//	});
	start(0);
	return 0;
}
