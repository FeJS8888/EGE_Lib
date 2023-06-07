#include "FeEGELib.h"
using FeEGE::getElementById;
using FeEGE::getkey; 

int main(){
	initgraph(700,500);
	
	//×¢²áElement
	
	PIMAGE choose_image = newimage();
	getimage(choose_image,".\\resources\\image\\choose.png");
	Element* choose_element = new Element("choose",choose_image,350,100);
	reg_Element(choose_element); 
	
	
	start(120);
}
