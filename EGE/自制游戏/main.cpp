#define MAXCLONESCOUNT 10000
#include "FeEGELib.h"
using namespace FeEGE;

int main(void) {
	initgraph(1000,600);
	setbkcolor(EGERGB(72,201,182),nullptr);

	initpen();

	PIMAGE begin_image = newimage();
	getimage(begin_image,".\\resources\\image\\begin.png");
	Element *begin_element = newElement("begin_button",begin_image,500,410);

	begin_element->listen("on_click",[](Element* e) {
		log("[Event] Begin Game\n");
		e->deletethis();
		getElementById("background")->show();
		Element* player_element = getElementById("player");
		player_element->show();

		player_element->listen("frame",[](Element* e) {
			//Gravity
			e->move_down(6);
			if(e->is_touched_by(getElementById("background"))) e->move_up(6);
			
			//move
			if(getkey('a') || getkey('A')){
				e->move_left(5); 
				e->set_image(2);
			}
			if(getkey('d') || getkey('D')){
				e->move_right(5);
				e->set_image(1);
			}
			if(getkey(' ')){
				getElementById("ball")->clone();
			}
		});
		setbkcolor(EGERGB(143,209,117));
	});

	reg_Element(begin_element);


	PIMAGE background_image = newimage();
	getimage(background_image,".\\resources\\image\\bg.png");
	Element *background_element = newElement("background",background_image,500,560);
	background_element->hide();

	reg_Element(background_element);


	PIMAGE player_image = newimage();
	PIMAGE player_image1[2];
	getimage(player_image,".\\resources\\image\\player.png");
	Element *player_element = newElement("player",player_image,200,160);
	for(int i = 0;i < 2;++ i){
		player_image1[i] = newimage();
		getimage(player_image1[i],(".\\resources\\image\\player" + to_string(i) + ".png").c_str());
		player_element->add_image(player_image1[i]);
	}
	player_element->hide();
	player_element->decrease_scale(30);

	reg_Element(player_element);
	
	PIMAGE ball_image = newimage();
	getimage(ball_image,".\\resources\\image\\ball.png");
	Element *ball_element = newElement("ball",ball_image,-100,-100);
	ball_element->hide();
	ball_element->decrease_scale(60);
	
	ball_element->listen("clones:on_clone",[](Element* e){
		Element* ball_element = getElementById("ball");
		if(ball_element->get_variable(1) != 0){
			e->deletethis();
			return;
			
		}
		log("[Event] Shoot\n");
		Element* player_element = getElementById("player");
		if(player_element->get_image_order() == 0){
			e->deletethis();
			return;
		}
		Position pos = player_element->getposition();
		int statu = (player_element->get_image_order() - 1) * 2 - 1;
		e->move_to(pos.x - statu * 30,pos.y - 20);
		e->show();
		e->set_variable(2,statu);
		ball_element->set_variable(1,5);
		e->listen("frame",[](Element* e){
			if(e->get_variable(2) == -1) e->move_right(30);
			else e->move_left(30);
			Position pos = e->getposition();
			if(pos.x < 0 || pos.x > getwidth() || pos.y < 0 || pos.y > getheight()){
				e->deletethis();
				return;
			}
		});
//		e->move_to(pos.x )
	});

	ball_element->listen("frame",[](Element* e){
		if(e->get_variable(1) > 0) e->set_variable(1,e->get_variable(1) - 1);	
	});

	reg_Element(ball_element);


	start(120);
	return 0;
}
