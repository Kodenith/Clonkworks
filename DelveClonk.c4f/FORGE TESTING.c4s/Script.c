/*-- Neues Szenario --*/

#strict

func Initialize() {
  FindObject2(Find_ID(FALW))->AddForgeable(METL, RGBa(177, 171, 170, 0), 3, 32, 2, "Meltable");
  FindObject2(Find_ID(FALW))->AddForgeable(CPIG, RGBa(170, 79, 66, 0), 4, 36, 3, "Meltable");
  
  ScriptGo(1);
  
  return(1);
  
}

func Script1(){
  var house = FindObject2(Find_ID(HUT3));
  AddEffect("Restock",house, 50, 1,house);
}

global func FxRestockTimer(object pTarget, int EffectNumber){
	var Itemlist;
	Itemlist = [WOOD, METL, CPIG, LORY, SPHR, CRYS, ARWP, FLNT, GUNP, CNKT, LNKT, CANN, XBOW];
	
	for(var id in Itemlist){
		if(ContentsCount(id, this()) == 0){
			CreateContents(id, this(), 1);
		}
	}
}
