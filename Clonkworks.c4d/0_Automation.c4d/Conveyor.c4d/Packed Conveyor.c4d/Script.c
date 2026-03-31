/*-- Neues Script --*/

#strict 2
#include WOOD

func Activate(){
	[$TxtUnpack$]
	if(GetCon() >= 100){
		var conv = CreateObject(CNVY);
		DoDamage(GetDamage(),conv);
		conv->Release();
	}else{	
		var conv = CreateObject(CNVB);
		DoDamage(GetDamage(),conv);
	}
	Sound("Connect");
	RemoveObject();
	return(1);
}

func IsAdvancedProduct(){ return(1); }

func CalcValue(){
	if(FindObject(ENDC)) return(0);
	return(7);
}

func CalcDefValue(){
	if(FindObject(ENDC)) return(0);
	return(7);
}