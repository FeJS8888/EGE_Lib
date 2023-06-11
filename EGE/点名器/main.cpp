#include "FeEGELib.h"
using FeEGE::getElementById;
using FeEGE::getkey; 

void openSettings(Element* e){
	Element* settings_element = getElementById("settings");
	settings_element->show();
}

int main(){
	initgraph(700,500);
	
	//ע��Element
	
	//choose_image���� 
	PIMAGE choose_image = newimage();
	getimage(choose_image,".\\resources\\image\\choose.png");
	Element* choose_element = new Element("choose",choose_image,350,100);
	reg_Element(choose_element); 
	choose_element->listen("on_click",openSettings);
	//settings����
	PIMAGE settings_image = newimage();
	getimage(settings_image,".\\resources\\image\\settings.png");
	Element* settings_element = new Element("settings",settings_image,350,100);
	reg_Element(settings_element); 
	settings_element->hide();
	
	start(120);
}
