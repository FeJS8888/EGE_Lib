#define MAXCLONESCOUNT 10000
#include "FeEGELib.h"
using namespace FeEGE;

bool BlockedBySettings = false;

vector<string> names;
string name;

int main() {
	initgraph(700,500);
	setbkcolor(EGERGB(0,50,200));
	initpen();
	
	pen::font(40,"��Բ"); 
	pen::print(230,120,"�����������");
//	putimage_transparent(nullptr,pen_image,0,0,EGERGBA(1,1,4,0));
//
//	delay_ms(0);
//	getch();

	//��ȡ��Ϣ
	freopen(".\\config\\students.txt","r",stdin);
	while(cin>>name) names.push_back(name);
	if(names.size() == 0) MessageBox(getHWnd(),"��ѧ����Ϣ��\n���ڸ�Ŀ¼�µ�config\\students.txt������","��ʾ",MB_OK | MB_ICONWARNING);

	//ע��Element

	//choose_image����
	PIMAGE choose_image = newimage();
	getimage(choose_image,".\\resources\\image\\choose.png");
	Element* choose_element = new Element("choose",choose_image,350,150);
	reg_Element(choose_element);

//	choose_element->listen("frame",[](Element* e){
//		if(gettime() - e->get_variable(1) == 5){
////			pen::clear_all();
//			pen::print(230,120,"�����������");
//		}
//	});

	choose_element->listen("on_click",[](Element* e){
		if(names.size() == 0){
			MessageBox(getHWnd(),"��ѧ����Ϣ��\n���ڸ�Ŀ¼�µ�config\\students.txt������","��ʾ",MB_OK | MB_ICONWARNING);
			return;
		}
//		int statu = 4;
//		while(statu == 4) statu = MessageBox(getHWnd(),names[random(names.size())].c_str(),"ѡȡ���(��������ٴ�ѡȡ)",MB_RETRYCANCEL); 
		name = names[random(names.size())];
		int len = 0;
		for(int i = 0;i < name.length();++ i){
			if(name[i] >= 0 && name[i] < 256) len += 20;
			else len += 40;
		}
		pen::clear_all();
		pen::print((getwidth() - len) >> 1,120,name.c_str()); 
//		e->set_variable(1,gettime());
	});

	//opensettings����
	PIMAGE opensettings = newimage();
	getimage(opensettings,".\\resources\\image\\open_settings.png");
	Element* open_settings_element = new Element("open_settings",opensettings,350,350);
	reg_Element(open_settings_element);

	open_settings_element->listen("on_click",[](Element* e) { //openSettings
		if(BlockedBySettings == true) return;
		log("[Event] Open Settings\n");
		BlockedBySettings = true;
		Element* settings_element = getElementById("settings");
		settings_element->set_variable(1,1);
	});

	//settings����
	PIMAGE settings_image = newimage();
	getimage(settings_image,".\\resources\\image\\settings.png");
	Element* settings_element = new Element("settings",settings_image,350,250);
	reg_Element(settings_element);
	settings_element->set_scale(0);

	settings_element->listen("frame",[](Element* e) {
		if(e->get_variable(1) == 1) {
			if(e->getscale() != 100) e->increase_scale(10);
			else {
				Element* close_settings_element = getElementById("close_settings");
				close_settings_element->show();
				e->set_variable(1,0);
			}
		}
		if(e->get_variable(1) == 2) {
			if(e->getscale() != 0) e->decrease_scale(10);
			else {
				e->set_variable(1,0);
			}
		}
	});

	//close_settings����
	PIMAGE close_settings_image = newimage();
	getimage(close_settings_image,".\\resources\\image\\close_settings.png");
	Element* close_settings_element = new Element("close_settings",close_settings_image,550,30);
	reg_Element(close_settings_element);
	close_settings_element->hide();
	close_settings_element->decrease_scale(92);
	close_settings_element->listen("on_click",[](Element* e) {
		log("[Event] Close Settings\n");
		BlockedBySettings = false;
		Element* settings_element = getElementById("settings");
		Element* close_settings_element = getElementById("close_settings");
		settings_element->set_variable(1,2);
		close_settings_element->hide();
	});

	//snow_particle����
	PIMAGE snow_image = newimage();
	getimage(snow_image,".\\resources\\image\\snow.png");
	Element* snow_element = new Element("snow",snow_image,550,-10);
	reg_Element(snow_element);
	snow_element->hide();

	snow_element->listen("clones:on_clone",[](Element* e) {
		e->decrease_scale(40);
		e->set_posx(random(getwidth()));
	});

	snow_element->listen("frame",[](Element* e) {
		if(random(25) == 0) e->clone()->listen("frame",[](Element* e) {
			e->move_down(1.5);
			e->turn_right(3);
			if(e->getposition().y >= getheight() + 20) delete e->deletethis();
		});
	});

	start(120);
}
